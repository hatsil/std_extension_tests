#pragma once

#if defined(ALL_TESTS) || defined(EXCEPTION_TESTS)
#include "test_exception.hpp"
#endif

#if defined(ALL_TESTS) || defined(SEMAPHORE_TESTS)
#include "test_semaphore.hpp"
#endif

#if defined(ALL_TESTS) || defined(COUNTDOWN_LATCH_TESTS)
#include "test_countdown_latch.hpp"
#endif
