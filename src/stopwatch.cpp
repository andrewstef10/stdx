#include <stdx/stopwatch.h>

namespace stdx {

    stopwatch::stopwatch()
    : m_startTime()
    , m_stopTime()
    , m_running(false)
    {
        reset();
    }

    stopwatch::stopwatch(const stopwatch& other)
    : m_startTime(other.m_startTime)
    , m_stopTime(other.m_stopTime)
    , m_running(other.m_running)
    {
    }

    stopwatch::~stopwatch()
    {
    }

    stopwatch& stopwatch::operator=(const stopwatch& other)
    {
        m_startTime = other.m_startTime;
        m_stopTime = other.m_stopTime;
        m_running = other.m_running;
        return *this;
    }

    void stopwatch::start(bool resetStopwatch)
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

    void stopwatch::stop()
    {
        if (m_running)
        {
            m_stopTime = now();
            m_running = false;
        }
    }

    void stopwatch::reset()
    {
        m_startTime = now();
        m_stopTime = m_startTime;
    }

    double stopwatch::get_elapsed_time_sec() const
    {
        return get_elapsed_time<std::chrono::duration<double, std::chrono::seconds::period>>().count();
    }
}