#include <gtest/gtest.h>

#include "carma_clock_example.h"

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
    bool execptionThrown = false;
    try {
        auto timeValue = clock.nowInMilliseconds();
    } catch (std::exception & e) {
        execptionThrown = true;
    }
    // should have thrown exception if no initialize occurred
    EXPECT_TRUE(execptionThrown);
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
