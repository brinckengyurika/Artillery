#include "InputManager.h"

#include <iostream>


InputManager::InputManager() :
    mDisplay(nullptr),
    mWindow(0),
    mQuit(false),
    mRightButton(false),
    mMouseDx(0),
    mMouseDy(0),
    mMouseX(0),
    mMouseY(0)
{
}

bool InputManager::initialize(Display *display, ::Window window)
{
    mDisplay = display;
    mWindow = window;

    if(!mDisplay || !mWindow)
    {
        std::cerr << "Invalid X11 handles!" << std::endl;
        return false;
    }

    XSelectInput(
        mDisplay,
        mWindow,
        KeyPressMask |
        KeyReleaseMask |
        ButtonPressMask |
        ButtonReleaseMask |
        PointerMotionMask |
        StructureNotifyMask
    );

    XFlush(mDisplay);

    std::cout << "InputManager initialized." << std::endl;

    return true;
}

void InputManager::update()
{
    mMouseDx = 0;
    mMouseDy = 0;

    while(XPending(mDisplay))
    {
        XEvent event;

        XNextEvent(mDisplay, &event);

        switch(event.type)
        {
            case KeyPress:
            {
                KeySym key = XLookupKeysym(&event.xkey, 0);

                if(key != NoSymbol)
                    mPressedKeys.insert(key);

                break;
            }

            case KeyRelease:
            {
                KeySym key = XLookupKeysym(&event.xkey, 0);

                if(key != NoSymbol)
                    mPressedKeys.erase(key);

                break;
            }

            case ButtonPress:
            {
                if(event.xbutton.button == Button3)
                    mRightButton = true;

                break;
            }

            case ButtonRelease:
            {
                if(event.xbutton.button == Button3)
                    mRightButton = false;

                break;
            }

            case MotionNotify:
            {
                mMouseDx += event.xmotion.x - mMouseX;
                mMouseDy += event.xmotion.y - mMouseY;

                mMouseX = event.xmotion.x;
                mMouseY = event.xmotion.y;

                break;
            }

            case DestroyNotify:
            {
                mQuit = true;
                break;
            }

            default:
                break;
        }
    }
}

bool InputManager::keyDown(KeySym key) const
{
    return mPressedKeys.find(key) != mPressedKeys.end();
}

bool InputManager::rightButtonDown() const
{
    return mRightButton;
}

int InputManager::mouseDeltaX() const
{
    return mMouseDx;
}

int InputManager::mouseDeltaY() const
{
    return mMouseDy;
}

bool InputManager::shouldQuit() const
{
    return mQuit;
}

void InputManager::shutdown()
{
    return;
}

