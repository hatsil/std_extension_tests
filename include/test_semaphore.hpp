#pragma once

#include <stdext/semaphore.hpp>
#include <gtest/gtest.h>
#include <thread>
#include <list>
#include <atomic>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std::chrono_literals;

class SemaphoreTestSuite : public ::testing::Test {};
class BinarySemaphoreTestSuite : public SemaphoreTestSuite {};
class CountingSemaphoreTestSuite : public SemaphoreTestSuite {};

TEST_F(BinarySemaphoreTestSuite, Test1) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        for (int i = 0; i < 10; i++) {
            sem.release();
        }
        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        sem.release(numOfThreads - 1);
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test2) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        for (int i = 0; i < 10; i++) {
            sem.release();
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        for (int i = 1; i < numOfThreads; i++) {
            threads.emplace_back([&sem] {
                sem.release();
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}


TEST_F(BinarySemaphoreTestSuite, Test3) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        for (int i = 0; i < 10; i++) {
            sem.release();
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        for (int i = 2; i < numOfThreads; i += 2) {
            threads.emplace_back([&sem] {
                sem.release(2);
            });
        }
        sem.release();
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test4) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        for (int i = 0; i < 10; i++) {
            sem.release();
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        for (int i = 4; i < numOfThreads; i += 4) {
            threads.emplace_back([&sem] {
                sem.release(4);
            });
        }
        sem.release(3);
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test5) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        for (int i = 0; i < 10; i++) {
            sem.release();
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        std::uniform_int_distribution<> distrib1(0, numOfThreads - 1);
        for (int remainingReleases = numOfThreads - 1, releases = 0; remainingReleases > 0; remainingReleases -= releases) {
            releases = std::min(remainingReleases, distrib1(gen));
            threads.emplace_back([&sem, releases] {
                sem.release(releases);
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test6) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(1);

        for (int i = 0; i < 10; i++) {
            sem.release(distrib(gen));
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        sem.release(numOfThreads - 1);
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test7) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(1);

        for (int i = 0; i < 10; i++) {
            sem.release(distrib(gen));
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        for (int i = 1; i < numOfThreads; i++) {
            threads.emplace_back([&sem] {
                sem.release();
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test8) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(1);

        for (int i = 0; i < 10; i++) {
            sem.release(distrib(gen));
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        for (int i = 2; i < numOfThreads; i += 2) {
            threads.emplace_back([&sem] {
                sem.release(2);
            });
        }
        sem.release();
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test9) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(1);

        for (int i = 0; i < 10; i++) {
            sem.release(distrib(gen));
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        for (int i = 4; i < numOfThreads; i += 4) {
            threads.emplace_back([&sem] {
                sem.release(4);
            });
        }
        sem.release(3);
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(BinarySemaphoreTestSuite, Test10) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::binary_semaphore sem(0);
        std::atomic_int count(0);
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(1);

        for (int i = 0; i < 10; i++) {
            sem.release(distrib(gen));
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 1);
        std::uniform_int_distribution<> distrib1(0, numOfThreads - 1);
        for (int remainingReleases = numOfThreads - 1, releases = 0; remainingReleases > 0; remainingReleases -= releases) {
            releases = std::min(remainingReleases, distrib1(gen));
            threads.emplace_back([&sem, releases] {
                sem.release(releases);
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}

TEST_F(CountingSemaphoreTestSuite, Test1) {
    for (int n = 0; n < 20; n++) {
        std::list<std::thread> threads;
        stdext::counting_semaphore<5> sem(0);
        std::atomic_int count(0);
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(1);

        for (int i = 0; i < 10; i++) {
            sem.release(distrib(gen));
        }

        const int numOfThreads = 100;
        for (int i = 0; i < numOfThreads; i++) {
            threads.emplace_back([&sem, &count] {
                sem.acquire();
                ++count;
            });
        }

        std::this_thread::sleep_for(250ms);
        ASSERT_EQ(count, 5);
        std::uniform_int_distribution<> distrib1(0, numOfThreads - 1);
        for (int remainingReleases = numOfThreads - 1, releases = 0; remainingReleases > 0; remainingReleases -= releases) {
            releases = std::min(remainingReleases, distrib1(gen));
            threads.emplace_back([&sem, releases] {
                sem.release(releases);
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }
        ASSERT_EQ(count, numOfThreads);
    }
}
