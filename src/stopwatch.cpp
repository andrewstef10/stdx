#include <stdx/stopwatch.h>

namespace stdx {

Stopwatch::Stopwatch()
    : m_running(false)
{
    reset();
}

void Stopwatch::start(bool resetStopwatch)
{
    if (!m_running)
    {
        if (resetStopwatch)
        {
            reset();
        }
        else
        {
            m_startTime = now();
        }
        m_running = true;
    }
}

void Stopwatch::stop()
{
    if (m_running)
    {
        m_stopTime = now();
        m_running = false;
    }
}

void Stopwatch::reset()
{
    m_startTime = now();
    m_stopTime = m_startTime;
}

double Stopwatch::getElapsedTimeSec() const
{
    return getElapsedTime<std::chrono::duration<double, std::chrono::seconds::period>>().count();
}
} // namespace stdx
