/**
 * @file
 * a_util::memory::make_unique test implementation
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#define A_UTIL_ENABLE_MAKE_UNIQUE
#include <a_util/memory/unique_ptr.h>

#include <gtest/gtest.h>

#include <string>
#include <type_traits>

namespace {

class MakeUniqueTestBase {
public:
    MakeUniqueTestBase() = default;
    virtual ~MakeUniqueTestBase() = default;

    MakeUniqueTestBase(const MakeUniqueTestBase&) = delete;
    MakeUniqueTestBase& operator=(const MakeUniqueTestBase&) = delete;
    MakeUniqueTestBase(MakeUniqueTestBase&&) = delete;
    MakeUniqueTestBase& operator=(MakeUniqueTestBase&&) = delete;

    virtual const char* testMsg() const noexcept
    {
        return "";
    }
};

class MakeUniqueTest : public MakeUniqueTestBase {
public:
    MakeUniqueTest() = default;
    MakeUniqueTest(const std::string& msg) : test_message(msg)
    {
    }

    const char* testMsg() const noexcept
    {
        return test_message.c_str();
    }

private:
    const std::string test_message = {"unknown"};
};

/**
 * @brief Test backport of make_unique for pre C++14 compilers
 * @req_id ODAUTIL-175
 */
TEST(memory_test, make_unique)
{
    static_assert(std::is_same<decltype(a_util::memory::make_unique<MakeUniqueTest>()),
                               std::unique_ptr<MakeUniqueTest>>::value,
                  "a_util::result::make_unique(Args&&...) creates wrong type!");
    static_assert(std::is_same<decltype(a_util::memory::make_unique<MakeUniqueTest[]>(5)),
                               std::unique_ptr<MakeUniqueTest[]>>::value,
                  "a_util::result::make_unique(size_t) creates wrong type!");

    // move construction
    std::unique_ptr<MakeUniqueTestBase> non_array_type_default_constructed(
        a_util::memory::make_unique<MakeUniqueTest>());
    EXPECT_STREQ("unknown", non_array_type_default_constructed->testMsg());

    std::unique_ptr<MakeUniqueTestBase> non_array_type(
        a_util::memory::make_unique<MakeUniqueTest>("Hello World"));
    EXPECT_STREQ("Hello World", non_array_type->testMsg());

    std::unique_ptr<MakeUniqueTest[]> array_type(a_util::memory::make_unique<MakeUniqueTest[]>(3));
    std::for_each(array_type.get(), array_type.get() + 3, [](const MakeUniqueTest& object) {
        EXPECT_STREQ("unknown", object.testMsg());
    });

    // move assignment
    non_array_type_default_constructed = a_util::memory::make_unique<MakeUniqueTest>();
    EXPECT_STREQ("unknown", non_array_type_default_constructed->testMsg());

    non_array_type = a_util::memory::make_unique<MakeUniqueTest>("Hello World, I'm reborn");
    EXPECT_STREQ("Hello World, I'm reborn", non_array_type->testMsg());

    array_type = a_util::memory::make_unique<MakeUniqueTest[]>(5);
    std::for_each(array_type.get(), array_type.get() + 5, [](const MakeUniqueTest& object) {
        EXPECT_STREQ("unknown", object.testMsg());
    });

    // disallowed by the standard, doesn't compile
    // const auto array_type_with_known_bound1 = make_unique("Hello World");
    // const auto array_type_with_known_bound2 = make_unique<MakeUniqueTest[]>("Foo", "Bar");
    // const auto array_type_with_known_bound3 = make_unique<MakeUniqueTest[3]>();
    // const auto array_type_with_known_bound4 = make_unique<MakeUniqueTest[3]>("Foo", "Bar", "Bo");
}

} // namespace
