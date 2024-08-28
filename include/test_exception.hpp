#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include <std_extension/exception.hpp>

int fib(int n) {
    if (0 == n || 1 == n) {
        throw ext::exception("fib throws");
    }

    return fib(n - 1) + fib(n - 2);
}

void funcThatTrows() { (void)fib(4); }

void nothrow() noexcept {
    try {
        funcThatTrows();
    } catch (const ext::exception &e) {
        std::cout << e << std::endl;
    }
}

class ExceptionTestSuite : public ::testing::Test {};

TEST_F(ExceptionTestSuite, Test) {
    ASSERT_THROW(funcThatTrows(), ext::exception);
    ASSERT_NO_THROW(nothrow());
}
