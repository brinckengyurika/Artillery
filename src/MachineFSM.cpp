#include "MachineFSM.h"

void MachineFSM::changeState(unsigned int newState)
{
    if (m_state == newState)
        return;

    m_state = newState;
    m_stateTime = 0.0f;
}

void MachineFSM::setState(unsigned int state)
{
    changeState(state);
}

void MachineFSM::start()
{
    if (m_state == IDLE)
        changeState(STARTING);
}

void MachineFSM::stop()
{
    if (m_state == RUNNING)
        changeState(STOPPING);
}

void MachineFSM::fail()
{
    changeState(ERROR);
}

void MachineFSM::reset()
{
    if (m_state == ERROR)
        changeState(IDLE);
}

void MachineFSM::update(float dt)
{
    if (dt <= 0.0f)
        return;

    m_stateTime += dt;

    switch (m_state)
    {
        case IDLE:
            break;

        case STARTING:
            if (m_stateTime >= 2.0f)
                changeState(RUNNING);
            break;

        case RUNNING:
            break;

        case STOPPING:
            if (m_stateTime >= 1.0f)
                changeState(IDLE);
            break;

        case ERROR:
            break;

        default:
            break;
    }
}
