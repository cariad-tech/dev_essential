<!---
Copyright @ 2021 VW Group. All rights reserved.

     This Source Code Form is subject to the terms of the Mozilla
     Public License, v. 2.0. If a copy of the MPL was not distributed
     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
-->

# Utility library

## Delivered components <a name="delivered-components"></a>
This library provides the following components:

- **a_util** itself serves as root component and provides common functionality for all subcomponents
- **a_util::chrono** for code compatibility to [std::chrono](https://en.cppreference.com/w/cpp/chrono)
- **a_util::concurrency** for concurrency handling, mainly but not exclusively for code compatibility
  to [standard thread support](https://en.cppreference.com/w/cpp/thread)
- **a_util::datetime** for date and time handling
- **a_util::experimental** for features which are considered experimental
- **a_util::filesystem** for path and filesystem functionality
- **a_util::logging** for basic logging functionality
- **a_util::memory** for memory access and management
- **a_util::parser** for common parsing functionality
- **a_util::preprocessor** for common preprocessor functionality
- **a_util::process** for interaction with operating system processes
- **a_util::regex** for regular expression functionality
- **a_util::result** for functionality handling error and return types
- **a_util::strings** for string handling and conversion functionality
- **a_util::system** for portable OS (Windows, Linux, ...) functionality
- **a_util::variant** for C++17 [std::variant](https://en.cppreference.com/w/cpp/utility/variant)
  like functionality
- **a_util::xml** for handling XML
