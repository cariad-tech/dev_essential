# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

if(MSVC)
    #disable RTTI msvc
    add_compile_options(/GR-)
else(MSVC)
    #disable RTTI gcc (only cpp files)
    add_compile_options($<$<AND:$<CXX_COMPILER_ID:GNU>,$<COMPILE_LANGUAGE:CXX>>:-fno-rtti>)
endif(MSVC)
