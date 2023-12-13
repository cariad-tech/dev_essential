# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

set(_msvc_compiler "$<OR:$<CXX_COMPILER_ID:MSVC>,$<C_COMPILER_ID:MSVC>>")
add_compile_options($<${_msvc_compiler}:/MP>)
unset(_msvc_compiler)