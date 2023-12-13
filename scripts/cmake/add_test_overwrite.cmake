# Copyright @ 2022 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

option(dev_essential_cmake_enable_add_test_command
       "Enable usage of add_test command in test/ folder. If 'OFF' an error is emitted during\
 configure phase informing the developer to use 'gtest_discover_tests()' instead. - (default: OFF)"
       OFF)

if(NOT dev_essential_cmake_enable_add_test_command)
    macro(add_test)
        if(NOT _dev_essential_cmake_enable_add_test_command)
            message(FATAL_ERROR
                    "'add_test()' is not allowed here, use 'gtest_discover_tests()' instead.\n"
                    "To globally allow 'add_test()' and disable this message, set cmake option "
                    "'dev_essential_cmake_enable_add_test_command' to 'ON'.\nTo allow "
                    "'add_test()' and disable this message in the current scope, set variable "
                    "'_dev_essential_cmake_enable_add_test_command' to 'ON'.")
        else()
            _add_test(${ARGV})
        endif()
    endmacro()
endif()