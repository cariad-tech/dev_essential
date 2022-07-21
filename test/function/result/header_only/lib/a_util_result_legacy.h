/**
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#ifndef A_UTIL_RESULT_LEGACY_LIBRARY
#define A_UTIL_RESULT_LEGACY_LIBRARY

#include <a_util/result/result_type_decl.h>

_MAKE_RESULT(-1, ERR_FAILED);

class A_UTIL_DLL_EXPORT LegacyResult {
public:
    static a_util::result::Result createWithDefaultCtor();
    static a_util::result::Result createWithErrorCode(std::int32_t error_code);
    static a_util::result::Result createWithSUCCESS();
    static a_util::result::Result createWithBool(bool val);
    static a_util::result::Result createWithERR_FAILED();
    static a_util::result::Result createWithDetailedERR_FAILED(const char* error_description,
                                                               std::int32_t line,
                                                               const char* file,
                                                               const char* function);
    static void testAndDestroyCurrentResult(a_util::result::Result result,
                                            std::int32_t error_code,
                                            const char* label = "(unknown)",
                                            const char* error_description = "",
                                            std::int32_t line = -1,
                                            const char* file = "",
                                            const char* function = "");
};

#endif // A_UTIL_RESULT_LEGACY_LIBRARY
