#include <stdads/Stopwatch.h>

namespace stdads {

    Stopwatch::Stopwatch()
    : startTime_()
    , stopTime_()
    , running_(false)
    {
        Reset();
    }

    Stopwatch::Stopwatch(const Stopwatch& other)
    : startTime_(other.startTime_)
    , stopTime_(other.stopTime_)
    , running_(other.running_)
    {
    }

    Stopwatch::~Stopwatch()
    {
    }

    Stopwatch& Stopwatch::operator=(const Stopwatch& other)
    {
        startTime_ = other.startTime_;
        stopTime_ = other.stopTime_;
        running_ = other.running_;
        return *this;
    }

    void Stopwatch::Start(bool reset)
    {
        if (!running_)
        {
            if (reset)
            {
                Reset();
            }
            else
            {
                startTime_ = Now();
            }
            running_ = true;
        }
    }

    void Stopwatch::Stop()
    {
        if (running_)
        {
            stopTime_ = Now();
            running_ = false;
        }
    }

    void Stopwatch::Reset()
    {
        startTime_ = Now();
        stopTime_ = startTime_;
    }

    double Stopwatch::GetElapsedTimeSec() const
    {
        return GetElapsedTime<std::chrono::duration<double, std::chrono::seconds::period>>().count();
    }
}