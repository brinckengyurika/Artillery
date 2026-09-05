#pragma once

#include <X11/Xlib.h>
#include <X11/keysym.h>

class IInputBackend
{
public:
    virtual ~IInputBackend() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update() = 0;
    virtual bool mouseWheelForward() const = 0;
    virtual bool mouseWheelBackward() const = 0;
    virtual bool keyDown(KeySym key) const = 0;

    virtual bool mouseButtonDown(
        unsigned int button) const = 0;

    virtual int mouseDeltaX() const = 0;
    virtual int mouseDeltaY() const = 0;

    virtual bool mouseCaptured() const = 0;
    virtual void setMouseCaptured(bool captured) = 0;

    virtual bool shouldQuit() const = 0;
};
