#include <gtest/gtest.h>
#include <stdads/Stopwatch.h>

#include <thread>

TEST(StopWatchTest, DefaultState)
{
    stdads::Stopwatch sw;

    EXPECT_FALSE(sw.Running());
    EXPECT_DOUBLE_EQ(0.0, sw.GetElapsedTimeSec());
}

TEST(StopWatchTest, StartSetsRunning)
{
    stdads::Stopwatch sw;

    sw.Start();
    EXPECT_TRUE(sw.Running());
}

TEST(StopWatchTest, StopSetsNotRunning)
{
    stdads::Stopwatch sw;

    sw.Start();
    sw.Stop();
    EXPECT_FALSE(sw.Running());
}

TEST(StopWatchTest, StopNotRunningTimerDoesNothing)
{
    stdads::Stopwatch sw;

    sw.Stop();
    EXPECT_FALSE(sw.Running());
    EXPECT_DOUBLE_EQ(0.0, sw.GetElapsedTimeSec());
}

TEST(StopWatchTest, ElapsedTimeIncreasesWhileRunning)
{
    stdads::Stopwatch sw;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    double t1 = sw.GetElapsedTimeSec();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    double t2 = sw.GetElapsedTimeSec();

    EXPECT_GT(t1, 0.0);
    EXPECT_GT(t2, t1); // ensure monotonic

    t1 = sw.GetElapsedTimeSec();
    t2 = sw.GetElapsedTimeSec();
    EXPECT_GE(t2, t1); // ensure monotonic
}

TEST(StopWatchTest, ElapsedTimeStopsAfterStop)
{
    stdads::Stopwatch sw;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sw.Stop();

    double t1 = sw.GetElapsedTimeSec();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    double t2 = sw.GetElapsedTimeSec();

    EXPECT_EQ(t1, t2);
}

TEST(StopWatchTest, ResetClearsTime)
{
    stdads::Stopwatch sw;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sw.Stop();

    sw.Reset();

    EXPECT_EQ(0.0, sw.GetElapsedTimeSec());
}

TEST(StopWatchTest, StartWithResetClearsTime)
{
    stdads::Stopwatch sw;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sw.Stop();

    sw.Start(true);  // reset + start

    EXPECT_TRUE(sw.Running());
    EXPECT_LT(sw.GetElapsedTimeSec(), 0.01);
}

TEST(StopWatchTest, MultipleStartCallsDoNothingIfAlreadyRunning)
{
    stdads::Stopwatch sw;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    sw.Start(true);  // should do nothing

    EXPECT_GT(sw.GetElapsedTimeSec(), 0.0);
    EXPECT_TRUE(sw.Running());

    long long elapsedTimeMs = sw.GetElapsedTime<std::chrono::milliseconds>().count();
    EXPECT_GT(elapsedTimeMs, 0);
}

TEST(StopWatchTest, MeasuresApproximately100ms)
{
    stdads::Stopwatch sw;

    std::chrono::steady_clock::time_point realStart = std::chrono::steady_clock::now();
    sw.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::chrono::steady_clock::time_point realEnd = std::chrono::steady_clock::now();
    sw.Stop();

    // Compare against actual steady_clock start and end time to combat OS scheduling differences
    double expectedElapsedTimeSec = std::chrono::duration<double>(realEnd - realStart).count();
    double elapsedTimeSec = sw.GetElapsedTimeSec();
    EXPECT_NEAR(expectedElapsedTimeSec, elapsedTimeSec, 0.001); // 1ms tolerance

    long long expectedElapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(realEnd - realStart).count();
    long long elapsedTimeMs = sw.GetElapsedTime<std::chrono::milliseconds>().count();
    EXPECT_NEAR(expectedElapsedTimeMs, elapsedTimeMs, 1); // 1ms tolerance
}

TEST(StopWatchTest, CopyConstructorAndAssignmentOperator)
{
    stdads::Stopwatch sw;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sw.Stop();

    // Copy stopped timer
    stdads::Stopwatch copy(sw);
    EXPECT_EQ(sw.Running(), copy.Running());
    EXPECT_EQ(sw.GetElapsedTimeSec(), copy.GetElapsedTimeSec());
    EXPECT_NE(&sw, &copy);

    stdads::Stopwatch assignmentCopy;
    assignmentCopy = sw;
    EXPECT_EQ(sw.Running(), assignmentCopy.Running());
    EXPECT_EQ(sw.GetElapsedTimeSec(), assignmentCopy.GetElapsedTimeSec());
    EXPECT_NE(&sw, &assignmentCopy);


    // Copy running timer
    sw.Start();

    stdads::Stopwatch copy2(sw);
    EXPECT_EQ(sw.Running(), copy2.Running());
    EXPECT_NE(&sw, &copy);

    stdads::Stopwatch assignmentCopy2;
    assignmentCopy2 = sw;
    EXPECT_EQ(sw.Running(), assignmentCopy2.Running());
    EXPECT_NE(&sw, &assignmentCopy2);
}