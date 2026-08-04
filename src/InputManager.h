#pragma once

#include <X11/Xlib.h>
#include <X11/keysym.h>



#include <unordered_set>

class InputManager
{
public:
    InputManager();
    ~InputManager() = default;

    bool initialize(Display *display, ::Window window);
    void shutdown();

    void update();

    bool shouldQuit() const;

    bool keyDown(KeySym key) const;

    bool rightButtonDown() const;

    int mouseDeltaX() const;
    int mouseDeltaY() const;

    void setRelativeMouseMode(bool enabled);
    bool relativeMouseMode() const;

private:

    Display *mDisplay{};
    ::Window mWindow{};

    bool mQuit{false};

    std::unordered_set<KeySym> mPressedKeys;

    bool mRightButton{false};

    int mMouseDx{0};
    int mMouseDy{0};

    int mMouseX{0};
    int mMouseY{0};
    bool mRelativeMouseMode = false;
};
