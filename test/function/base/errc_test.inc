// Common error code and condition implementation test for C/C++
//
// Copyright @ 2022 VW Group. All rights reserved.
//
//     This Source Code Form is subject to the terms of the Mozilla
//     Public License, v. 2.0. If a copy of the MPL was not distributed
//     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
//
// If it is not possible or desirable to put the notice in a particular file, then
// You may include the notice in a location (such as a LICENSE file in a
// relevant directory) where a recipient would be likely to look for such a notice.
//
// You may add additional accurate notices of copyright ownership.

#ifndef STATIC_ASSERT_EQ
#error STATIC_ASSERT_EQ(expected_val, errc_enum_val) must be defined
#endif // !STATIC_ASSERT_EQ

STATIC_ASSERT_EQ(-2, unknown);
STATIC_ASSERT_EQ(-3, unexpected);
STATIC_ASSERT_EQ(-4, invalid_pointer);
STATIC_ASSERT_EQ(-5, invalid_argument);
STATIC_ASSERT_EQ(-7, invalid_address);
STATIC_ASSERT_EQ(-11, invalid_file);
STATIC_ASSERT_EQ(-12, memory);
STATIC_ASSERT_EQ(-13, timeout);
STATIC_ASSERT_EQ(-15, resource_in_use);
STATIC_ASSERT_EQ(-16, not_impl);
STATIC_ASSERT_EQ(-17, no_interface);
STATIC_ASSERT_EQ(-19, not_supported);
STATIC_ASSERT_EQ(-20, not_found);
STATIC_ASSERT_EQ(-21, canceled);
STATIC_ASSERT_EQ(-22, retry);
STATIC_ASSERT_EQ(-23, file_not_found);
STATIC_ASSERT_EQ(-24, path_not_found);
STATIC_ASSERT_EQ(-25, access_denied);
STATIC_ASSERT_EQ(-31, bad_device);
STATIC_ASSERT_EQ(-32, device_io);
STATIC_ASSERT_EQ(-33, device_not_ready);
STATIC_ASSERT_EQ(-35, not_connected);
STATIC_ASSERT_EQ(-36, unknown_format);
STATIC_ASSERT_EQ(-37, not_initialized);
STATIC_ASSERT_EQ(-38, failed);
STATIC_ASSERT_EQ(-40, invalid_state);
STATIC_ASSERT_EQ(-41, exception_raised);
STATIC_ASSERT_EQ(-42, invalid_type);
STATIC_ASSERT_EQ(-43, empty);
STATIC_ASSERT_EQ(-49, out_of_range);
STATIC_ASSERT_EQ(-50, known_problem);
