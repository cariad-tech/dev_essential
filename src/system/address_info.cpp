/**
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

#include <a_util/system/address_info.h>

#include <memory>
#include <type_traits>

#ifndef _WIN32
#include <dlfcn.h>
#endif // !_WIN32

namespace a_util {
namespace system {

filesystem::Path AddressInfo::getFilePath() const
{
#ifdef _WIN32
    HMODULE hModule{};
    auto address = static_cast<LPCTSTR>(_address);
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, address, &hModule)) {
        return {};
    }

    // GetModuleHandleEx increments the reference counter, FreeLibrary decrements it
    const auto decrement_ref_count = [](const HMODULE* module) { ::FreeLibrary(*module); };
    const auto module_handle =
        std::unique_ptr<HMODULE, decltype(decrement_ref_count)>(&hModule, decrement_ref_count);
    WCHAR buffer[MAX_PATH] = {};
    const auto module_result = GetModuleFileNameW(hModule, buffer, ARRAYSIZE(buffer));
    if ((module_result >= ARRAYSIZE(buffer)) || (module_result == 0)) {
        // String truncation or some other error.
        return {};
    }

    TCHAR utf8[MAX_PATH] = {};
    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, utf8, ARRAYSIZE(utf8), {}, {});
    return utf8;
#else
    Dl_info dl_info{};
#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
    dladdr(const_cast<handle_t>(_address), &dl_info);
#else
    dladdr(_address, &dl_info);
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
    return dl_info.dli_fname;
#endif // _WIN32
}

} // namespace system
} // namespace a_util
