/// @file
/// Common error conditions for multilanguage support - DO NOT INCLUDE DIRECTLY
///
/// @copyright
/// @verbatim
/// Copyright @ 2022 VW Group. All rights reserved.
///
/// This Source Code Form is subject to the terms of the Mozilla
/// Public License, v. 2.0. If a copy of the MPL was not distributed
/// with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
/// @endverbatim

// If new values are added, also add a test in file test/function/base/errc_test.inc
// Exising values MUST NOT BE CHANGED!

/// This scoped enumeration defines the values of portable error conditions for cross-project usage
DEV_ESSENTIAL_ENUM_CLASS errc{
    unknown = -2,            //!< Unknown error
    unexpected = -3,         //!< Unexpected error
    invalid_pointer = -4,    //!< Invalid pointer (e.g. nullptr)
    invalid_argument = -5,   //!< Invalid argument
    invalid_address = -7,    //!< The memory address is invalid
    invalid_file = -11,      //!< The file is invalid
    memory = -12,            //!< No memory left
    timeout = -13,           //!< Operation timed out
    resource_in_use = -15,   //!< A resource is already in use
    not_impl = -16,          //!< A callable is not implemented (e.g. a function)
    no_interface = -17,      //!< The requested interface does not exist
    not_supported = -19,     //!< Operation not supported
    not_found = -20,         //!< Resource (e.g. some chunk of memory) is not found
    canceled = -21,          //!< Operation canceled
    retry = -22,             //!< Retry
    file_not_found = -23,    //!< File not found
    path_not_found = -24,    //!< Requested path not found
    access_denied = -25,     //!< Access denied
    bad_device = -31,        //!< Bad device
    device_io = -32,         //!< I/O error on device
    device_not_ready = -33,  //!< Device not ready
    not_connected = -35,     //!< A connection could not be established
    unknown_format = -36,    //!< File has unknown format
    not_initialized = -37,   //!< Not initialized
    failed = -38,            //!< Failed
    invalid_state = -40,     //!< A resource is in an invalid state
    exception_raised = -41,  //!< An exception was raised
    invalid_type = -42,      //!< A given type is invalid
    empty = -43,             //!< File empty
    out_of_range = -49,      //!< Out of range
    known_problem = -50      //!< Known problem
};
