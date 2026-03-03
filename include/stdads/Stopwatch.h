#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

namespace stdads {

    class Stopwatch {
    public:

        /**
         * @brief Default constructor.
         *
         * Creates a Stopwatch that is not running with no elapsed time.
         */
        Stopwatch();

        /**
         * @brief Copy constructor.
         *
         * @param other Stopwatch to copy.
         */
        Stopwatch(const Stopwatch& other);

        /**
         * @brief Destructor.
         */
        ~Stopwatch();

        /**
         * @brief Assignment operator.
         *
         * @param other Stopwatch to assign this to.
         * @return Reference to this object
         */
        Stopwatch& operator=(const Stopwatch& other);

        /**
         * @brief Starts the Stopwatch.
         * 
         * If the Stopwatch is already running, this function does nothing.
         *
         * @param reset True to also reset the time, otherwise (default) false to not reset.
         */
        void Start(bool reset = false);

        /**
         * @brief Stops the Stopwatch.
         */
        void Stop();

        /**
         * @brief Resets the Stopwatch.
         */
        void Reset();

        /**
         * @brief Get the elapsed time.
         *
         * @return Elapsed time in the desired units as a duration
         */
        template<typename Duration>
        Duration GetElapsedTime() const;

        /**
         * @brief Get the elapsed time in seconds.
         *
         * @return Elapsed time in seconds
         */
        double GetElapsedTimeSec() const;

        /**
         * @brief Get if the Stopwatch is running.
         *
         * @return True if the Stopwatch is running, false otherwise.
         */
        bool Running() const { return running_; }

    private:

        /**
         * @brief Helper function to get the current steady time.
         *
         * @return time_point with the current steady time using chrono
         */
        static std::chrono::steady_clock::time_point Now() { return std::chrono::steady_clock::now(); }

        std::chrono::steady_clock::time_point startTime_;
        std::chrono::steady_clock::time_point stopTime_;
        bool running_;
    };

    template<typename Duration>
    Duration Stopwatch::GetElapsedTime() const
    {
        std::chrono::steady_clock::time_point latestTime = running_ ? Now() : stopTime_;
        return std::chrono::duration_cast<Duration>(latestTime - startTime_);
    }
}

#endif