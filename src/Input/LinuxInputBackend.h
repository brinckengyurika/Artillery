
#pragma once

#include "IInputBackend.h"

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

#include <unordered_set>

class LinuxInputBackend : public IInputBackend
{
public:
    LinuxInputBackend(Display *display, ::Window window);
    ~LinuxInputBackend() override;

    bool initialize() override;
    void shutdown() override;
    void update() override;

    bool keyDown(KeySym key) const override;

    bool mouseButtonDown(unsigned int button) const override;
    bool mouseWheelForward() const;
    bool mouseWheelBackward() const;


    int mouseDeltaX() const override;
    int mouseDeltaY() const override;

    bool mouseCaptured() const override;
    void setMouseCaptured(bool captured) override;

    bool shouldQuit() const override;

private:
    bool initializeXInput2();
    bool selectRawMotionEvents();

    Display *mDisplay{};
    ::Window mWindow{};

    int mXiOpcode{-1};

    bool mInitialized{false};
    bool mQuit{false};
    bool mMouseCaptured{false};

    bool mMouseButtons[8]{};

    bool mMouseWheelForward{false};
    bool mMouseWheelBackward{false};

    int mMouseDx{0};
    int mMouseDy{0};

    std::unordered_set<KeySym> mPressedKeys;
};
