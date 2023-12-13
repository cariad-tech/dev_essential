/**
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "test_static_library.h"

a_util::filesystem::Path TestStaticLib::getFilePathFromStaticLibrary()
{
    const a_util::system::AddressInfo address_info(&getFilePathFromStaticLibrary);
    return address_info.getFilePath();
}
