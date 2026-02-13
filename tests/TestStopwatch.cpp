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
}

TEST(StopWatchTest, MeasuresApproximately100ms)
{
    stdads::Stopwatch sw;

    sw.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sw.Stop();

    double elapsedTimeSec = sw.GetElapsedTimeSec();
    long long elapsedTimeMs = sw.GetElapsedTime<std::chrono::milliseconds>().count();
    EXPECT_NEAR(elapsedTimeSec, 0.1, 0.02); // 20ms tolerance
    EXPECT_NEAR(elapsedTimeMs, 100, 20); // 20ms tolerance
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