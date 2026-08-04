#include "InputManager.h"

#include <iostream>
#include <SDL_mouse.h>

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

    int mouse_x ;      // Koordináta az ablakon belül
    int mouse_y ;      // Koordináta az ablakon belül
    int root_x  ; // Koordináta a teljes képernyőn
    int root_y  ; // Koordináta a teljes képernyőn


    while(XPending(mDisplay))
    {
        XEvent event;

        XNextEvent(mDisplay, &event);
        switch(event.type)
        {
            case MotionNotify:
            {
                int mouse_x = event.xmotion.x;      // Koordináta az ablakon belül
                int mouse_y = event.xmotion.y;      // Koordináta az ablakon belül
                int root_x  = event.xmotion.x_root; // Koordináta a teljes képernyőn
                int root_y  = event.xmotion.y_root; // Koordináta a teljes képernyőn

                printf("Egér pozíció: %d, %d\n", mouse_x, mouse_y);
                mMouseDx += event.xmotion.x - mMouseX;
                mMouseDy += event.xmotion.y - mMouseY;

                mMouseX = event.xmotion.x;
                mMouseY = event.xmotion.y;


                break;
            }
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
                std::cout << "ButtonPress" << std::endl;
                if(event.xbutton.button == Button3)
                    mRightButton = true;

                break;
            }

            case ButtonRelease:
            {
                std::cout << "ButtonPress" << std::endl;
                if(event.xbutton.button == Button3)
                    mRightButton = false;

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

void InputManager::setRelativeMouseMode(bool enabled)
{
    if( enabled == mRelativeMouseMode )
        return;

    SDL_SetRelativeMouseMode(
        enabled ? SDL_TRUE : SDL_FALSE );

    mRelativeMouseMode = enabled;
}

bool InputManager::relativeMouseMode() const
{
    return mRelativeMouseMode;
}
