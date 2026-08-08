#pragma once

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <memory>

class IInputBackend;

class InputManager
{
public:
    InputManager();
    ~InputManager();

    bool initialize(Display *display, ::Window window);
    void shutdown();

    void update();

    bool keyDown(KeySym key) const;
    bool mouseButtonDown(unsigned int button) const;

    bool mouseCaptured() const;
    void setMouseCaptured(bool captured);
    bool mouseWheelForward() const;
    bool mouseWheelBackward() const;
    int mouseDeltaX() const;
    int mouseDeltaY() const;

    bool shouldQuit() const;

private:
    std::unique_ptr<IInputBackend> mBackend;
};
