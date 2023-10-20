/**
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/preprocessor/dll_export.h>
#include <a_util/strings/strings_functions.h>
#include <a_util/system/address_info.h>

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif
A_UTIL_DLL_EXPORT const char* getFilePathFromSharedLibrary()
{
    const a_util::system::AddressInfo address_info(&getFilePathFromSharedLibrary);
    const auto path_str = static_cast<std::string>(address_info.getFilePath());
    auto file_path_ptr = std::make_unique<char[]>(path_str.size() + 1);
    a_util::strings::copy(
        file_path_ptr.get(), path_str.size() + 1, path_str.c_str(), path_str.size());
    return file_path_ptr.release();
}
#ifdef __cplusplus
}
#endif
