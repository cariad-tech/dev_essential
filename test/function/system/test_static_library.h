/**
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <a_util/system/address_info.h>

class TestStaticLib {
public:
    static a_util::filesystem::Path getFilePathFromStaticLibrary();
};
