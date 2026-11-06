#pragma once

class MachineFSM
{
public:
    // Állapotazonosítók
    static constexpr unsigned int IDLE     = 0;
    static constexpr unsigned int STARTING = 1;
    static constexpr unsigned int RUNNING  = 2;
    static constexpr unsigned int STOPPING = 3;
    static constexpr unsigned int ERROR    = 4;

    MachineFSM() = default;

    void start();
    void stop();
    void fail();
    void reset();

    void update(float dt);

    unsigned int getState() const
    {
        return m_state;
    }

    void setState(unsigned int state);

private:
    unsigned int m_state = IDLE;
    float m_stateTime = 0.0f;

    void changeState(unsigned int newState);
};
