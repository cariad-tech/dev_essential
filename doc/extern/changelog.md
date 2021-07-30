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

# Dev essential changelog

All notable changes to project [Dev essential libraries][] will be documented here.
For upcoming features and known issues see the bottom of this file.

The format is based on [Keep a Changelog][] and this project adheres to [Semantic Versioning][].

## Version history
### [Dev essentials libraries](#dev_essential_libraries)
* [1.0.0](#dev_essential_1_0_0) \| [Changes](#dev_essential_1_0_0_changes) \|
  [Fixes](#dev_essential_1_0_0_fixes) \| [Known issues](#dev_essential_1_0_0_known_issues) \|
  Release date: 2021/08/04
* [Upcoming](#upcoming)

### [Utility library](#utility_library)
* [5.8.0](#a_util_5_8_0) \| [Changes](#a_util_5_8_0_changes) \| [Fixes](#a_util_5_8_0_fixes) \|
  [Known issues](#a_util_5_8_0_known_issues) \| Release date: 2021/02/17
* [5.7.0](#a_util_5_7_0) \| [Changes](#a_util_5_7_0_changes) \| Fixes \|
  [Known issues](#a_util_5_7_0_known_issues) \| Release date: 2020/05/20
* [5.6.1](#a_util_5_6_1) \| [Changes](#a_util_5_6_1_changes) \| [Fixes](#a_util_5_6_1_fixes) \|
  [Known issues](#a_util_5_6_1_known_issues) \| Release date: 2020/02/12
* [5.6.0](#a_util_5_6_0) \| [Changes](#a_util_5_6_0_changes) \| [Fixes](#a_util_5_6_0_fixes) \|
  [Known issues](#a_util_5_6_0_known_issues) \| Release date: 2019/09/06
* [5.5.0](#a_util_5_5_0) \| [Changes](#a_util_5_5_0_changes) \| [Fixes](#a_util_5_5_0_fixes) \|
  [Known issues](#a_util_5_5_0_known_issues) \| Release date: 2019/04/12
* [5.4.0](#a_util_5_4_0) \| [Changes](#a_util_5_4_0_changes) \| Fixes \|
  [Known issues](#a_util_5_4_0_known_issues) \| Release date: 2018/11/07
* [5.3.0](#a_util_5_3_0) \| [Changes](#a_util_5_3_0_changes) \| [Fixes](#a_util_5_3_0_fixes) \|
  [Known issues](#a_util_5_3_0_known_issues) \| Release date: 2018/09/21
* [5.2.0](#a_util_5_2_0) \| [Changes](#a_util_5_2_0_changes) \| [Fixes](#a_util_5_2_0_fixes) \|
  [Known issues](#a_util_5_2_0_known_issues) \| Release date: 2018/03/30
* [5.1.0](#a_util_5_1_0) \| [Changes](#a_util_5_1_0_changes) \| [Fixes](#a_util_5_1_0_fixes) \|
  [Known issues](#a_util_5_1_0_known_issues) \| Release date: 2017/12/08
* [5.0.0](#a_util_5_0_0) \| [Changes](#a_util_5_0_0_changes) \| [Fixes](#a_util_5_0_0_fixes) \|
  [Known issues](#a_util_5_0_0_known_issues) \| Release date: 2017/09/01

### [DDL library](#ddl_library)
* [4.6.0](#ddl_4_6_0) \| [Changes](#ddl_4_6_0_changes) \| [Fixes](#ddl_4_6_0_fixes) \|
  [Known issues](#ddl_4_6_0_known_issues) \| Release date: 2021/02/22
* [4.5.0](#ddl_4_5_0) \| [Changes](#ddl_4_5_0_changes) \| [Fixes](#ddl_4_5_0_fixes) \|
  [Known issues](#ddl_4_5_0_known_issues) \| Release date: 2020/05/25
* [4.4.1](#ddl_4_4_1) \| [Changes](#ddl_4_4_1_changes) \| [Fixes](#ddl_4_4_1_fixes) \|
  [Known issues](#ddl_4_4_1_known_issues) \| Release date: 2020/02/12
* [4.4.0](#ddl_4_4_0) \| [Changes](#ddl_4_4_0_changes) \| [Fixes](#ddl_4_4_0_fixes) \|
  [Known issues](#ddl_4_4_0_known_issues) \| Release date: 2019/09/06
* [4.3.0](#ddl_4_3_0) \| [Changes](#ddl_4_3_0_changes) \| [Fixes](#ddl_4_3_0_fixes) \|
  [Known issues](#ddl_4_3_0_known_issues) \| Release date: 2019/06/14
* [4.2.1](#ddl_4_2_1) \| Changes \| [Fixes](#ddl_4_2_1_fixes) \|
  [Known issues](#ddl_4_2_1_known_issues) \| Release date: 2019/03/19
* [4.2.0](#ddl_4_2_0) \| [Changes](#ddl_4_2_0_changes) \| [Fixes](#ddl_4_2_0_fixes) \|
  [Known issues](#ddl_4_2_0_known_issues) \| Release date: 2018/11/08
* [4.1.0](#ddl_4_1_0) \| [Changes](#ddl_4_1_0_changes) \| [Fixes](#ddl_4_1_0_fixes) \|
  [Known issues](#ddl_4_1_0_known_issues) \| Release date: 2018/06/30
* [4.0.0](#ddl_4_0_0) \| [Changes](#ddl_4_0_0_changes) \| [Fixes](#ddl_4_0_0_fixes) \|
  [Known issues](#ddl_4_0_0_known_issues) \| Release date: 2017/12/06


<a name="dev_essential_libraries"></a>
## Dev essential libraries

<a name="dev_essential_1_0_0"></a>
### [dev_essential 1.0.0][] - 2021/08/04
> _**This release is the successor of a_util, ddl library and pkg_rpc, merging all three libraries
> into one repository.**_

<a name="dev_essential_1_0_0_changes"></a>
#### Changes
- [ODAUTIL-79] - [Result] Enable usage of error description objects with static storage
- [ODAUTIL-270] - Enable position independent code and position independent executables
- [ODAUTIL-301] - [build] Elevate compiler warning levels for all util components
- [ODAUTIL-302] - [build] Fix all compiler warnings and set compiler warnings as errors
- [ODAUTIL-313] - Integrate cdpkgrpc master and tags into a_util repository
- [ODAUTIL-314] - Integrate cdddl master and tags into a_util repository
- [ODAUTIL-385] - Move or copy DDL description generation code from ADTF 3 to DDL Library
- [ODAUTIL-405] - Add type reflection API to the DDL Library
- [ODAUTIL-423] - Improve XML Data Model and DDL Parser
- [ODAUTIL-431] - Redesign OO DDL Description API (Data Model + API)
  - **Breaking change:** The deprecated `ddl::DDLDescription` API has been replaced with the
    [DDL DataDefinition](ddl/dd.md). Customer code using the deprecated `ddl::DDLDescription` needs
    to be rewritten accordingly.
- [ODAUTIL-451] - Integrate cdpkgrpc into top level directory of a_util
- [ODAUTIL-459] - Provide ddl, pkg_rpc and a_util as dev_essential package version 1.0.0
- [ODAUTIL-461] - Integrate cdddl into top level directory of a_util
- [ODAUTIL-462] - Update license disclaimers to MPL2.0 and VW Group copyright
- [ODAUTIL-463] - [BuildSystem] Add Linux_x64_gcc7, Linux_armv8_gcc7, and Windows_x64_vc142_VS2019 profile
- [ODAUTIL-468] - [BuildSystem] Provide Release builds for additional platforms
- [ODAUTIL-471] - [BuildSystem] Provide updated build information in README.md
- [ODAUTIL-472] - Remove ddl2header and header2ddl
  - The successors of those tools will be available as part of the DDL Utilities. See
    [here] and
    [here] for details.

 <a name="dev_essential_1_0_0_fixes"></a>
#### Fixes
- [ODAUTIL-454] - [filesystem] `a_util::filesytem::Path::makeRelative()` fails if any path contains ".."
- [ODAUTIL-464] - remove / access for DDL internal datastructures type_access_list, type_access_map are not working
  correctly
- [ODAUTIL-467] - If removing a struct_type which is used in another type the validation model will crash
- [ODAUTIL-470] - Mapping Configuration for invalid file will crash
- [ODAUTIL-473] - Codec: serialized and deserialized offsets of structures in array elements are not evaluated right

<a name="dev_essential_1_0_0_known_issues"></a>
#### [Known issues]

<a name="utility_library"></a>
## Utility library

<a name="a_util_5_8_0"></a>
### [a_util 5.8.0][] - 2021/02/17
> _**Important note for users updating their downstream projects to this version:**_
>
> Some of the implementation details of component _Result_ had to be made public and are part of the
> users code now. This results in a change of exported symbols in the delivered binary. To prevent
> linker errors due to missing symbols recompiling and relinking against the a\_util 5.8.0 is
> necessary.

<a name="a_util_5_8_0_changes"></a>
#### Changes
- [ODAUTIL-125] - Provide component `a_util::result` header-only and deploy as single library
  - Starting from this release, the _Result_ component can be built and deployed as own header-only
    package. To require the package, conan reference `a_util_result/5.8.0@fep/stable` can be used.
    Changes to `find_package(a_util ...)` calls are not required.
- [ODAUTIL-175] - [memory] Add support for `std::make_unique()` for pre C\+\+14 compilers
  - This introduces backport implementations `a_util::memory::make_unique()`
- [ODAUTIL-188] - [build] Use native GTest and CMake functionality
- [ODAUTIL-191] - [filesystem] Compare content of two files
  - This introduces new functions `a_util::filesystem::compareFiles()`
- [ODAUTIL-196] - Use user "fep" instead of "aev25" in package references of the conan recipes
  - Starting from this release, a_util packages can be required with the conan reference
    `a_util/<version>@fep/<channel>`, e.g. `a_util/5.8.0@fep/stable`.
- [ODAUTIL-199] - Add _.clang-format_ file and apply ruleset on C\+\+ codebase
- [ODAUTIL-200] - Add preprocessor component to a_util library
  - This adds the new component `a_util::preprocessor` for basic preprocessor functionality
- [ODAUTIL-205] - [build] Provide packages for missing new profiles
- [ODAUTIL-206] - [build] Define and apply whitespace settings in _.gitattributes_ file
- [ODAUTIL-208] - [build] Generate API documentation using CoRTEXDocumentationHelper with doxygen
- [ODAUTIL-219] - [build] License information of dependencies must be part of the package
- [ODAUTIL-300] - [documentation] Provide easier access and better overview for API documentation

<a name="a_util_5_8_0_fixes"></a>
#### Fixes
- [ODAUTIL-122] - [variant] Copy Constructor does not correctly copy array values
- [ODAUTIL-174] - Product name in wiki does not match delivery
- [ODAUTIL-181] - [cmake] Redundant include directories and link libraries defined by targets
- [ODAUTIL-204] - [result] Functional test "result_type_from_lib_test" runs the wrong executable
- [ODAUTIL-207] - [build] Missing includes for VS2019 vc142
- [ODAUTIL-209] - [filesystem] `a_util::filesystem::Path::makeRelative()` fails for Root
- [ODAUTIL-211] - [build] Conan recipes not compatible with conan versions \>1.16.2
- [ODAUTIL-212] - [result] Exported CMake config does not work for either 32 bit or 64 bit architectures
- [ODAUTIL-215] - [build] Packaged CMake target 'a\_util' not part of export set

<a name="a_util_5_8_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_7_0"></a>
### [a_util 5.7.0][] - 2020/05/20
> _**Important note for users updating to this version:**_
>
> If you encounter any link errors due to unresolved symbols or any build errors due to missing
> or unknown types, please consult the documentation of the types and functions in question about
> the correct components to link and headers to include.
>
> Those errors may be raised due to fixes and clean up of includes and linked libraries, but will
> not break any other existing code.

<a name="a_util_5_7_0_changes"></a>
#### Changes
- [ODAUTIL-76] - [filesystem] Provide means to convert absolute to relative paths and vice versa
- [ODAUTIL-93] - [system] Get path to executable in currently running process
- [ODAUTIL-124] - [filesystem] Appending arbitrary strings to basenames of paths
- [ODAUTIL-138] - [filesystem] `operator<<` required for `a_util::filesystem::Path`
- [ODAUTIL-161] - [build] Generate documentation using conan
- [ODAUTIL-176] - [build] Use the newly defined profiles as base for the delivery packages
- [ODAUTIL-182] - [build] All public header files need to be self contained
- [ODAUTIL-183] - [documentation] Provide thorough documentation for the public API

<a name="a_util_5_7_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_6_1"></a>
### [a_util 5.6.1][] - 2020/02/12

<a name="a_util_5_6_1_changes"></a>
#### Changes
- [ODAUTIL-163] - Extend Jenkinsfile for cross platform build of QNX target on Windows host

<a name="a_util_5_6_1_fixes"></a>
#### Fixes
- [ODAUTIL-105] - [Result] Warning "unreferenced formal parameter" arises when including result\_type\_impl.h
- [ODAUTIL-157] - [BuildSystem] Language standard C\+\+11 is not propagated
- [ODAUTIL-168] - [Result] Macros cannot be used with a closing semicolon without compiler warning
- [ODAUTIL-169] - Function findNode of dom.h raises `-Werror=comment` warning on gcc

<a name="a_util_5_6_1_known_issues"></a>
#### [Known issues]

<a name="a_util_5_6_0"></a>
### [a_util 5.6.0][] - 2019/09/06

<a name="a_util_5_6_0_changes"></a>
#### Changes
- [ODAUTIL-139] - Implement new license mechanism in a_util
- [ODAUTIL-150] - Add Conan recipe and Jenkinsfile to support Jenkins Multibranch Pipeline jobs
- [ODAUTIL-155] - [BuildSystem] Change names of delivered library (\*.lib and \*.a) files

<a name="a_util_5_6_0_fixes"></a>
#### Fixes
- [ODAUTIL-142] - [memory] new allocated buffer is not nullified
- [ODAUTIL-149] - [system] `getCurrentUserName()` doesn't work correctly when running on QNX
- [ODAUTIL-160] - [log] log.h file is not self contained

<a name="a_util_5_6_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_5_0"></a>
### [a_util 5.5.0][] - 2019/04/12

<a name="a_util_5_5_0_changes"></a>
#### Changes
- [ODAUTIL-119] - Porting to SCALEXIO QNX
- [ODAUTIL-121] - Remove C\+\+11 compatibility code
  - **Breaking change:** Compilers without C\+\+11 support are not supported anymore. Please check
    the supported compiler section in the documentation.

#### Tasks
- [ODAUTIL-123] - Include the Open Source Disclaimer to the package
- [ODAUTIL-131] - Add install complete reference files

<a name="a_util_5_5_0_fixes"></a>
#### Fixes
- [ODAUTIL-126] - Use of plain signature of `target_link_libraries` on test targets leads to CMake error during
  configuration
- [ODAUTIL-129] - [Changelog] "Known Issues" use the wrong query
- [ODAUTIL-133] - Usage of gtest and gtest_main instead of `GTest::GTest` and `GTest::Main` as link libraries leads
  to linker errors

<a name="a_util_5_5_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_4_0"></a>
### [a_util 5.4.0][] - 2018/11/07

<a name="a_util_5_4_0_changes"></a>
#### Changes
- [ODAUTIL-115] - [common] Remove ADPL and add MPL 2.0 License
  - since the ADPL license is NOT compatible with the terms and conditions of the GPLv3 we will now
    distribute the A Util library under the common MPL 2.0 (Mozilla Public License 2.0). This
    will allow to reuse the library under GPLv3.

<a name="a_util_5_4_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_3_0"></a>
### [a_util 5.3.0][] - 2018/09/21

<a name="a_util_5_3_0_changes"></a>
#### Changes
- [ODAUTIL-94] - [parser] Add basic functionality to parse comma separated value (CSV) files
  - New component `a_util::parser` added with initial implementation to read comma separated value
    (\*.csv) files
  - **Usage:**
    - `find_package(a_util COMPONENTS csv_reader)` includes the csv reader into the current project
    - `target_link_libraries(myProject a_util_csv_reader)` might be used to link against the
      library
    - <tt>\#include "a_util/parser.h"</tt> includes the parser API into your current source file
- [ODAUTIL-103] - [process] Add a component for process control
  - New component `a_util::process` added with initial implementations to run executables within a
    new process as well as retrieve the current process id
  - **Usage:**
    - `find_package(a_util COMPONENTS process)` includes the process component into the current
      project
    - `target_link_libraries(myProject a_util_process)` might be used to link against the
      library
    - <tt>\#include "a_util/process.h"</tt> includes the process API into your current source file
- [ODAUTIL-108] - [process] Add reading and writing environment variables to the process
- [ODAUTIL-109] - Changelog must provide more and better readable information

<a name="a_util_5_3_0_fixes"></a>
#### Fixes
- [ODAUTIL-99] - Make odautil compiles out of the box
- [ODAUTIL-101] - [CMAKE] Could not call `find_package(a_util COMPONENTS ...)` more than one time
  - **Important note:** This encourages the usage of CMake package components to include the wanted
    a\_util components into projects. So instead of writing `find_package(a_util_strings)` we
    encourage you to write `find_package(a_util COMPONENTS strings)`

<a name="a_util_5_3_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_2_0"></a>
### [a_util 5.2.0][] - 2018/03/30

<a name="a_util_5_2_0_changes"></a>
#### Changes
- [ODAUTIL-3] - Add Solution for old cCommandLine (AUL-69)
  - [Clara]
    v1.1.2 was added as 3rdparty command line tool (C\+\+11 and above required)
  - **Usage:** After a successful `find_package(a_util)` consuming projects may first call
    `find_package(Clara)` followed by a `target_link_libraries(myLibrary Clara)`.
    Afterwards, Clara can be used as documented on its
    [project page]
- [ODAUTIL-77] - [DateTime] Timestamps must be generated with reference to Julian Time or Unix Time
  - `enum TimeStampReference` may be used to choose the wanted behavior in methods `DateTime::set`
    and `DateTime::toTimestamp`
- [ODAUTIL-95] - Use googletest functionality provided by CMake command `find_package(GTest)`
- [ODAUTIL-98] - Add changelog file and initially start documenting changes for version 5.2.0

<a name="a_util_5_2_0_fixes"></a>
#### Fixes
- [ODAUTIL-81] - `Semaphore::wait_for` loops forever if timeout is too big
- [ODAUTIL-90] - [DateTime] Scoped enums and `std::chrono` don't compile with pre-C\+\+11 gcc compilers
- [ODAUTIL-97] - [base] CMake `configure_file()` must not generate config.h into source tree

<a name="a_util_5_2_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_1_0"></a>
### [a_util 5.1.0][] - 2017/12/08

<a name="a_util_5_1_0_changes"></a>
#### Changes
- [ODAUTIL-61] - [Result] Add debug helper for Visual Studio 2012\+ (.natvis file) (AUL-65)
- [ODAUTIL-68] - Remove googletest as 3rdparty dependency (AUL-68)
- [ODAUTIL-72] - Add ADPL License Info to all artefact (Header, CPPs)
- [ODAUTIL-73] - Automatically set C\+\+ standard to the platform maximum
- [ODAUTIL-75] - Add try\_wait method to `a_util::concurrency::semaphore` class

<a name="a_util_5_1_0_fixes"></a>
#### Fixes
- [ODAUTIL-10] - Thread detach() test can fail due to race condition (AUL-67)
- [ODAUTIL-66] - StackPtr Test crashes on U1604\_x64\_gcc54 (AUL-66)

<a name="a_util_5_1_0_known_issues"></a>
#### [Known issues]

<a name="a_util_5_0_0"></a>
### [a_util 5.0.0][] - 2017/09/01

<a name="a_util_5_0_0_changes"></a>
#### Changes
- [ODAUTIL-12] - Fix Timer by rewriting Thread (AUL-63)
- [ODAUTIL-14] - [XML] Provide functionality to check for empty DOM and DOMElement objects (AUL-61)
- [ODAUTIL-15] - Split API and implementation of header only libraries into separate files (AUL-60)
- [ODAUTIL-16] - Apply style guidelines (AUL-57)
- [ODAUTIL-17] - [Base] Provide static_assert() backport (AUL-56)
- [ODAUTIL-20] - Test result output path must be configurable in integrated build scenario (AUL-53)
- [ODAUTIL-21] - Adapt API to updated style guidelines (AUL-51)
- [ODAUTIL-22] - Improve DOM FindNodes performance (AUL-50)
- [ODAUTIL-26] - Create a_util_base component (AUL-46)
- [ODAUTIL-29] - Provide interface target pulling in all AUL components at once (AUL-42)
- [ODAUTIL-32] - Provide mechanism to generate GUIDs (AUL-38)
- [ODAUTIL-33] - Provide support for high resolution timing on all platforms (AUL-37)
- [ODAUTIL-34] - Provide access to hostname (AUL-36)
- [ODAUTIL-35] - Disallow deleted CCTORS and assignment operators in concurrency fallback classes (AUL-35)
- [ODAUTIL-37] - [Base] Implement nullptr fallback for compilers not providing an implementation (AUL-33)
- [ODAUTIL-38] - Add basic semaphore implementation (AUL-32)
- [ODAUTIL-40] - Add missing string functionality (AUL-30)
- [ODAUTIL-41] - [Result] Port tResult and error handling mechanism from a_util 4 (AUL-29)
- [ODAUTIL-42] - `a_util::memory::MemoryBuffer` must support wrapping non-owned memory (AUL-28)
- [ODAUTIL-43] - `a_util::filesystem` has to have a rename or move function (AUL-27)
- [ODAUTIL-44] - extend `a_util::memory` for a mem_zero (AUL-26)
- [ODAUTIL-45] - Add copy functionality to `a_util::strings` (AUL-25)
- [ODAUTIL-46] - [System] Provide periodic timer - {Redmine #38883} (AUL-24)
- [ODAUTIL-47] - [System] Provide access to system API - {Redmine #37454} (AUL-22)
- [ODAUTIL-48] - [Filesystem] Provide support for filesytem access - {Redmine #37451} (AUL-20)
- [ODAUTIL-49] - [Chrono] Provide date and time functionality - {Redmine #37446} (AUL-19)
- [ODAUTIL-50] - [Regex] Provide support for regular expressions - {Redmine #37432} (AUL-18)
- [ODAUTIL-51] - [Logging] Provide infrastructure for application logging - {Redmine #37426} (AUL-17)
- [ODAUTIL-52] - [Variant] Provide wrapper to store arbitrary data types - {Redmine #37424} (AUL-16)
- [ODAUTIL-53] - [Strings] Provide extended functionality for `std::string` and C-style strings - {Redmine #37423} (AUL-15)
- [ODAUTIL-54] - [XML] Implement XML API - {Redmine #37350} (AUL-13)
- [ODAUTIL-55] - [cmake] Enable a_util_library to be usable as CMake package - {Redmine #37301} (AUL-12)
- [ODAUTIL-56] - Provide solution for last system error (AUL-11)
- [ODAUTIL-57] - [PortableTypes] Provide portable base types for all supported platforms - {Redmine #37352} (AUL-10)
- [ODAUTIL-58] - Enable doxygen documentation (AUL-3)
- [ODAUTIL-62] - [cmake] Naming convention of target libraries must reflect their platform dependence and build
  type - {Redmine #37994} (AUL-23)
- [ODAUTIL-63] - [Memory] Provide support for handling raw memory - {Redmine #37452} (AUL-21)
- [ODAUTIL-64] - [Memory] Provide smart pointer implementations for platforms not providing them - {Redmine #37353} (AUL-14)
- [ODAUTIL-65] - [Concurrency] Provide threading and synchronization mechanisms - {Redmine #38652} (AUL-4)
- [ODAUTIL-70] - Provide means to make canonical paths in `a_util::filesystem` (AUL-43)
- [ODAUTIL-71] - Provide fallback for C\+\+11 type traits (AUL-41)

<a name="a_util_5_0_0_fixes"></a>
#### Fixes
- [ODAUTIL-13] - [Result] Error definitions must not pollute the global namespace (AUL-62)
- [ODAUTIL-18] - System error test fails on Windows 7 / Visual Studio 2010 (AUL-55)
- [ODAUTIL-23] - `concurrency::thread::detach` crashes on fallback platforms (AUL-49)
- [ODAUTIL-24] - [Test] Overwrite `add_test()` macro instead of providing wrapper `add_util_test()` (AUL-48)
- [ODAUTIL-25] - [Test] Fix disabled installation of googletest (AUL-47)
- [ODAUTIL-31] - [Testing] Setting an alternative working directory (AUL-39)
- [ODAUTIL-36] - [Testing] Remove link dependency to a_util_filesystem for test targets (AUL-34)
- [ODAUTIL-39] - `a_util::variant::Variant` CCTOR doesn't properly copy strings (AUL-31)

<a name="a_util_5_0_0_known_issues"></a>
#### [Known issues]


<a name="ddl_library"></a>
## DDL library

<a name="ddl_4_6_0"></a>
### [ddl library 4.6.0][] - 2021/02/22

<a name="ddl_4_6_0_changes"></a>
#### Changes
- [CDDDL-116] - [Header2DDL] Types of hexadecimal values in enums must be interpreted as underlying type
- [CDDDL-117] - [Header2DDL] Basic preprocessor functionality needs to be implemented
- [CDDDL-118] - [Header2DDL] Basic arithmetic operation of constant integers must be implemented
- [CDDDL-132] - Use user "fep" instead of "aev25" in package references of the conan recipes
- [CDDDL-136] - [build] Define and apply whitespace settings in .gitattributes file
- [CDDDL-140] - [build] Use native GTest and CMake functionality
- [CDDDL-141] - [build] Generate API documentation using CoRTEXDocumentationHelper with doxygen
- [CDDDL-142] - [build] Restructure files and folders in source tree compliant to common configuration management
- [CDDDL-146] - Add .clang-format file and apply ruleset on C\+\+ codebase
- [CDDDL-155] - [test] Use more fitting test macros

<a name="ddl_4_6_0_fixes"></a>
#### Fixes
- [CDDDL-60] - Make cdddl compiles out of the box
- [CDDDL-97] - [ddlcontainer] Segmentation fault on linux during deleteAll()
- [CDDDL-98] - [build] Mutual dependency between conan recipe and CMake build automation of
  'list_used_licenses.md.in'
- [CDDDL-126] - [ddl2header] Test fixture cTesterddlgeneratorCmdLine fails on platforms using gcc
- [CDDDL-134] - [build] Unresolved symbols after making parts of a_util_result header-only
- [CDDDL-138] - [build] Conan recipes not compatible with conan versions \>1.16.2
- [CDDDL-139] - [ddl2header] Might crash with --struct command line option
- [CDDDL-153] - [Mapping] Possible null pointer exception

<a name="ddl_4_6_0_known_issues"></a>
#### [Known issues]


<a name="ddl_4_5_0"></a>
### [ddl library 4.6.0][] - 2020/05/25

<a name="ddl_4_5_0_changes"></a>
#### Changes
- [CDDDL-123] - Use the newly defined profiles as base for the delivery packages

<a name="ddl_4_5_0_fixes"></a>
#### Fixes
- [CDDDL-126] - [build] Build errors after update to a_util 5.7.0

<a name="ddl_4_5_0_known_issues"></a>
#### [Known issues]


<a name="ddl_4_4_1"></a>
### [ddl library 4.4.1][] - 2020/02/12

<a name="ddl_4_4_1_changes"></a>
#### Changes
- [CDDDL-111] - Extend Jenkinsfile for cross platform build of QNX target on Windows host

<a name="ddl_4_4_1_fixes"></a>
#### Fixes
- [CDDDL-119] - Missing semicolons when using a_util `_MAKE_ERROR` lead to error during compile time

<a name="ddl_4_4_1_known_issues"></a>
#### [Known issues]


<a name="ddl_4_4_0"></a>
### [ddl library 4.4.0][] - 2019/09/06

<a name="ddl_4_4_0_changes"></a>
#### Changes
- [CDDDL-85] - Implement new license mechanism in ddl
- [CDDDL-89] - Implement valid copy and move semantics
- [CDDDL-99] - Add conan recipe and Jenkinsfile to repository
- [CDDDL-102] - [BuildSystem] Change names of delivered libraries (*.lib and *.a) files

<a name="ddl_4_4_0_fixes"></a>
#### Fixes
- [CDDDL-92] - Missing merge of CDDDL-56 into master
- [CDDDL-103] - [BuildSystem] Language standard C\+\+11 is not propagated

<a name="ddl_4_4_0_known_issues"></a>
#### [Known issues]


<a name="ddl_4_3_0"></a>
### [ddl library 4.3.0][] - 2019/06/14

<a name="ddl_4_3_0_changes"></a>
#### Changes
- [CDDDL-74] - [Test] Normalize AEV_TEST macro to standard GoogleTest macro
- [CDDDL-81] - [Test] Exchange `aev_add_gtest()` with ordinary CMake command `add_test()``
- [CDDDL-82] - Porting to QNX
- [CDDDL-84] - Add reference files for tester install complete to verify DDL Library package

<a name="ddl_4_3_0_fixes"></a>
#### Fixes
- [CDDDL-65] - [ddlrepresentation] cannot rename sorted DDLContainer item
- [CDDDL-67] - [ddlinspector] Min/max values for tBool hold literals instead of numeric values
- [CDDDL-70] - [DDLDescription] Default min\+max values for tInt8 and tUInt8 are not numbers
- [CDDDL-71] - [DDLCloner] Check for memory leak in cDDLCloner
- [CDDDL-75] - [ddlinspector] Default value of a structure element without bounding limits is not compared with
  datatype bounding limits

<a name="ddl_4_3_0_known_issues"></a>
#### [Known issues]


<a name="ddl_4_2_1"></a>
### [ddl library 4.2.1][] - 2019/03/19

<a name="ddl_4_2_1_fixes"></a>
#### Fixes
- [CDDDL-77] - DDLResolver uses current date and time in generated description

<a name="ddl_4_2_1_known_issues"></a>
#### [Known issues]


<a name="ddl_4_2_0"></a>
### [ddl library 4.2.0][] - 2018/11/08

<a name="ddl_4_2_0_changes"></a>
#### Changes
- [CDDDL-61] - Create XSDs for DDL 3 and 4
- [CDDDL-62] - Remove ADPL and add MPL 2.0 License

<a name="ddl_4_2_0_fixes"></a>
#### Fixes
- [CDDDL-64] - DDL version migrate cmake makro throws warnings

<a name="ddl_4_2_0_known_issues"></a>
#### [Known issues]


<a name="ddl_4_1_0"></a>
### [ddl library 4.1.0][] - 2018/06/30

<a name="ddl_4_1_0_changes"></a>
#### Changes
- [CDDDL-15] - Change Version Variable of OO DDL from Floating to a struct Major/Minor
- [CDDDL-16] - Change to new Coding Style
- [CDDDL-17] - Reenable Documentation creation
- [CDDDL-24] - Unify commandline arguments with other utilities - commandline tooling must use --my_parameter
  (corresponding -mp)
- [CDDDL-34] - [DOC] DDL Documentation must use Markdown format and the content must be updated
- [CDDDL-39] - Create clang-tidy script for porting usercode from 4.0 to 4.1
- [CDDDL-44] - [Mapping] Add method to mapping engine to check whether a signal has any triggers
- [CDDDL-45] - Add simple possibility to identify dynamic arrays
- [CDDDL-46] - [DDLInspector] Invalid numbers containing multiple '.' must be detected and corrected
- [CDDDL-48] - Check const correctness of DDL Library

<a name="ddl_4_1_0_fixes"></a>
#### Fixes
- [CDDDL-35] - cDDLImporter uses `ArraySizeSource` wrong
- [CDDDL-36] - cDDLCloner cannot clone FEP Driver DDL
- [CDDDL-41] - [DDL] Type names containing '/' are not supported

<a name="ddl_4_1_0_known_issues"></a>
#### [Known issues]


<a name="ddl_4_0_0"></a>
### [ddl library 4.0.0][] - 2017/12/06

<a name="ddl_4_0_0_changes"></a>
#### Changes

<a name="ddl_4_0_0_fixes"></a>
#### Fixes

<a name="ddl_4_0_0_known_issues"></a>
#### [Known issues]


<a name="upcoming"></a>
### [Upcoming][]
Shows the upcoming features.

[dev_essential 1.0.0]:
https://github.com/cariad-tech/FEP/dev_essential/tree/v1.0.0
