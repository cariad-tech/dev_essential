/**
 * @file
 * System test implementation
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // _WIN32

#include "test_static_library.h"

#include <a_util/result/result_info.h>
#include <a_util/system/address_info.h>

#include <gtest/gtest.h>

#ifndef _WIN32
#include <dlfcn.h>
#endif // !_WIN32

template <typename T>
#ifdef _WIN32
inline T* lookupSymbol(HMODULE module, const char* symbol_name)
#else
inline T* lookupSymbol(void* module, const char* symbol_name)
#endif
{
#ifdef _WIN32
    return reinterpret_cast<T*>(::GetProcAddress(module, symbol_name));
#else
    return reinterpret_cast<T*>(::dlsym(module, symbol_name));
#endif
}

namespace {

int test_var_global = 5;

int foobar(double val) noexcept
{
    return static_cast<int>(val);
}

struct StaticFunctionTestStruct {
    static const int test_var_static;
    static int foobar(double val)
    {
        return ::foobar(val);
    }
};

const int StaticFunctionTestStruct::test_var_static = 5;

} // namespace

TEST(AddressInfo, getFilePathWorks)
{
    { // Test getFilePath with a free function
        const a_util::system::AddressInfo address_info(foobar);
        const auto result = address_info.getFilePath();
        EXPECT_EQ(result, a_util::filesystem::Path(TEST_EXE_PATH));
    }

    { // Test getFilePath with a static member function
        a_util::system::AddressInfo address_info(StaticFunctionTestStruct::foobar);
        const auto result = address_info.getFilePath();
        EXPECT_EQ(result, a_util::filesystem::Path(TEST_EXE_PATH));
    }

    { // Test getFilePath with a global variable
        const a_util::system::AddressInfo address_info(test_var_global);
        const auto result = address_info.getFilePath();
        EXPECT_EQ(result, a_util::filesystem::Path(TEST_EXE_PATH));
    }

    { // Test getFilePath with a static variable
        a_util::system::AddressInfo address_info(StaticFunctionTestStruct::test_var_static);
        const auto result = address_info.getFilePath();
        EXPECT_EQ(result, a_util::filesystem::Path(TEST_EXE_PATH));
    }
}

TEST(AddressInfo, getFilePathFromStaticLibrary)
{
    /**
     * If getFilePath called in a static lib, the path of executable should be returned.
     */
    EXPECT_EQ(TestStaticLib::getFilePathFromStaticLibrary(),
              a_util::filesystem::Path(TEST_EXE_PATH));
}

TEST(AddressInfo, getFilePathFromSharedLibrary)
{
    /**
     * If getFilePath called in a shared library, the path of shared library should be returned.
     */
#ifdef _WIN32
    const auto library_handle = ::LoadLibrary(SHARED_LIB_PATH);
#else
    const auto library_handle = ::dlopen(SHARED_LIB_PATH, RTLD_LAZY);
#endif // _WIN32
    if (!library_handle) {
        throw std::runtime_error("failed to load shared library '" + std::string(SHARED_LIB_PATH));
    }
    const auto& get_function_get_file_path =
        lookupSymbol<const char*()>(library_handle, "getFilePathFromSharedLibrary");
    const auto shared_lib_path = std::unique_ptr<const char[]>(get_function_get_file_path());
    EXPECT_EQ(a_util::filesystem::Path(SHARED_LIB_PATH),
              a_util::filesystem::Path(shared_lib_path.get()));
}

/// Using stack or heap variables doesn't work ...
TEST(AddressInfo, getFilePathDoesNotWorkForNonStaticVariables)
{
    using a_util::system::AddressInfo;
    {
        const auto stack_test_var = 0;
        const auto address_info = AddressInfo(stack_test_var);
        EXPECT_TRUE(address_info.getFilePath().isEmpty());
    }

    {
        const auto heap_test_var = new int{0};
        const auto heap_test_var_unique_ptr = std::unique_ptr<int>(heap_test_var);
        const auto address_info = AddressInfo(heap_test_var);
        EXPECT_TRUE(address_info.getFilePath().isEmpty());
    }

    {
        const auto address_info = AddressInfo(this);
        EXPECT_TRUE(address_info.getFilePath().isEmpty());
    }
}