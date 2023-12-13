# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

if(MSVC)
    set(dev_essential_cmake_cxx_warn_flags "/WX;/W4;/permissive-")
else()
    set(dev_essential_cmake_cxx_warn_flags "-Werror;-Wall;-Wno-unknown-pragmas;-Wextra;-pedantic;$<$<COMPILE_LANGUAGE:CXX>:-Wno-reorder>")
endif()
