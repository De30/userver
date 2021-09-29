#pragma once

/// @file userver/utest/utest.hpp
/// @brief Versions of gtest macros that run tests in a coroutine environment

#include <chrono>
#include <functional>
#include <utility>

#include <gtest/gtest.h>

#include <userver/engine/run_in_coro.hpp>  // legacy
#include <userver/utest/test_case_macros.hpp>
#include <userver/utils/assert.hpp>
#include <userver/utils/invariant_error.hpp>
#include <userver/utils/strong_typedef.hpp>

inline constexpr std::chrono::seconds kMaxTestWaitTime(20);

// gtest-specific serializers
namespace testing {

void PrintTo(std::chrono::seconds s, std::ostream* os);
void PrintTo(std::chrono::milliseconds ms, std::ostream* os);
void PrintTo(std::chrono::microseconds us, std::ostream* os);
void PrintTo(std::chrono::nanoseconds ns, std::ostream* os);

}  // namespace testing

namespace formats::json {

class Value;

void PrintTo(const Value&, std::ostream*);

}  // namespace formats::json

namespace utils {

template <class Tag, class T, StrongTypedefOps Ops>
void PrintTo(const StrongTypedef<Tag, T, Ops>& v, std::ostream* os) {
  ::testing::internal::UniversalTersePrint(v.GetUnderlying(), os);
}

}  // namespace utils

namespace decimal64 {

template <int Prec, typename RoundPolicy>
class Decimal;

template <int Prec, typename RoundPolicy>
void PrintTo(const Decimal<Prec, RoundPolicy>& v, std::ostream* os) {
  *os << v;
}

}  // namespace decimal64

#ifdef __APPLE__
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DISABLED_IN_MAC_OS_TEST_NAME(name) DISABLED_##name
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DISABLED_IN_MAC_OS_TEST_NAME(name) name
#endif

#ifdef _LIBCPP_VERSION
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DISABLED_IN_LIBCPP_TEST_NAME(name) DISABLED_##name
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DISABLED_IN_LIBCPP_TEST_NAME(name) name
#endif

/// @{
/// Test that a UINVARIANT check triggers
///
/// @hideinitializer
#ifdef NDEBUG
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define EXPECT_UINVARIANT_FAILURE(statement) \
  EXPECT_THROW(statement, ::utils::InvariantError)
#else
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define EXPECT_UINVARIANT_FAILURE(statement) EXPECT_DEATH(statement, "")
#endif
/// @}

/// @{
/// @brief Versions of gtest macros that run tests in a coroutine environment
///
/// There are the following extensions:
///
/// 1. `_MT` ("multi-threaded") macro versions take 'thread_count' integer
///    as the 3rd parameter at the test definition. It specifies the number of
///    worker threads that should be created for the test. By default,
///    there is only 1 worker thread, which should be enough for most tests;
/// 2. `GetThreadCount()` method is available in the test scope.
///
/// ## Usage examples:
/// @snippet core/src/engine/semaphore_test.cpp  UTEST macro example 1
/// @snippet core/src/engine/semaphore_test.cpp  UTEST macro example 2
///
/// @hideinitializer
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define UTEST(test_suite_name, test_name) \
  IMPL_UTEST_TEST(test_suite_name, test_name, 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define UTEST_MT(test_suite_name, test_name, thread_count) \
  IMPL_UTEST_TEST(test_suite_name, test_name, thread_count)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define UTEST_F(test_suite_name, test_name) \
  IMPL_UTEST_TEST_F(test_suite_name, test_name, 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define UTEST_F_MT(test_suite_name, test_name, thread_count) \
  IMPL_UTEST_TEST_F(test_suite_name, test_name, thread_count)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define UTEST_P(test_suite_name, test_name) \
  IMPL_UTEST_TEST_P(test_suite_name, test_name, 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define UTEST_P_MT(test_suite_name, test_name, thread_count) \
  IMPL_UTEST_TEST_P(test_suite_name, test_name, thread_count)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TYPED_UTEST(test_suite_name, test_name) \
  IMPL_UTEST_TYPED_TEST(test_suite_name, test_name, 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TYPED_UTEST_MT(test_suite_name, test_name, thread_count) \
  IMPL_UTEST_TYPED_TEST(test_suite_name, test_name, thread_count)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TYPED_UTEST_P(test_suite_name, test_name) \
  IMPL_UTEST_TYPED_TEST_P(test_suite_name, test_name, 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TYPED_UTEST_P_MT(test_suite_name, test_name, thread_count) \
  IMPL_UTEST_TYPED_TEST_P(test_suite_name, test_name, thread_count)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TYPED_UTEST_SUITE(test_suite_name, types)                        \
  namespace IMPL_UTEST_NAMESPACE_NAME(test_suite_name) {                 \
    IMPL_UTEST_HIDE_USER_FIXTURE_BY_TEST_LAUNCHER_TYPED(test_suite_name) \
    TYPED_TEST_SUITE(test_suite_name, types,                             \
                     ::utest::impl::DefaultNameGenerator);               \
  }                                                                      \
  struct UtestImplForceSemicolon

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define INSTANTIATE_UTEST_SUITE_P(prefix, test_suite_name, ...)             \
  namespace IMPL_UTEST_NAMESPACE_NAME(test_suite_name) {                    \
    IMPL_UTEST_HIDE_USER_FIXTURE_BY_TEST_LAUNCHER(                          \
        test_suite_name,                                                    \
        ::utest::impl::TestLauncherParametric<IMPL_UTEST_NON_PARENTHESIZED( \
            test_suite_name)::ParamType>)                                   \
    INSTANTIATE_TEST_SUITE_P(prefix, test_suite_name, __VA_ARGS__);         \
  }                                                                         \
  struct UtestImplForceSemicolon

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define REGISTER_TYPED_UTEST_SUITE_P(test_suite_name, ...)               \
  namespace IMPL_UTEST_NAMESPACE_NAME(test_suite_name) {                 \
    IMPL_UTEST_HIDE_USER_FIXTURE_BY_TEST_LAUNCHER_TYPED(test_suite_name) \
    REGISTER_TYPED_TEST_SUITE_P(test_suite_name, __VA_ARGS__);           \
  }                                                                      \
  struct UtestImplForceSemicolon

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define INSTANTIATE_TYPED_UTEST_SUITE_P(prefix, test_suite_name, types)  \
  namespace IMPL_UTEST_NAMESPACE_NAME(test_suite_name) {                 \
    IMPL_UTEST_HIDE_USER_FIXTURE_BY_TEST_LAUNCHER_TYPED(test_suite_name) \
    INSTANTIATE_TYPED_TEST_SUITE_P(prefix, test_suite_name, types,       \
                                   ::utest::impl::DefaultNameGenerator); \
  }                                                                      \
  struct UtestImplForceSemicolon

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TYPED_UTEST_SUITE_P(test_suite_name)                             \
  namespace IMPL_UTEST_NAMESPACE_NAME(test_suite_name) {                 \
    IMPL_UTEST_HIDE_USER_FIXTURE_BY_TEST_LAUNCHER_TYPED(test_suite_name) \
    TYPED_TEST_SUITE_P(test_suite_name);                                 \
  }                                                                      \
  struct UtestImplForceSemicolon
/// @}