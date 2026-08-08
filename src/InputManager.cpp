#include "InputManager.h"

#include "Input/LinuxInputBackend.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
    shutdown();
}

bool InputManager::initialize(
    Display *display,
    ::Window window)
{
    mBackend =
        std::make_unique<LinuxInputBackend>(
            display,
            window
        );

    return mBackend->initialize();
}

void InputManager::shutdown()
{
    if(mBackend)
        mBackend->shutdown();

    mBackend.reset();
}

void InputManager::update()
{
    if(mBackend)
        mBackend->update();
}

bool InputManager::keyDown(KeySym key) const
{
    if(!mBackend)
        return false;

    return mBackend->keyDown(key);
}

bool InputManager::mouseButtonDown(
    unsigned int button) const
{
    if(!mBackend)
        return false;

    return mBackend->mouseButtonDown(button);
}

bool InputManager::mouseCaptured() const
{
    if(!mBackend)
        return false;

    return mBackend->mouseCaptured();
}

void InputManager::setMouseCaptured(
    bool captured)
{
    if(mBackend)
        mBackend->setMouseCaptured(captured);
}

int InputManager::mouseDeltaX() const
{
    if(!mBackend)
        return 0;

    return mBackend->mouseDeltaX();
}

int InputManager::mouseDeltaY() const
{
    if(!mBackend)
        return 0;

    return mBackend->mouseDeltaY();
}

bool InputManager::shouldQuit() const
{
    if(!mBackend)
        return true;

    return mBackend->shouldQuit();
}
bool InputManager::mouseWheelForward() const
{
    if(!mBackend)
        return false;

    return mBackend->mouseWheelForward();
}

bool InputManager::mouseWheelBackward() const
{
    if(!mBackend)
        return false;

    return mBackend->mouseWheelBackward();
}
