#pragma once

#include <stdext/exception.hpp>
#include <gtest/gtest.h>
#include <iostream>

int fib(int n) {
    if (0 == n || 1 == n) {
        throw stdext::exception("fib throws");
    }

    return fib(n - 1) + fib(n - 2);
}

void funcThatTrows() {
    (void)fib(4);
}

void nothrow() noexcept {
    try {
        funcThatTrows();
    } catch (const stdext::exception &e) {
        std::cout << e << std::endl;
    }
}

class ExceptionTestSuite : public ::testing::Test {};

TEST_F(ExceptionTestSuite, Test) {
    ASSERT_THROW(funcThatTrows(), stdext::exception);
    ASSERT_NO_THROW(nothrow());
}
