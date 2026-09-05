#include "LinuxInputBackend.h"

#include <iostream>
#include <cstring>

LinuxInputBackend::LinuxInputBackend(
    Display *display,
    ::Window window) :
    mDisplay(display),
    mWindow(window) {
}

LinuxInputBackend::~LinuxInputBackend() {
    shutdown();
}

bool LinuxInputBackend::initialize() {
    if(!mDisplay || !mWindow) {
        std::cerr
                << "LinuxInputBackend: invalid X11 handles."
                << std::endl;

        return false;
    }

    if(!initializeXInput2())
        return false;

    /*
     * Keyboard and button events are received from the
     * normal X11 event queue.
     */
    XSelectInput(
        mDisplay,
        mWindow,
        KeyPressMask |
        KeyReleaseMask |
        ButtonPressMask |
        ButtonReleaseMask |
        StructureNotifyMask
    );

    XFlush(mDisplay);

    mInitialized = true;

    std::cout
            << "LinuxInputBackend initialized."
            << std::endl;

    return true;
}

bool LinuxInputBackend::initializeXInput2() {
    int event = 0;
    int error = 0;

    if(!XQueryExtension(
                mDisplay,
                "XInputExtension",
                &mXiOpcode,
                &event,
                &error)) {
        std::cerr
                << "XInput extension is not available."
                << std::endl;

        return false;
    }

    int major = 2;
    int minor = 0;

    int result = XIQueryVersion(
                     mDisplay,
                     &major,
                     &minor
                 );

    if(result != Success) {
        std::cerr
                << "XInput2 is not available."
                << std::endl;

        return false;
    }

    std::cout
            << "XInput2 version: "
            << major
            << "."
            << minor
            << std::endl;

    return selectRawMotionEvents();
}

bool LinuxInputBackend::selectRawMotionEvents() {
    unsigned char mask[
     (XI_LASTEVENT + 7) / 8
    ] {};

    XISetMask(
        mask,
        XI_RawMotion
    );

    XIEventMask eventMask{};

    eventMask.deviceid = XIAllDevices;
    eventMask.mask_len = sizeof(mask);
    eventMask.mask = mask;

    int result = XISelectEvents(
                     mDisplay,
                     DefaultRootWindow(mDisplay),
                     &eventMask,
                     1
                 );

    XFlush(mDisplay);

    if(result != Success) {
        std::cerr
                << "Failed to select XInput2 events."
                << std::endl;

        return false;
    }

    std::cout
            << "XInput2 Raw Motion enabled."
            << std::endl;

    return true;
}

void LinuxInputBackend::update() {
    if(!mInitialized)
        return;

    mMouseWheelForward = false;
    mMouseWheelBackward = false;
    mMouseDx = 0;
    mMouseDy = 0;

    while(XPending(mDisplay)) {
        XEvent event;

        XNextEvent(
            mDisplay,
            &event
        );

        /*
         * XInput2 events are delivered as GenericEvent.
         */
        if(event.type == GenericEvent) {
            XGenericEventCookie *cookie =
                &event.xcookie;

            if(cookie->extension == mXiOpcode &&
                    XGetEventData(mDisplay, cookie)) {
                if(cookie->evtype == XI_RawMotion) {

                    XIRawEvent *raw =
                        static_cast<XIRawEvent *>(cookie->data);

                    int valueIndex = 0;

                    for(int valuator = 0;
                            valuator < raw->valuators.mask_len * 8;
                            ++valuator) {
                        if(!XIMaskIsSet(
                                    raw->valuators.mask,
                                    valuator)) {
                            continue;
                        }

                        double value =
                            raw->raw_values[valueIndex++];

                        if(valuator == 0) {
                            mMouseDx +=
                                static_cast<int>(value);
                        } else if(valuator == 1) {
                            mMouseDy +=
                                static_cast<int>(value);
                        }
                    }

                }
                XFreeEventData(
                    mDisplay,
                    cookie
                );
            }

            continue;
        }

        switch(event.type) {
        case KeyPress: {
            KeySym key =
                XLookupKeysym(
                    &event.xkey,
                    0
                );

            if(key != NoSymbol)
                mPressedKeys.insert(key);

            break;
        }

        case KeyRelease: {
            KeySym key =
                XLookupKeysym(
                    &event.xkey,
                    0
                );

            if(key != NoSymbol)
                mPressedKeys.erase(key);

            break;
        }
        /*
                case ButtonPress: {
                    unsigned int button =
                        event.xbutton.button;

                    if(button < 8)
                        mMouseButtons[button] = true;

                    if(button == Button3)
                        setMouseCaptured(true);

                    break;
                }
        */
        case ButtonPress: {
            unsigned int button =
                event.xbutton.button;

            if(button < 8)
                mMouseButtons[button] = true;

            if(button == Button3)
                setMouseCaptured(true);

            if(button == Button4)
                mMouseWheelForward = true;

            if(button == Button5)
                mMouseWheelBackward = true;

            break;
        }


        case ButtonRelease: {
            unsigned int button =
                event.xbutton.button;

            if(button < 8)
                mMouseButtons[button] = false;

            if(button == Button3)
                setMouseCaptured(false);

            break;
        }

        case DestroyNotify: {
            mQuit = true;
            break;
        }

        default:
            break;
        }
    }
}

bool LinuxInputBackend::keyDown(KeySym key) const {
    return
        mPressedKeys.find(key) !=
        mPressedKeys.end();
}

bool LinuxInputBackend::mouseButtonDown(
    unsigned int button) const {
    if(button >= 8)
        return false;

    return mMouseButtons[button];
}

int LinuxInputBackend::mouseDeltaX() const {
    return mMouseDx;
}

int LinuxInputBackend::mouseDeltaY() const {
    return mMouseDy;
}

bool LinuxInputBackend::mouseCaptured() const {
    return mMouseCaptured;
}

void LinuxInputBackend::setMouseCaptured(
    bool captured) {
    if(mMouseCaptured == captured)
        return;

    mMouseCaptured = captured;

    if(captured) {
        XGrabPointer(
            mDisplay,
            mWindow,
            True,
            ButtonPressMask |
            ButtonReleaseMask |
            PointerMotionMask,
            GrabModeAsync,
            GrabModeAsync,
            mWindow,
            None,
            CurrentTime
        );

        XDefineCursor(
            mDisplay,
            mWindow,
            None
        );
    } else {
        XUngrabPointer(
            mDisplay,
            CurrentTime
        );
    }

    XFlush(mDisplay);
}

bool LinuxInputBackend::shouldQuit() const {
    return mQuit;
}

void LinuxInputBackend::shutdown() {
    if(!mDisplay)
        return;

    if(mMouseCaptured)
        setMouseCaptured(false);

    mPressedKeys.clear();

    mInitialized = false;
}

bool LinuxInputBackend::mouseWheelForward() const
{
    return mMouseWheelForward;
}

bool LinuxInputBackend::mouseWheelBackward() const
{
    return mMouseWheelBackward;
}
