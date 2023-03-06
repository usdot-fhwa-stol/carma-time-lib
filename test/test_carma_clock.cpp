#include <gtest/gtest.h>

#include "carma_clock.h"

#include <thread>

using namespace fwha_stol::lib::time;
using namespace std::chrono;

constexpr int SYSTEM_SLEEP_TIME = 150;

TEST(test_carma_clock, test_system_time_initialization)
{
    // try a real clock
    CarmaClock clock;
    auto start = system_clock::now();
    clock.wait_for_initialization();
    auto after = system_clock::now();
    auto msCount = duration_cast<milliseconds>(after - start).count();
    // should have happened immediately
    EXPECT_NEAR(0, msCount, 5);
}

TEST(test_carma_clock, test_system_time_sleep_until)
{
    // try a real clock
    CarmaClock clock;
    clock.wait_for_initialization();
    auto start = system_clock::now();
    auto clockNow = clock.nowInMilliseconds();
    // now should match up to the start time we just calculated
    EXPECT_NEAR(clockNow, duration_cast< milliseconds>(start.time_since_epoch()).count(), 1);
    auto futureTimeTp = start + std::chrono::milliseconds(SYSTEM_SLEEP_TIME);
    auto futureTimeMs = duration_cast< milliseconds>(futureTimeTp.time_since_epoch()).count();
    clock.sleep_until(futureTimeMs);
    auto after = system_clock::now();
    auto msCount = duration_cast<milliseconds>(after - start).count();
    // should have slept approximately what we asked
    EXPECT_NEAR(SYSTEM_SLEEP_TIME, msCount, 5);
}

TEST(test_carma_clock, test_system_time_update_exception)
{
    // try a real clock
    CarmaClock clock;
    bool execptionThrown = false;
    try {
        clock.update(0);
    } catch (std::exception & e) {
        // should not be able to update system clock
        execptionThrown = true;
    }
    EXPECT_TRUE(execptionThrown);
}

TEST(test_carma_clock, test_sim_time_initialization)
{
    // try a sim clock
    CarmaClock clock(true);
    std::thread t([&clock]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(SYSTEM_SLEEP_TIME));
        clock.update(1);
    });
    auto start = system_clock::now();
    clock.wait_for_initialization();
    auto after = system_clock::now();
    auto msCount = duration_cast<milliseconds>(after - start).count();
    // should have happened after the update occurred in other thread
    EXPECT_NEAR(SYSTEM_SLEEP_TIME, msCount, 10);
    t.join();
}

TEST(test_carma_clock, test_sim_time_initialization_early)
{
    // try a sim clock
    CarmaClock clock(true);
    clock.update(1);
    std::thread t([&clock]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(SYSTEM_SLEEP_TIME));
    });
    auto start = system_clock::now();
    clock.wait_for_initialization();
    auto after = system_clock::now();
    auto msCount = duration_cast<milliseconds>(after - start).count();
    // should have happened immediately
    EXPECT_NEAR(0, msCount, 5);
    t.join();
}

TEST(test_carma_clock, test_sim_time_not_initialized)
{
    // try a sim clock
    CarmaClock clock(true);
    {
        bool execptionThrown = false;
        try {
            auto timeValue = clock.nowInMilliseconds();
        } catch (std::exception & e) {
            execptionThrown = true;
        }
        // should have thrown exception if no initialize occurred
        EXPECT_TRUE(execptionThrown);
    }
    {
        bool execptionThrown = false;
        try {
            auto timeValue = clock.nowInSeconds();
        } catch (std::exception & e) {
            execptionThrown = true;
        }
        // should have thrown exception if no initialize occurred
        EXPECT_TRUE(execptionThrown);
    }
}

TEST(test_carma_clock, test_sim_time_not_initialized_for_sleep)
{
    // try a sim clock
    CarmaClock clock(true);
    bool execptionThrown = false;
    try {
        clock.sleep_until(1);
    } catch (std::exception & e) {
        execptionThrown = true;
    }
    // should have thrown exception if no initialize occurred
    EXPECT_TRUE(execptionThrown);
}

TEST(test_carma_clock, test_sim_time_sleep_until)
{
    // try a sim clock
    CarmaClock clock(true);
    clock.update(1);
    std::thread t([&clock]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(SYSTEM_SLEEP_TIME));
        clock.update(2);
    });
    clock.wait_for_initialization();
    auto simTimeNow = clock.nowInMilliseconds();
    EXPECT_EQ(simTimeNow, 1);
    auto start = system_clock::now();
    clock.sleep_until(simTimeNow + 1);
    auto after = system_clock::now();
    auto msCount = duration_cast<milliseconds>(after - start).count();
    t.join();
    // should have happened after the update occurred in other thread
    EXPECT_NEAR(SYSTEM_SLEEP_TIME, msCount, 5);
}

TEST(test_carma_clock, test_sim_time_sleep_until_past)
{
    // try a sim clock
    CarmaClock clock(true);
    clock.update(1);
    std::thread t([&clock]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(SYSTEM_SLEEP_TIME));
        clock.update(2);
    });
    clock.wait_for_initialization();
    auto simTimeNow = clock.nowInMilliseconds();
    EXPECT_EQ(simTimeNow, 1);
    auto start = system_clock::now();
    clock.sleep_until(simTimeNow);
    auto after = system_clock::now();
    auto msCount = duration_cast<milliseconds>(after - start).count();
    t.join();
    // should have happened instantly since was not a future time
    EXPECT_NEAR(0, msCount, 5);
}

TEST(test_carma_clock, test_sim_time_sleep_until_multiple)
{
    // try a sim clock
    CarmaClock clock(true);
    clock.update(1);
    std::thread t([&clock]() {
        for (int i = 2; i <=4; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(SYSTEM_SLEEP_TIME));
            clock.update(i);
        }
    });
    clock.wait_for_initialization();
    auto simTimeNow = clock.nowInMilliseconds();
    EXPECT_EQ(simTimeNow, 1);
    auto start = system_clock::now();
    std::thread t1([&clock, simTimeNow]() {
        clock.sleep_until(simTimeNow + 1);
    });
    constexpr int SECOND_SLEEP_DURATION = 3;
    std::thread t2([&clock, simTimeNow]() {
        clock.sleep_until(simTimeNow + SECOND_SLEEP_DURATION);
    });
    constexpr int THIRD_SLEEP_DURATION = 2;
    std::thread t3([&clock, simTimeNow]() {
        clock.sleep_until(simTimeNow + THIRD_SLEEP_DURATION);
    });
    {
        t1.join();
        auto after = system_clock::now();
        auto msCount = duration_cast<milliseconds>(after - start).count();
        // should match up with one sleep period
        EXPECT_NEAR(SYSTEM_SLEEP_TIME, msCount, 5);
    }
    {
        // check t3 first as it should end before t2
        t3.join();
        auto after = system_clock::now();
        auto msCount = duration_cast<milliseconds>(after - start).count();
        // should match up sleep periods for this thread
        EXPECT_NEAR(SYSTEM_SLEEP_TIME * THIRD_SLEEP_DURATION, msCount, 5);
    }
    {
        t2.join();
        auto after = system_clock::now();
        auto msCount = duration_cast<milliseconds>(after - start).count();
        // should match up sleep periods for this thread
        EXPECT_NEAR(SYSTEM_SLEEP_TIME * SECOND_SLEEP_DURATION, msCount, 10);
    }
    t.join();
}

TEST(test_carma_clock, test_time_in_seconds_simulation)
{
    // try a sim clock
    CarmaClock clock(true);
    clock.update(1000);
    auto secondsConunt = clock.nowInSeconds();
    EXPECT_EQ(1, secondsConunt);
    // check rounding as should always go down
    clock.update(1999);
    secondsConunt = clock.nowInSeconds();
    EXPECT_EQ(1, secondsConunt);
}

TEST(test_carma_clock, test_time_in_seconds_realtime)
{
    // try a realclock
    CarmaClock clock;
    auto secondsCount = clock.nowInSeconds();
    auto epochSeconds = duration_cast<seconds>(system_clock::now().time_since_epoch());
    // time should match system time
    EXPECT_EQ(epochSeconds.count(), secondsCount);
}
