#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <list>
#include <random>
#include <std_extention/countdown_latch.hpp>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

class CountdownLatchTestSuite : public ::testing::Test {};

TEST_F(CountdownLatchTestSuite, Test_1) {
    for (std::size_t count = 0; count < 33; count++) {
        for (int rep = 0; rep < 10; rep++) {
            ext::countdown_latch cl(count);

            ASSERT_EQ(count, cl.count());

            std::list<std::thread>                     threads;
            std::atomic<std::size_t>                   countUp(0);
            std::random_device                         rd;
            std::mt19937                               gen(rd());
            std::uniform_int_distribution<std::size_t> sleepingDist(0, 30);

            std::uniform_int_distribution<std::size_t> groupADist(0, 50);
            const std::size_t                          groupA = groupADist(gen);
            for (std::size_t i = 0; i < groupA; i++) {
                threads.emplace_back([sleepFor = sleepingDist(gen), &cl] {
                    std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));
                    cl.wait();
                });
            }

            std::uniform_int_distribution<std::size_t> groupBDist(10, 100);
            const std::size_t                          groupB = groupBDist(gen);
            for (std::size_t i = 0; i < groupB; i++) {
                threads.emplace_back([sleepFor = sleepingDist(gen), count, &cl, &countUp] {
                    std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));
                    for (std::size_t currentCount = countUp.load(); currentCount < count;) {
                        while (!countUp.compare_exchange_weak(currentCount,
                                                              std::min(count, currentCount + 1)));
                        if (currentCount < count) {
                            cl.countdown();
                        }
                    }
                });
            }

            for (auto &&thread : threads) {
                thread.join();
            }

            ASSERT_EQ(count, countUp.load());
            ASSERT_EQ(0, cl.count());
        }
    }
}

TEST_F(CountdownLatchTestSuite, Test_2) {
    const std::size_t groupASize = 50;
    const std::size_t groupBSize = 60;
    for (int rep = 0; rep < 333; rep++) {
        ext::countdown_latch mainCl1(groupASize + groupBSize), mainCl2(groupASize + groupBSize),
            globalCl(1);
        auto workersFunc = [&](std::size_t workerID, std::vector<int> &successes,
                               ext::countdown_latch &thisGroupCl,
                               ext::countdown_latch &otherGroupCl) {
            successes[workerID] =
                workerID % 2 ? thisGroupCl.wait_for(25ms)
                             : thisGroupCl.wait_until(std::chrono::steady_clock::now() + 25ms);
            mainCl1.countdown();
            globalCl.wait();
            otherGroupCl.countdown();
            successes[workerID] =
                workerID % 2 ? thisGroupCl.wait_for(1h)
                             : thisGroupCl.wait_until(std::chrono::steady_clock::now() + 1h);
            mainCl2.countdown();
        };

        std::list<std::thread> threads;

        std::vector<int>     succGA(groupASize, true), succGB(groupBSize, true);
        ext::countdown_latch clGA(groupBSize), clGB(groupASize);

        for (size_t workerID = 0; workerID < groupASize; workerID++) {
            threads.emplace_back([&, workerID]() { workersFunc(workerID, succGA, clGA, clGB); });
        }
        for (size_t workerID = 0; workerID < groupBSize; workerID++) {
            threads.emplace_back([&, workerID]() { workersFunc(workerID, succGB, clGB, clGA); });
        }

        mainCl1.wait();

        for (bool succ : succGA) {
            ASSERT_FALSE(succ);
        }
        for (bool succ : succGB) {
            ASSERT_FALSE(succ);
        }

        globalCl.countdown();
        mainCl2.wait();

        for (bool succ : succGA) {
            ASSERT_TRUE(succ);
        }
        for (bool succ : succGB) {
            ASSERT_TRUE(succ);
        }

        for (std::thread &thread : threads) {
            thread.join();
        }
    }
}
