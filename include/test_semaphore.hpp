#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <list>
#include <random>
#include <std_extension/semaphore.hpp>
#include <thread>

using namespace std::chrono_literals;

class SemaphoreTestSuite : public ::testing::Test {};
class BinarySemaphoreTestSuite : public SemaphoreTestSuite {};
class CountingSemaphoreTestSuite : public SemaphoreTestSuite {};
class FairBinarySemaphoreTestSuite : public SemaphoreTestSuite {};
class FairCountingSemaphoreTestSuite : public SemaphoreTestSuite {};

#define COUNTING_SEMAPHORE_TEST_F(testSuite, countSize, testNum, CountingSemaphore)            \
    TEST_F(testSuite, Test_##countSize##_##testNum) {                                          \
        for (int n = 0; n < 333; n++) {                                                        \
            const std::size_t                 numOfThreads = std::max(100uz, 2uz * countSize); \
            std::list<std::thread>            threads;                                         \
            ext::CountingSemaphore<countSize> sem1(0);                                         \
            ext::CountingSemaphore<1>         sem2(0);                                         \
            std::atomic<std::size_t>          count(0);                                        \
            std::random_device                rd;                                              \
            std::mt19937                      gen(rd());                                       \
            std::uniform_int_distribution<std::size_t> dist1(1);                               \
                                                                                               \
            for (std::size_t i = 0; i < std::max<std::size_t>(countSize * 2, 10); i++) {       \
                sem1.release(dist1(gen));                                                      \
            }                                                                                  \
                                                                                               \
            for (std::size_t i = 0; i < numOfThreads; i++) {                                   \
                threads.emplace_back([&sem1, &sem2, &count] {                                  \
                    sem1.acquire();                                                            \
                    if (countSize == ++count) {                                                \
                        sem2.release();                                                        \
                    }                                                                          \
                });                                                                            \
            }                                                                                  \
                                                                                               \
            sem2.acquire();                                                                    \
            std::this_thread::sleep_for(10ms);                                                 \
            ASSERT_EQ(count, countSize);                                                       \
            std::uniform_int_distribution<std::size_t> dist2(0, numOfThreads - countSize);     \
            for (std::size_t remainingReleases = numOfThreads - countSize, releases = 0;       \
                 remainingReleases != 0; remainingReleases -= releases) {                      \
                releases = std::min(remainingReleases, dist2(gen));                            \
                threads.emplace_back([&sem1, releases] { sem1.release(releases); });           \
            }                                                                                  \
                                                                                               \
            for (auto &thread : threads) {                                                     \
                thread.join();                                                                 \
            }                                                                                  \
            ASSERT_EQ(count, numOfThreads);                                                    \
        }                                                                                      \
    }

COUNTING_SEMAPHORE_TEST_F(BinarySemaphoreTestSuite, 1, 1, counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(CountingSemaphoreTestSuite, 5, 1, counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(CountingSemaphoreTestSuite, 10, 1, counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(CountingSemaphoreTestSuite, 50, 1, counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(CountingSemaphoreTestSuite, 100, 1, counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(FairBinarySemaphoreTestSuite, 1, 1, fair_counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(FairCountingSemaphoreTestSuite, 5, 1, fair_counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(FairCountingSemaphoreTestSuite, 10, 1, fair_counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(FairCountingSemaphoreTestSuite, 50, 1, fair_counting_semaphore)
COUNTING_SEMAPHORE_TEST_F(FairCountingSemaphoreTestSuite, 100, 1, fair_counting_semaphore)
