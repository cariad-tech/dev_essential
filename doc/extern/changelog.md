<!---
Copyright @ 2023 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
-->

# Dev essential changelog

All notable changes to project **dev essential libraries** will be documented here.
For upcoming features and known issues see the bottom of this file.

The format is based on [Keep a Changelog][] and this project adheres to [Semantic Versioning][].

## Version history
<h3><a href="#dev_essential_libraries">Dev essential libraries</a></h3>

* [1.3.1](#dev_essential_1_3_1) \| Changes \|
  [Fixes](#dev_essential_1_3_1_fixes) \| [Known issues](#dev_essential_1_3_1_known_issues) \|
  Release date: 2023/07/10
* [1.3.0](#dev_essential_1_3_0) \| [Changes](#dev_essential_1_3_0_changes) \|
  [Fixes](#dev_essential_1_3_0_fixes) \| [Known issues](#dev_essential_1_3_0_known_issues) \|
  Release date: 2022/11/29
* [1.2.2](#dev_essential_1_2_2) \| Changes \|
  [Fixes](#dev_essential_1_2_2_fixes) \| [Known issues](#dev_essential_1_2_2_known_issues) \|
  Release date: 2022/11/11
* [1.2.1](#dev_essential_1_2_1) \| Changes \|
  [Fixes](#dev_essential_1_2_1_fixes) \| [Known issues](#dev_essential_1_2_1_known_issues) \|
  Release date: 2022/08/12
* [1.2.0](#dev_essential_1_2_0) \| [Changes](#dev_essential_1_2_0_changes) \|
  [Fixes](#dev_essential_1_2_0_fixes) \| [Known issues](#dev_essential_1_2_0_known_issues) \|
  Release date: 2022/07/15
* [1.1.4](#dev_essential_1_1_4) \| Changes \|
  [Fixes](#dev_essential_1_1_4_fixes) \| [Known issues](#dev_essential_1_1_4_known_issues) \|
  Release date: 2022/05/20
* [1.1.3](#dev_essential_1_1_3) \| Changes \|
  [Fixes](#dev_essential_1_1_3_fixes) \| [Known issues](#dev_essential_1_1_3_known_issues) \|
  Release date: 2022/05/13
* [1.1.2](#dev_essential_1_1_2) \| Changes \|
  [Fixes](#dev_essential_1_1_2_fixes) \| [Known issues](#dev_essential_1_1_2_known_issues) \|
  Release date: 2022/03/04
* [1.1.1](#dev_essential_1_1_1) \| [Changes](#dev_essential_1_1_1_changes) \|
  [Fixes](#dev_essential_1_1_1_fixes) \| [Known issues](#dev_essential_1_1_1_known_issues) \|
  Release date: 2022/01/21
* [1.1.0](#dev_essential_1_1_0) \| [Changes](#dev_essential_1_1_0_changes) \|
  [Fixes](#dev_essential_1_1_0_fixes) \| [Known issues](#dev_essential_1_1_0_known_issues) \|
  Release date: 2021/10/14
* [1.0.0](#dev_essential_1_0_0) \| [Changes](#dev_essential_1_0_0_changes) \|
  [Fixes](#dev_essential_1_0_0_fixes) \| [Known issues](#dev_essential_1_0_0_known_issues) \|
  Release date: 2021/08/04
* [Upcoming](#upcoming)

<h3><a href="#utility_library">Utility library</a></h3>

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

<h3><a href="#ddl_library">DDL library</a></h3>

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


<h2><a name="dev_essential_libraries">Dev essential libraries</a></h2>
<a name="dev_essential_1_3_1"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.3.1">dev_essential 1.3.1</a> - 2023/07/10</h3>

<a name="dev_essential_1_3_1_fixes"></a>
#### Fixes
_**Fixed**_
 - [CPPDEV-507][] - [System] `a_util::system::Timer` crashes [\[view\]][79913024c9a] [\[view\]][e1e759f0b69] [\[view\]][cbd935f973f]
   - Fixes sporadically crashing timer
   - Fixes timer in case of ONESHOT
   - Fixes timer test for repeatedly calls
 - [CPPDEV-590][] - [mapping] Test execution creates and deletes files in source tree [\[view\]][99f440f0f83]
   - Creation of files moved to build folder
 - [CPPDEV-592][] - [system] system_test.TestUsername fails when executed in Ubuntu 18.04 32 bit docker image [\[view\]][8bad6d55f5f]
   - Runs commands in docker container as non-root
 - [CPPDEV-615][] - [result,ddl] Using `min()/max()` in public headers might interfere with function like macros in Windows.h [\[view\]][ee37afe687f]
   - Fixes min/max macro expansion from &quot;Windows.h&quot; using parenthesis around the min/max function calls
 - [CPPDEV-617][] - [ddl] Mapping language version interpreted as double [\[view\]][55777ea26fa]
 - [CPPDEV-619][] - Known issues links in changelog contain useless information [\[view\]][4aa35bf38b3]
 - [CPPDEV-620][] - [concurrency] Execution of `a_util::concurrency::semaphore` tests takes a long time [\[view\]][926ce37a1b0]
 - [CPPDEV-624][] - [system] `a_util::system::AddressInfo` does not decrease the ref count of the referenced module (windows only) [\[view\]][c47707f6210]
   - Fixes reference count for module handle
 - [CPPDEV-631][] - MPL2.0 license notice is wrong [\[view\]][3475aecc47a]
 - [CPPDEV-636][] - [DOC] Documentation contains false information (ongoing) [\[view\]][b3cc17cdbc7] [\[view\]][d4703d5fa86]
 - [CPPDEV-638][] - [build] _version_ file not exported to conan exports folder [\[view\]][b0f8ea36a60]
   - version file is now exported to conan exports folder
 - [CPPDEV-639][] - [ddl] Build error when cross building from &quot;Windows:x86_64&quot; to &quot;Windows:x86&quot; [\[view\]][1af294a5f4a]

<a name="dev_essential_1_3_1_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.3.1%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-507]: https://devstack.vwgroup.com/jira/browse/CPPDEV-507
[CPPDEV-590]: https://devstack.vwgroup.com/jira/browse/CPPDEV-590
[CPPDEV-592]: https://devstack.vwgroup.com/jira/browse/CPPDEV-592
[CPPDEV-615]: https://devstack.vwgroup.com/jira/browse/CPPDEV-615
[CPPDEV-617]: https://devstack.vwgroup.com/jira/browse/CPPDEV-617
[CPPDEV-619]: https://devstack.vwgroup.com/jira/browse/CPPDEV-619
[CPPDEV-620]: https://devstack.vwgroup.com/jira/browse/CPPDEV-620
[CPPDEV-624]: https://devstack.vwgroup.com/jira/browse/CPPDEV-624
[CPPDEV-631]: https://devstack.vwgroup.com/jira/browse/CPPDEV-631
[CPPDEV-636]: https://devstack.vwgroup.com/jira/browse/CPPDEV-636
[CPPDEV-638]: https://devstack.vwgroup.com/jira/browse/CPPDEV-638
[CPPDEV-639]: https://devstack.vwgroup.com/jira/browse/CPPDEV-639

<!--- Commit links -->
[79913024c9a]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/79913024c9ac151604b9d20f2ee5ba75e189f9eb
[e1e759f0b69]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/e1e759f0b694450856abe53817468bf58e37184f
[cbd935f973f]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/cbd935f973ff5e47c6d8653d82ba96bd4e1527fe
[99f440f0f83]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/99f440f0f83be7bdefda9e516a391351e64afa6d
[8bad6d55f5f]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/8bad6d55f5f35155fe54dabe584429f658486a16
[ee37afe687f]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/ee37afe687f6c7264fa610b1d5eec1b1cab96d4d
[55777ea26fa]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/55777ea26fa7c7ebf564e9c3d4b2641277ea04d3
[c47707f6210]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/c47707f6210ca7457e386153b7ac83818494dc73
[b0f8ea36a60]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/b0f8ea36a60871b18dcc77ac24043af469887b71
[4aa35bf38b3]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/4aa35bf38b3e346d601357727d1f76461faaa8e0
[926ce37a1b0]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/926ce37a1b0759bb2d2cb6e76e3a64a74618826b
[3475aecc47a]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/3475aecc47a6b26bc6281ef4891f03f256664c32
[1af294a5f4a]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/1af294a5f4a1d00d6085638449e8ffba9723a945
[b3cc17cdbc7]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/b3cc17cdbc707032987c6271f6e12b7956df130a
[d4703d5fa86]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/d4703d5fa864452061bc32b32ef50daff263f231

<a name="dev_essential_1_3_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.3.0">dev_essential 1.3.0</a> - 2022/11/29</h3>

<a name="dev_essential_1_3_0_changes"></a>
#### Changes
_**Done**_
- [CPPDEV-165][] - [Result] Make `a_util::result::isOk()` self explaining and type safe
  - Adds explicit bool conversion operator with `a_util::result::Result::operator bool()` [\[view\]][95b93898b08]
  - Deprecates `a_util::result::isOk()` and `a_util::result::isFailed()` [\[view\]][f83a3d6cb93]
- [CPPDEV-178][] - [system] Add functionality to query the file path of the binary a memory address is located in [\[view\]][a67082bbd15] 
  - Added `a_util::system::AddressInfo` class to query the file path of the binary the memory address is located in
- [CPPDEV-535][] - Update to conan version 1.48.2 and enable [CONAN_V2_MODE][]
  - Replaces build_requires with tool_requires [\[view\]][f494c5bba58] 
  - Removes short_paths attribute from conanfiles and uses environment variable `CONAN_USE_ALWAYS_SHORT_PATHS="True"` in Jenkinsfile instead [\[view\]][975c0e2bbd8]
  - Uses MPL-2.0 license (SPDX identifier) as value for license attribute [\[view\]][5a3a0d1487e]
  - Adds homepage attribute [\[view\]][0fd9f12d8cb]
  - Removes deprecated settings arch_build and os_build attributes and sets the necessary settings in the Jenkinsfile instead [\[view\]][f680db51621]
  - Removes attributes default_channel and default_user which can be passed on the command line when calling conan install/create [\[view\]][d837e8d1b88]
  - Enables CONAN_V2_MODE and fixes the warnings acccordingly [\[view\]][025b8717105]
- [CPPDEV-514][] - [ddl] Adapt `ddl::DDCompare` to deal with padding bytes [\[view\]][8e240bd2153] 
  - Adapt `ddl::DDCompare::isBinaryEqual()` for padding bytes
- [CPPDEV-544][] - [build] Docker adjustments for Linux x86 packages [\[view\]][fc04fd277da] 
  - Adds an i386 Dockerfile for different gcc/conan versions
  - Adds two new stages to Jenkinsfile using the dockerfile agent to build gcc5/7 32 bit variants
- [CPPDEV-546][] - [build] Update CMake version
  - Replaces add_test() with gtest_discover_tests() [\[view\]][644c448713e]
  - Adds `add_test()` usage check [\[view\]][95a6be7c4a0]
  - Uses `cmake_path()` over (deprecated) `get_filename_components()` (requires at least CMake 3.20) [\[view\]][f8ebf440e77]
  - Changes `gtest discover tests()` mode to PRE_TEST [\[view\]][5a3ff1a2c22]
  - Uses `ctest --output-junit` to create junit xml file (requires at least CMake 3.21) [\[view\]][4190e6b4882]
- [CPPDEV-574][] - [clipp][] available in dev essentials
  - Adds clipp 1.2.3 to 3rdparty components [\[view\]][e94e0c91f6c]
  - Adds clipp find script to scripts/cmake/modules [\[view\]][201c1a6aac3]
  - Adds clipp tests as private 3rdparty tests [\[view\]][4b213f21ce8] [\[view\]][7c8159c6fa0]
  - Removes [clara][] which is replaced by clipp [\[view\]][899031056ed]
  - Adds acceptance tests for stubgenerator [\[view\]][f85400c3eb7]
  - Replaces `cCommandLine` with clipp [\[view\]][bbe6bb0c6c5]
- [CPPDEV-581][] - [preprocessor] Add macro to enable/disable deprecated warnings [\[view\]][32362f137a9] [\[view\]][166dd57162a] 
  - Adds `DEV_ESSENTIAL_DEPRECATED()` macro in new file _a\_util/preprocessor/deprecated.h_
  - The deprecation of entities can be disabled by defining `DEV_ESSENTIAL_DEPRECATED_DISABLE_WARNINGS`
  - Adds &quot;Deprecated section&quot; in doxygen documentation
  - Adds doxygen alias &quot;\@dev_essential_deprecated&quot; to document deprecated entities
- [CPPDEV-598][] - [rpc] Add possibility to handle non-RPC http requests [\[view\]][4c79d02e394] 
  - Added new virtual function that is passed all relevant HTTP request and response data
- [CPPDEV-610][] Append _cmake/modules_ path to CMAKE_MODULE_PATH for downstream projects [\[view\]][505353db5e0]
  - Adds path _&lt;dev_essential_DIR&gt;/cmake/modules_ to `CMAKE_MODULE_PATH` if component pkg_rpc is found by consuming project

_**Won't Do**_
- [CPPDEV-248][] - Evaluate and - if possible - use [argtable3][] instead of `cCommandLine` for libjson-rpc-cpp
  - Already solved with [CPPDEV-574][]

<a name="dev_essential_1_3_0_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-524][] - DDL specification documentation contains information about ADTF but not about streams [\[view\]][f6eae2d106b] 
  - Added documentation for streams and a TOC for ddl chapters in documentation
  - &quot;ADTF&quot; in documentation is now removed
  - documentation for streams is added to the specification
- [CPPDEV-560][] - [build] clang-tidy default checks show findings [\[view\]][4ec2d960974]
  - Found and fixed/silenced all default clang-tidy (10.0.0) findings
- [CPPDEV-573][] - Merge of incompatible DDL structs of the same name does not throw an exception [\[view\]][a53e41151ef] 
  - Added tests for merging of data definitions
  - Fixed merging of different and incompatible StructTypes with the same name
- [CPPDEV-586][] - [base] Redefinition of `std::is_enum_v` if _a\_util/base/type\_traits.h_ is compiled with clang -std=c++17 [\[view\]][cd970efb7d6] 
  - Fixes feature check for `std::is_enum_v`
- [CPPDEV-587][] - [build] Different symbol visibility between standalone and integrated tests [\[view\]][52663731064] 
  - Renamed file to _set_symbol_visibility.cmake_
  - Included file _scripts/cmake/set_symbol_visibility.cmake_ in standalone tests as well
- [CPPDEV-589][] - [ddl] Check for MSVC does not consider selected C++ language in _ddl/utilities/dd_access_list.h_ [\[view\]][3913852ec5f] 
  - Adds library feature check for `std::string_view`
  - Uses the library feature check to correctly determine C++14/17 compliant compilers
- [CPPDEV-609][] Result description is missing for 32bit [\[view\]][58c61bfd693]
  - Fixes result pointer cast for gcc x86

_**Duplicate**_
- [CPPDEV-588][] - [ddl] DD::StructureGenerator creates invalid DDL descriptions if used as global variable 
  - Fixed with [CPPDEV-580][] and released with [dev_essential 1.2.1](#dev_essential_1_2_1)

_**Won't Do**_
- [CPPDEV-602][] - [rpc] static library compiled without -fPIC might cause coredumps on QNX when linked into a shared library 
   - No QNX release is scheduled for pkg_rpc anymore. However, a fix was provided in [dev_essential 1.0.0](#dev_essential_1_0_0) with issue [CPPDEV-270][].

<a name="dev_essential_1_3_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.3.0%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-165]: https://devstack.vwgroup.com/jira/browse/CPPDEV-165
[CPPDEV-178]: https://devstack.vwgroup.com/jira/browse/CPPDEV-178
[CPPDEV-248]: https://devstack.vwgroup.com/jira/browse/CPPDEV-248
[CPPDEV-514]: https://devstack.vwgroup.com/jira/browse/CPPDEV-514
[CPPDEV-524]: https://devstack.vwgroup.com/jira/browse/CPPDEV-524
[CPPDEV-535]: https://devstack.vwgroup.com/jira/browse/CPPDEV-535
[CPPDEV-544]: https://devstack.vwgroup.com/jira/browse/CPPDEV-544
[CPPDEV-546]: https://devstack.vwgroup.com/jira/browse/CPPDEV-546
[CPPDEV-560]: https://devstack.vwgroup.com/jira/browse/CPPDEV-560
[CPPDEV-573]: https://devstack.vwgroup.com/jira/browse/CPPDEV-573
[CPPDEV-574]: https://devstack.vwgroup.com/jira/browse/CPPDEV-574
[CPPDEV-581]: https://devstack.vwgroup.com/jira/browse/CPPDEV-581
[CPPDEV-586]: https://devstack.vwgroup.com/jira/browse/CPPDEV-586
[CPPDEV-587]: https://devstack.vwgroup.com/jira/browse/CPPDEV-587
[CPPDEV-588]: https://devstack.vwgroup.com/jira/browse/CPPDEV-588
[CPPDEV-589]: https://devstack.vwgroup.com/jira/browse/CPPDEV-589
[CPPDEV-598]: https://devstack.vwgroup.com/jira/browse/CPPDEV-598
[CPPDEV-602]: https://devstack.vwgroup.com/jira/browse/CPPDEV-602
[CPPDEV-609]: https://devstack.vwgroup.com/jira/browse/CPPDEV-609
[CPPDEV-610]: https://devstack.vwgroup.com/jira/browse/CPPDEV-610

<!--- Commit links -->
[fc04fd277da]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/fc04fd277dad2f4846153d58f689823a059e3aa3
[4190e6b4882]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/4190e6b48824b8a354c7b293c8106f0d695c1a42
[8e240bd2153]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/8e240bd21533435755487e4aec78037dfca3a3a5
[4ec2d960974]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/compare/diff?targetBranch=ce1a93ef56c0ec1ec135fe0dbcddca2de58f9060&sourceBranch=4ec2d960974c1c80982bd2caeac9af902e95daf6&targetRepoId=1703
[166dd57162a]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/166dd57162a1610d86210d2cec68837d0e67bb6d
[32362f137a9]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/32362f137a920f1a0d0eae584dd97090ed6e9c88
[95b93898b08]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/95b93898b0896de6c202dcd0d9dc1ed1a0f16faf
[f83a3d6cb93]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f83a3d6cb93d7a16ef887ba1151848f66c62dff7
[52663731064]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/52663731064872f226bb58d5e72c7ffba0d96f07
[f494c5bba58]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f494c5bba587aac40b5ab5b16d96c0581367551e
[975c0e2bbd8]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/975c0e2bbd854ff331874e71283535855808812e
[5a3a0d1487e]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/5a3a0d1487e791b276d3f4068215c40462196f01
[0fd9f12d8cb]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/0fd9f12d8cbd0074c7891c01448c7bc4c20fd7a6
[f680db51621]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f680db51621ff2dbfb4d7848965943c3d896f0d6
[d837e8d1b88]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/d837e8d1b88f2245e56e606972b29d00db8ca62c
[025b8717105]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/025b87171051011a1aaaa40fbe016b29f61ac4d9
[a67082bbd15]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/a67082bbd158fde4681c89d1dd6027f50a5c1f02
[644c448713e]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/644c448713e783bde29c2fc3128bd90ee0cb15d8
[3913852ec5f]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/3913852ec5fef50ac7c404c46ac1835196796516
[5a3ff1a2c22]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/5a3ff1a2c22f499468c2b71a2803116a25edbf54
[cd970efb7d6]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/cd970efb7d67d92b4904f72c43a9abce0d3bb611
[e94e0c91f6c]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/e94e0c91f6c7dfedb741a6f3a275c14c65aae663
[201c1a6aac3]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/201c1a6aac33f3860ae188be7e3445ce97427fc7
[4b213f21ce8]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/4b213f21ce8624d229cc7b302420532e66d89cb0
[899031056ed]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/899031056ed84d1a43334f748ce62bf1ab304b7d
[f8ebf440e77]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f8ebf440e77b314e5c1728e99b1f59a9641eccb3
[a53e41151ef]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/a53e41151efb3f46a8550b208eb7dfd43482038f
[f6eae2d106b]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f6eae2d106b61955a0fb91226662a584d16dfbcb
[4c79d02e394]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/4c79d02e394e1b4ea0759b1e4711635a230f8980
[95a6be7c4a0]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/95a6be7c4a05a97d9901d309697a9bb0823a5f09
[f85400c3eb7]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f85400c3eb72686e3b6d4fccd418a0d90721b9db
[bbe6bb0c6c5]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/bbe6bb0c6c5f1fe55e978fa6f5899aba608d5a2f
[7c8159c6fa0]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/7c8159c6fa0edec9a754d4d794b8533045571555
[505353db5e0]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/505353db5e02b0e958594920100848ee8e49ae6f
[58c61bfd693]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/58c61bfd6936d25a5bd97356964cf7e4bbb1251a

<a name="dev_essential_1_2_2"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.2.2">dev_essential 1.2.2</a> - 2022/11/11</h3>

<a name="dev_essential_1_2_2_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-604] - [ddl] DDL has performance problems within dependency management if many dependencies added [\[view\]][a818d733a51]
    - changed the datastructure within (hidden - deprecated) validationmodel to std::unordered_set<> (see CPPDEV-611)
    - more time is "wasted" within typeinfo model while copying a StructType/DataType/EnumType from one DD to another DD

<a name="dev_essential_1_2_2_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.2.2%22">Known issues</a></h4>

 <!--- Issue links -->
[CPPDEV-604]: https://devstack.vwgroup.com/jira/browse/CPPDEV-604

<!--- Commit links -->
[a818d733a51]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/a818d733a51094768db949793babb6116099c139

<a name="dev_essential_1_2_1"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.2.1">dev_essential 1.2.1</a> - 2022/08/12</h3>

<a name="dev_essential_1_2_1_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-580] - [ddl] Static intializing order problem for all `ddl::dd::Version::ddl_version_*` used within other static variables [\[view\]][48fa35f7cd3]
- [CPPDEV-585] - [ddl] Wrong bit_size by `ddl::codec::CodecIndex::getIndexForArrayPos()` [\[view\]][b12e8500d96]

<a name="dev_essential_1_2_1_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.2.1%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-580]: https://devstack.vwgroup.com/jira/browse/CPPDEV-580
[CPPDEV-585]: https://devstack.vwgroup.com/jira/browse/CPPDEV-585

<!--- Commit links -->
[48fa35f7cd3]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/48fa35f7cd3a4ae909cd4d6e04e1af1f8b956447
[b12e8500d96]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/b12e8500d96c1c5f1140ad15f90d71dbeb50be07

<a name="dev_essential_1_2_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.2.0">dev_essential 1.2.0</a> - 2022/07/15</h3>

<a name="dev_essential_1_2_0_changes"></a>
#### Changes
_**Done**_
- [CPPDEV-214][] - Use clang-tidy as static code analyzer [\[view\]][f4cd19d2705] [\[view\]][a40ec9e5d7c]
- [CPPDEV-217][] - Add `a_util::ScopeGuard` class [\[view\]][2321ca13609]
  - Adds scope guards based on Andrei Alexandrescu's talk &quot;Declarative Control Flow&quot;
- [CPPDEV-306][] - Enable TCP_NODELAY in httplib for faster RPC [\[view\]][9ddd6bc6a9c]
- [CPPDEV-420][] - [ddl] Merge StructLayout (of Codec API) and StructTypeAccess [\[view\]][4122a427c3d]
- [CPPDEV-430][] - [Tests] Create automated memory leak tests 
- [CPPDEV-456][] - [filesystem] Refactor `a_util::filesystem::Path::getRoot()` to be consistent with std naming [\[view\]][89b9047a7f3]
  - Deprecates `a_util::filesystem::Path::getRoot()` and adds
    - `a_util::filesystem::Path::getRootName()` and
    - `a_util::filesystem::Path::getRootPath()`
- [CPPDEV-484][] - [build] Decouple conan and CMake [\[view\]][ca6bdd7fe6b] [\[view\]][62a40d231dd]
- [CPPDEV-493][] - Provide common error codes and types [\[view\]][bcc89848174]
  - Introduces `enum a_util::errc`
- [CPPDEV-541][] - [ddl] Extend type reflection API to support all optional DDL struct and element information [\[view\]][c33b96ea557]
  - Extends `ddl::DDStructure` with methods
    - `ddl::DDStructure::setStructInfo()`
    - `ddl::DDStructure::setElementInfo()`
    - `ddl::DDStructure::setElementUnit()`
- [CPPDEV-543][] - [memory] Add `a_util::memory::makeStackPtr()` function equivalent to `std::make_unique` [\[view\]][eb730959e6a]
- [CPPDEV-548][] - [ddl] Description files saved by the xml writer always follow the same pattern [\[view\]][58b9538330b]
- [CPPDEV-554][] - [xml] Add possibility to sort xml nodes [\[view\]][42bdfeb6dec]
  - Adds function overload `a_util::strings::replace()` and new methods
    `a_util::xml::DOMElement::sortNodes()`
- [CPPDEV-577][] - Add a small LeafCodecIndex to raise performance for default decoding [\[view\]][f52137f7d32] [\[view\]][30bcabca24f]
  - Introduces `ddl::codec::LeafCodecIndex` class for high performant ddl coder/decoder access
  - Introduces type traits file with the following function/types:
    - `std::is_enum_v` and `std::void_t` as std backports only for non C++17 compliant compilers
    - `a_util::underlying_type_or_type` and `a_util::underlying_type_or_type_t`
    - `a_util::is_explicitly_convertible_to` and `a_util::is_explicitly_convertible_to_v`

_**Won't Do**_
- [CPPDEV-5][] - Add Solution for old cFile \(AUL-70\)
- [CPPDEV-85][] - [concurrency] Implement method `semaphore::wait_until`
- [CPPDEV-107][] - Add child process control functions
- [CPPDEV-117][] - Provide solution for further requirements regarding `a_util::process`
- [CPPDEV-118][] - Array Support for As*-Methods of Variant
- [CPPDEV-128][] - Use `_nullptr` instead of `nullptr` in public headers
- [CPPDEV-134][] - Cleanup usage of defines and preprocessor macros
- [CPPDEV-146][] - [process] Create possibility to interact with child process \(stdIn\)
- [CPPDEV-264][] - [DOC] Methods, Variables, Classes need documentation
- [CPPDEV-387][] - [ddl] Add Functionality to define 'aliases' to predefined data types in \<datatypes/\> section
  - Superseded by [CPPDEV-426][]
- [CPPDEV-505][] - Try sending RPC with SendRPCMessage in a predefined IP first
- [CPPDEV-506][] - Add a thread safe queue

_**Duplicate**_
- [CPPDEV-173][] - [process] Add functionality to resolve environment variables
  - Duplicated by [CPPDEV-108][]
- [CPPDEV-249][] - Change to new coding style and use clang tidy to convert user code
  - Duplicates by [CPPDEV-214][]
- [CPPDEV-475][] - Redesign of the `ddl::access_element` API and CodecFactory internal data to increase creation performance
  - Duplicated by [CPPDEV-420][]
- [CPPDEV-502][] - Predefined error codes for cross-project use
  - Duplicated by [CPPDEV-493][]
- [CPPDEV-551][] - [ddl2header] Preserve ddl struct namespaces
  - Duplicated by [DDLUTILITY-111][]

<a name="dev_essential_1_2_0_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-135][] - [Result] Construction with 'ResultInfo<void>' doesn't call desired constructor [\[view\]][b444f606a47]
  - **Please note:** This change might lead to compile time errors when using `_MAKE_RESULT()`
    with a value of `0` in customer code. For hints how to fix/deactivate this behavior, consult
    the resulting compiler message.
- [CPPDEV-381][] - `ddl::getType()` does not return a meaningful error message [\[view\]][449e2e7553c]
- [CPPDEV-509][] - dev_essential cannot be built with clang 10.0.0 and mingw32 frontend [\[view\]][6976be5e94b]
- [CPPDEV-512][] - [ddl] compare for enums does not check for equivalence [\[view\]][3cd76c4adaf]
- [CPPDEV-571][] - [build] No SCA stage for develop branch executed [\[view\]][9170346bdac]
- [CPPDEV-575][] - threaded_http_server doesn't wait for detached threads on call to StopListening [\[view\]][a7cb41901f6]

_**Won't Do**_
- [CPPDEV-233][] - [HTTP Parser] The used http parser is too minimal
  - Fixed with [CPPDEV-250][]
- [CPPDEV-565][] - [ddl] TesterOODDL::checkTypeCalculationPerformance test fails for gcc5 and gcc7
  - Fixed with [CPPDEV-420][]

<a name="dev_essential_1_2_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.2.0%22">Known issues</a></h4>

<!--- Issue links -->
[DDLUTILITY-111]: https://devstack.vwgroup.com/jira/browse/DDLUTILITY-111
[CPPDEV-5]: https://devstack.vwgroup.com/jira/browse/CPPDEV-5
[CPPDEV-85]: https://devstack.vwgroup.com/jira/browse/CPPDEV-85
[CPPDEV-107]: https://devstack.vwgroup.com/jira/browse/CPPDEV-107
[CPPDEV-117]: https://devstack.vwgroup.com/jira/browse/CPPDEV-117
[CPPDEV-118]: https://devstack.vwgroup.com/jira/browse/CPPDEV-118
[CPPDEV-128]: https://devstack.vwgroup.com/jira/browse/CPPDEV-128
[CPPDEV-134]: https://devstack.vwgroup.com/jira/browse/CPPDEV-134
[CPPDEV-135]: https://devstack.vwgroup.com/jira/browse/CPPDEV-135
[CPPDEV-146]: https://devstack.vwgroup.com/jira/browse/CPPDEV-146
[CPPDEV-173]: https://devstack.vwgroup.com/jira/browse/CPPDEV-173
[CPPDEV-214]: https://devstack.vwgroup.com/jira/browse/CPPDEV-214
[CPPDEV-217]: https://devstack.vwgroup.com/jira/browse/CPPDEV-217
[CPPDEV-233]: https://devstack.vwgroup.com/jira/browse/CPPDEV-233
[CPPDEV-249]: https://devstack.vwgroup.com/jira/browse/CPPDEV-249
[CPPDEV-250]: https://devstack.vwgroup.com/jira/browse/CPPDEV-250
[CPPDEV-264]: https://devstack.vwgroup.com/jira/browse/CPPDEV-264
[CPPDEV-306]: https://devstack.vwgroup.com/jira/browse/CPPDEV-306
[CPPDEV-381]: https://devstack.vwgroup.com/jira/browse/CPPDEV-381
[CPPDEV-387]: https://devstack.vwgroup.com/jira/browse/CPPDEV-387
[CPPDEV-420]: https://devstack.vwgroup.com/jira/browse/CPPDEV-420
[CPPDEV-426]: https://devstack.vwgroup.com/jira/browse/CPPDEV-426
[CPPDEV-430]: https://devstack.vwgroup.com/jira/browse/CPPDEV-430
[CPPDEV-456]: https://devstack.vwgroup.com/jira/browse/CPPDEV-456
[CPPDEV-475]: https://devstack.vwgroup.com/jira/browse/CPPDEV-475
[CPPDEV-484]: https://devstack.vwgroup.com/jira/browse/CPPDEV-484
[CPPDEV-493]: https://devstack.vwgroup.com/jira/browse/CPPDEV-493
[CPPDEV-502]: https://devstack.vwgroup.com/jira/browse/CPPDEV-502
[CPPDEV-505]: https://devstack.vwgroup.com/jira/browse/CPPDEV-505
[CPPDEV-506]: https://devstack.vwgroup.com/jira/browse/CPPDEV-506
[CPPDEV-509]: https://devstack.vwgroup.com/jira/browse/CPPDEV-509
[CPPDEV-512]: https://devstack.vwgroup.com/jira/browse/CPPDEV-512
[CPPDEV-541]: https://devstack.vwgroup.com/jira/browse/CPPDEV-541
[CPPDEV-543]: https://devstack.vwgroup.com/jira/browse/CPPDEV-543
[CPPDEV-548]: https://devstack.vwgroup.com/jira/browse/CPPDEV-548
[CPPDEV-551]: https://devstack.vwgroup.com/jira/browse/CPPDEV-551
[CPPDEV-554]: https://devstack.vwgroup.com/jira/browse/CPPDEV-554
[CPPDEV-565]: https://devstack.vwgroup.com/jira/browse/CPPDEV-565
[CPPDEV-571]: https://devstack.vwgroup.com/jira/browse/CPPDEV-571
[CPPDEV-575]: https://devstack.vwgroup.com/jira/browse/CPPDEV-575
[CPPDEV-577]: https://devstack.vwgroup.com/jira/browse/CPPDEV-577

<!--- Commit links -->
[b444f606a47]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/b444f606a47e5efc941f95a433a75ffd69c41633
[449e2e7553c]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/449e2e7553c9d6227dbe688e895297e2062c14d0
[6976be5e94b]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/6976be5e94bda8a4b7825c9849b8d02b4e4d44d8
[3cd76c4adaf]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/3cd76c4adaffd3c83ad51ad036ca949d53b75590
[9170346bdac]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/9170346bdac4b8024b2fef4ef894d3249781cd50
[a7cb41901f6]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/a7cb41901f679aacf6f6750ef57e4747243c2355
[f4cd19d2705]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f4cd19d27058d3c16e2eb3c0ca1ef263ab41eb7f
[a40ec9e5d7c]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/a40ec9e5d7ce1de5c80eebea28796f452d2178d8
[2321ca13609]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/2321ca136093b2d2ca484e82c0225283bdd6f2d0
[9ddd6bc6a9c]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/9ddd6bc6a9c6ed3d2cfc614560b5c6bb40ebdd9a
[4122a427c3d]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/4122a427c3d19b7c5d1068bd8c86e6cff9ca0bcd
[89b9047a7f3]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/89b9047a7f3881dae9032e34c3c18dd79d98ca14
[ca6bdd7fe6b]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/ca6bdd7fe6b6a0108c39b4a26e31ae57cde7b6ec
[62a40d231dd]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/62a40d231dd10616f9d82b195144d68ac3172608
[c33b96ea557]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/c33b96ea557e1344512bc0a5767968a2d6ccccdc
[bcc89848174]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/bcc89848174fce42848fea07a36886dfd5f7ee04
[eb730959e6a]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/eb730959e6a883ba08ac52f3c552732c1ca118c6
[58b9538330b]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/58b9538330bf0dae7ef7b89147dae6e79c803ecc
[42bdfeb6dec]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/42bdfeb6dec41cb2721f9dbc174a25e4ac3510c3
[f52137f7d32]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f52137f7d32f9a499f5b899266d61cdec416573c
[30bcabca24f]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/30bcabca24ff35e3dac30fed753e2dd4cd5f56b8

<a name="dev_essential_1_1_4"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.1.4">dev_essential 1.1.4</a> - 2022/05/20</h3>

<a name="dev_essential_1_1_4_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-568][] - [ddl] TypeAccessList within 1.1.3 is not compatible with 1.1.2 [\[view\]][f28b0d7932e]

<a name="dev_essential_1_1_4_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.1.4%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-568]: https://devstack.vwgroup.com/jira/browse/CPPDEV-568

<!--- Commit links -->
[f28b0d7932e]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f28b0d7932e23b09e67dc1e497a3c14e8487c7c1

<a name="dev_essential_1_1_3"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.1.3">dev_essential 1.1.3</a> - 2022/05/13</h3>

<a name="dev_essential_1_1_3_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-539][] - [ddl] ElementTypeInfo will not be updated if add is used and has performance problem [\[view\]][95f6184c087]
- [CPPDEV-545][] - [Variant] Move construction and move assignment very slow [\[view\]][33a0843551d]
- [CPPDEV-550][] - [ddl] If a predefined type is used in enum the ValidationInfo will not be created

<a name="dev_essential_1_1_3_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.1.3%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-539]: https://devstack.vwgroup.com/jira/browse/CPPDEV-539
[CPPDEV-545]: https://devstack.vwgroup.com/jira/browse/CPPDEV-545
[CPPDEV-550]: https://devstack.vwgroup.com/jira/browse/CPPDEV-550

<!--- Commit links -->
[95f6184c087]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/95f6184c087206c8638b27df51ea9f42bf944c73
[33a0843551d]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/33a0843551d9adfcf2a20b7a9a92e06baf870494

<a name="dev_essential_1_1_2"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.1.2">dev_essential 1.1.2</a> - 2022/03/04</h3>

<a name="dev_essential_1_1_2_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-540][] - [ddl] Performance collapse due to DDFromXMLFactory::isInteger change [\[view\]][9974971d8ab]

<a name="dev_essential_1_1_2_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.1.2%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-540]: https://devstack.vwgroup.com/jira/browse/CPPDEV-540

<!--- Commit links -->
[9974971d8ab]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/9974971d8abc079463caf6e4755b8b061aa97b10

<a name="dev_essential_1_1_1"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.1.1">dev_essential 1.1.1</a> - 2022/01/21</h3>

<a name="dev_essential_1_1_1_changes"></a>
#### Changes
_**Won't Do**_
- [CPPDEV-203][] - [build] Evaluate dl linker option _--exclude-libs_ to hide symbols of linked static libraries
  - Fixed with [CPPDEV-515][]

<a name="dev_essential_1_1_1_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-312][] - [rpc] httplib crashes when it receives an https request [\[view\]][52f5939edb3]
- [CPPDEV-511][] - [ddl] Loading of description file leads to Segfault error on Linux [\[view\]][bdfb3e05bc6]
- [CPPDEV-515][] - [build] GCC default symbol visibility might lead to multiple defined symbols in shared libraries [\[view\]][8efa1a27975]
- [CPPDEV-517][] - [build] `CMAKE_SOURCE_DIR` points to wrong folder when integrating dev_essential into ConanWorkspace build [\[view\]][59e1593c8be]
- [CPPDEV-522][] - [ddl] static constexpr variables might violate the ODR if linked by C++17 built downstream project [\[view\]][ad797d7ccd9]
- [CPPDEV-523][] - [ddl] DDL documentation defective [\[view\]][6977f3f8ff3]
- [CPPDEV-530][] - [rpc] httpclient does not read full response body [\[view\]][a163c870001]
- [CPPDEV-532][] - [ddl] Static Singleton factory in header leads to problem in gcc 7 [\[view\]][2861f9a90db]

<a name="dev_essential_1_1_1_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.1.1%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-203]: https://devstack.vwgroup.com/jira/browse/CPPDEV-203
[CPPDEV-312]: https://devstack.vwgroup.com/jira/browse/CPPDEV-312
[CPPDEV-511]: https://devstack.vwgroup.com/jira/browse/CPPDEV-511
[CPPDEV-515]: https://devstack.vwgroup.com/jira/browse/CPPDEV-515
[CPPDEV-517]: https://devstack.vwgroup.com/jira/browse/CPPDEV-517
[CPPDEV-522]: https://devstack.vwgroup.com/jira/browse/CPPDEV-522
[CPPDEV-523]: https://devstack.vwgroup.com/jira/browse/CPPDEV-523
[CPPDEV-530]: https://devstack.vwgroup.com/jira/browse/CPPDEV-530
[CPPDEV-532]: https://devstack.vwgroup.com/jira/browse/CPPDEV-532

<!--- Commit links -->
[52f5939edb3]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/52f5939edb37dcc0d4ce5a7da291e95e6e4e5939
[8efa1a27975]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/8efa1a279757f2400cfcc5ff4939daee59f3268e
[59e1593c8be]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/59e1593c8be4c0003653a492187da088ee45ee02
[ad797d7ccd9]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/ad797d7ccd920d0116c3873e69482a9e6908a39d
[6977f3f8ff3]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/6977f3f8ff38a7864a66c228d21f2f157e696144
[bdfb3e05bc6]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/bdfb3e05bc6e8da7cc9d8760817ad2ac3bb4977d
[a163c870001]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/a163c87000176baaf149923df6e1dbcca09cb111
[2861f9a90db]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/2861f9a90dbf942dbd85a0c91fba1f2f2327fa0f

<a name="dev_essential_1_1_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.1.0">dev_essential 1.1.0</a> - 2021/10/14</h3>

<a name="dev_essential_1_1_0_changes"></a>
#### Changes
_**Done**_
- [CPPDEV-460][] - [ddl_capi] Move DDL C-API to fep dev_essential
  - See [CPPDEV-476][]
- [CPPDEV-465][] - [build] Comply to C\+\+ Core Guidelines [SF.12][] rule [\[view\]][6d1dfbdac60]
- [CPPDEV-474][] - [ddl] DDL TypeReflection - Extend detection of missing elements by adding padding bytes automatically [\[view\]][373f6582788]
- [CPPDEV-476][] - [ddl_capi] Integrate ddl-c-api into top level directory of a_util [\[view\]][83280e9efcc]
- [CPPDEV-481][] - [build] Provide conditional clang-format SCA tester in CI [\[view\]][cf594afbf9f]
- [CPPDEV-483][] - [build] Add baseline check to Jenkinsfile [\[view\]][8718cd9bf16]
- [CPPDEV-485][] - [memory] Support alignment specification for `a_util::memory::StackPtr` [\[view\]][ea46c3f12bd]
- [CPPDEV-488][] - [ddl] Possibility to create a description only with pre-defined data types [\[view\]][67b76f293f0]
- [CPPDEV-489][] - [build] Provide single source of truth for product version [\[view\]][b28a734fb92]
- [CPPDEV-491][] - [ddl] DDL Mapping Engine does not forward the correct time while using "data" or "signal" as trigger type [\[view\]][e17b65d147f]
- [CPPDEV-492][] - [regex] Use `std::regex` for implementation of `a_util::regex` functionality [\[view\]][136da05b3a8]
- [CPPDEV-498][] - [ddl] DDL TypeReflection - Add a dedicated validate function to the `DDStructureGenerator` [\[view\]][373f6582788]

_**Won't Do**_
- [CPPDEV-380][] - [ddl] Remove DDLCompareFunctor and use lambdas where necessary
  - File deleted in [2d5bd295a87][] in issue [CPPDEV-431][]
- [CPPDEV-382][] - [ddl] Add missing documentation to header file
  - DDLImporter has been deleted with commit [2d5bd295a87][] in [CPPDEV-431][]
- [CPPDEV-401][] - [ddl] Cleanup copy and move semantics
  - Superseded by issue [CPPDEV-431][] which removed almost all classes in question
- [CPPDEV-403][] - [header2ddl] Use abstract syntax tree to generate description file
  - Superseded by [DDLUTILITY-101][]

_**Duplicate**_
- [CPPDEV-429][] - [build] Extend Conan Pkg Info in ddl
  - DDL integrated into dev_essential, so it will be done together with [CPPDEV-190][]

<a name="dev_essential_1_1_0_fixes"></a>
#### Fixes
_**Fixed**_
- [CPPDEV-453][] - [concurrency] _a\_util/concurency.h_ brings windows includes in scope and destroys APIs [\[view\]][3d4bdb79210] [\[view\]][0b449eaffa8]
- [CPPDEV-458][] - [system] Leftover ADTF include emits compiler error under `__APPLE__` [\[view\]][fb2a088917b]
- [CPPDEV-480][] - [build] Self-contained header tester takes a long time to complete [\[view\]][c0252fe2ba7] [\[view\]][a4715ea42dd]
- [CPPDEV-486][] - [ddl] Typo in _dd_predefined_datatypes.h_ prevents creating DataType int8_t [\[view\]][9f2285521a4] [\[view\]][e8781cc14b8]
- [CPPDEV-487][] - [result] nativs file only installed for MSVC [\[view\]][f0085fe1176]
- [CPPDEV-496][] - [xml] pugixml in a_util_xml causes linker problems when using a pugixml in a derivated work [\[view\]][81ccdb3bbe4]
- [CPPDEV-497][] - [ddl] The CTOR with elements of `DDEnumGenerator<T>` has a problem if type is not arithmetic [\[view\]][370f6e0613d]
- [CPPDEV-500][] - [build] Compiler error when building with gcc >= 8.x [\[view\]][e405f89e75d]

_**Won't Do**_
- [CPPDEV-343][] - [ddlinspector] Duplicated types if renamed and merged
  - Fixed with redesign of OO DDL in issue [CPPDEV-431][]

_**Cannot Reproduce**_
- [CPPDEV-490][] - [ddl] `ddl::dd::StructType::Element::getNumBits()` returns zero for `DataTypes`, also if size is set in ddl file

<a name="dev_essential_1_1_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.1.0%22">Known issues</a></h4>

<!--- Issue links -->
[DDLUTILITY-101]: https://devstack.vwgroup.com/jira/browse/DDLUTILITY-101
[CPPDEV-190]: https://devstack.vwgroup.com/jira/browse/CPPDEV-190
[CPPDEV-343]: https://devstack.vwgroup.com/jira/browse/CPPDEV-343
[CPPDEV-380]: https://devstack.vwgroup.com/jira/browse/CPPDEV-380
[CPPDEV-382]: https://devstack.vwgroup.com/jira/browse/CPPDEV-382
[CPPDEV-401]: https://devstack.vwgroup.com/jira/browse/CPPDEV-401
[CPPDEV-403]: https://devstack.vwgroup.com/jira/browse/CPPDEV-403
[CPPDEV-429]: https://devstack.vwgroup.com/jira/browse/CPPDEV-429
[CPPDEV-453]: https://devstack.vwgroup.com/jira/browse/CPPDEV-453
[CPPDEV-458]: https://devstack.vwgroup.com/jira/browse/CPPDEV-458
[CPPDEV-460]: https://devstack.vwgroup.com/jira/browse/CPPDEV-460
[CPPDEV-465]: https://devstack.vwgroup.com/jira/browse/CPPDEV-465
[CPPDEV-474]: https://devstack.vwgroup.com/jira/browse/CPPDEV-474
[CPPDEV-476]: https://devstack.vwgroup.com/jira/browse/CPPDEV-476
[CPPDEV-480]: https://devstack.vwgroup.com/jira/browse/CPPDEV-480
[CPPDEV-481]: https://devstack.vwgroup.com/jira/browse/CPPDEV-481
[CPPDEV-482]: https://devstack.vwgroup.com/jira/browse/CPPDEV-482
[CPPDEV-483]: https://devstack.vwgroup.com/jira/browse/CPPDEV-483
[CPPDEV-485]: https://devstack.vwgroup.com/jira/browse/CPPDEV-485
[CPPDEV-486]: https://devstack.vwgroup.com/jira/browse/CPPDEV-486
[CPPDEV-487]: https://devstack.vwgroup.com/jira/browse/CPPDEV-487
[CPPDEV-488]: https://devstack.vwgroup.com/jira/browse/CPPDEV-488
[CPPDEV-489]: https://devstack.vwgroup.com/jira/browse/CPPDEV-489
[CPPDEV-490]: https://devstack.vwgroup.com/jira/browse/CPPDEV-490
[CPPDEV-491]: https://devstack.vwgroup.com/jira/browse/CPPDEV-491
[CPPDEV-492]: https://devstack.vwgroup.com/jira/browse/CPPDEV-492
[CPPDEV-494]: https://devstack.vwgroup.com/jira/browse/CPPDEV-494
[CPPDEV-496]: https://devstack.vwgroup.com/jira/browse/CPPDEV-496
[CPPDEV-497]: https://devstack.vwgroup.com/jira/browse/CPPDEV-497
[CPPDEV-498]: https://devstack.vwgroup.com/jira/browse/CPPDEV-498
[CPPDEV-500]: https://devstack.vwgroup.com/jira/browse/CPPDEV-500

<!--- Commit links -->
[6d1dfbdac60]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/6d1dfbdac60871fb83ad728a462dc9983dec81c8
[373f6582788]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/373f6582788147cf81a5142fa60e13ab5aabab60
[83280e9efcc]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/83280e9efcc8c6804208c2683cf1d8631bbbd7f5
[cf594afbf9f]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/cf594afbf9fd3a32a6bdf894abdc82489933185c
[8718cd9bf16]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/8718cd9bf16a9e2e26ada3f789c698309d279873
[ea46c3f12bd]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/ea46c3f12bd681b2c4f73bae7d7f26643a148b34
[b28a734fb92]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/b28a734fb92ad30f8ab825e97bcaf0e7849de04d
[e17b65d147f]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/e17b65d147f4866b413b00b1a051bc00e5691795
[136da05b3a8]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/136da05b3a8568f939d7b25846894b114a093e14
[3d4bdb79210]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/3d4bdb79210a5a750973e996c8eee097f651e7fe
[0b449eaffa8]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/0b449eaffa80d548d8bd63dcd556cf9a09b0c15b
[fb2a088917b]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/fb2a088917bf3397945036c02dc03e53e7b19ee2
[c0252fe2ba7]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/c0252fe2ba7c9fd61c32b090d8cb42b1f6ce53b5
[a4715ea42dd]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/a4715ea42dd3515bccb5621e2a7474667f7b211f
[9f2285521a4]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/9f2285521a4f186dde4eb3c2a0af3809062d2bf4
[e8781cc14b8]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/e8781cc14b876e08da6bfe26f394407142381f13
[f0085fe1176]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/f0085fe1176eddac8fc6d4290da90744156ca0ff
[81ccdb3bbe4]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/81ccdb3bbe4056d0a0045442e319e63768826efc
[370f6e0613d]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/370f6e0613d66acc777d03c130bc0a6c664d8389
[e405f89e75d]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/e405f89e75d092f06093543fe6b41f276aa105c5
[67b76f293f0]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/67b76f293f09c74a11f91ea4132c22dc3e88bb6f

<a name="dev_essential_1_0_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fdev_essential_v1.0.0">dev_essential 1.0.0</a> - 2021/08/04</h3>
> _**This release is the successor of a_util, ddl library and pkg_rpc, merging all three libraries
> into one repository.**_

<a name="dev_essential_1_0_0_changes"></a>
#### Changes
- [CPPDEV-79](https://devstack.vwgroup.com/jira/browse/CPPDEV-79) -
  [Result] Enable usage of error description objects with static storage
- [CPPDEV-270][] -
  Enable position independent code and position independent executables
- [CPPDEV-301](https://devstack.vwgroup.com/jira/browse/CPPDEV-301) -
  [build] Elevate compiler warning levels for all util components
- [CPPDEV-302](https://devstack.vwgroup.com/jira/browse/CPPDEV-302) -
  [build] Fix all compiler warnings and set compiler warnings as errors
- [CPPDEV-313](https://devstack.vwgroup.com/jira/browse/CPPDEV-313) -
  Integrate cdpkgrpc master and tags into a_util repository
- [CPPDEV-314](https://devstack.vwgroup.com/jira/browse/CPPDEV-314) -
  Integrate cdddl master and tags into a_util repository
- [CPPDEV-385](https://devstack.vwgroup.com/jira/browse/CPPDEV-385) -
  Move or copy DDL description generation code from ADTF 3 to DDL Library
- [CPPDEV-405](https://devstack.vwgroup.com/jira/browse/CPPDEV-405) -
  Add type reflection API to the DDL Library
- [CPPDEV-423](https://devstack.vwgroup.com/jira/browse/CPPDEV-423) -
  Improve XML Data Model and DDL Parser
- [CPPDEV-431][] -
  Redesign OO DDL Description API (Data Model + API)
  - **Breaking change:** The deprecated `ddl::DDLDescription` API has been replaced with the
    [DDL DataDefinition](ddl/dd.md). Customer code using the deprecated `ddl::DDLDescription` needs
    to be rewritten accordingly.
- [CPPDEV-451](https://devstack.vwgroup.com/jira/browse/CPPDEV-451) -
  Integrate cdpkgrpc into top level directory of a_util
- [CPPDEV-459](https://devstack.vwgroup.com/jira/browse/CPPDEV-459) -
  Provide ddl, pkg_rpc and a_util as dev_essential package version 1.0.0
- [CPPDEV-461](https://devstack.vwgroup.com/jira/browse/CPPDEV-461) -
  Integrate cdddl into top level directory of a_util
- [CPPDEV-462](https://devstack.vwgroup.com/jira/browse/CPPDEV-462) -
  Update license disclaimers to MPL2.0 and VW Group copyright
- [CPPDEV-463](https://devstack.vwgroup.com/jira/browse/CPPDEV-463) -
  [BuildSystem] Add Linux_x64_gcc7, Linux_armv8_gcc7, and Windows_x64_vc142_VS2019 profile
- [CPPDEV-468](https://devstack.vwgroup.com/jira/browse/CPPDEV-468) -
  [BuildSystem] Provide Release builds for additional platforms
- [CPPDEV-471](https://devstack.vwgroup.com/jira/browse/CPPDEV-471) -
  [BuildSystem] Provide updated build information in README.md
- [CPPDEV-472](https://devstack.vwgroup.com/jira/browse/CPPDEV-472) - Remove ddl2header and header2ddl
  - The successors of those tools will be available as part of the DDL Utilities. See
    [here](https://devstack.vwgroup.com/jira/browse/DDLUTILITY-100) and
    [here](https://devstack.vwgroup.com/jira/browse/DDLUTILITY-101) for details.

<a name="dev_essential_1_0_0_fixes"></a>
#### Fixes
- [CPPDEV-454](https://devstack.vwgroup.com/jira/browse/CPPDEV-454) -
  [filesystem] `a_util::filesytem::Path::makeRelative()` fails if any path contains ".."
- [CPPDEV-464](https://devstack.vwgroup.com/jira/browse/CPPDEV-464) -
  remove / access for DDL internal datastructures type_access_list, type_access_map are not working
  correctly
- [CPPDEV-467](https://devstack.vwgroup.com/jira/browse/CPPDEV-467) -
  If removing a struct_type which is used in another type the validation model will crash
- [CPPDEV-470](https://devstack.vwgroup.com/jira/browse/CPPDEV-470) -
  Mapping Configuration for invalid file will crash
- [CPPDEV-473](https://devstack.vwgroup.com/jira/browse/CPPDEV-473) -
  Codec: serialized and deserialized offsets of structures in array elements are not evaluated right

<a name="dev_essential_1_0_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22dev_essential%201.0.0%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-431]: https://devstack.vwgroup.com/jira/browse/CPPDEV-431
[CPPDEV-270]: https://devstack.vwgroup.com/jira/browse/CPPDEV-270

<!--- Commit links -->
[2d5bd295a87]: https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/commits/2d5bd295a8702f3b104c2052261234d8fadc47c8

<a name="utility_library"></a>
## Utility library

<a name="a_util_5_8_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.8.0">a_util 5.8.0</a> - 2021/02/17</h3>
> _**Important note for users updating their downstream projects to this version:**_
>
> Some of the implementation details of component _Result_ had to be made public and are part of the
> users code now. This results in a change of exported symbols in the delivered binary. To prevent
> linker errors due to missing symbols recompiling and relinking against the a\_util 5.8.0 is
> necessary.

<a name="a_util_5_8_0_changes"></a>
#### Changes
- [CPPDEV-125](https://devstack.vwgroup.com/jira/browse/CPPDEV-125) -
  Provide component `a_util::result` header-only and deploy as single library
  - Starting from this release, the _Result_ component can be built and deployed as own header-only
    package. To require the package, conan reference `a_util_result/5.8.0@fep/stable` can be used.
    Changes to `find_package(a_util ...)` calls are not required.
- [CPPDEV-175](https://devstack.vwgroup.com/jira/browse/CPPDEV-175) -
  [memory] Add support for `std::make_unique()` for pre C\+\+14 compilers
  - This introduces backport implementations `a_util::memory::make_unique()`
- [CPPDEV-188](https://devstack.vwgroup.com/jira/browse/CPPDEV-188) -
  [build] Use native GTest and CMake functionality
- [CPPDEV-191](https://devstack.vwgroup.com/jira/browse/CPPDEV-191) -
  [filesystem] Compare content of two files
  - This introduces new functions `a_util::filesystem::compareFiles()`
- [CPPDEV-196](https://devstack.vwgroup.com/jira/browse/CPPDEV-196) -
  Use user "fep" instead of "aev25" in package references of the conan recipes
  - Starting from this release, a_util packages can be required with the conan reference
    `a_util/<version>@fep/<channel>`, e.g. `a_util/5.8.0@fep/stable`.
- [CPPDEV-199](https://devstack.vwgroup.com/jira/browse/CPPDEV-199) -
  Add _.clang-format_ file and apply ruleset on C\+\+ codebase
- [CPPDEV-200](https://devstack.vwgroup.com/jira/browse/CPPDEV-200) -
  Add preprocessor component to a_util library
  - This adds the new component `a_util::preprocessor` for basic preprocessor functionality
- [CPPDEV-205](https://devstack.vwgroup.com/jira/browse/CPPDEV-205) -
  [build] Provide packages for missing new profiles
- [CPPDEV-206](https://devstack.vwgroup.com/jira/browse/CPPDEV-206) -
  [build] Define and apply whitespace settings in _.gitattributes_ file
- [CPPDEV-208](https://devstack.vwgroup.com/jira/browse/CPPDEV-208) -
  [build] Generate API documentation using CoRTEXDocumentationHelper with doxygen
- [CPPDEV-219](https://devstack.vwgroup.com/jira/browse/CPPDEV-219) -
  [build] License information of dependencies must be part of the package
- [CPPDEV-300](https://devstack.vwgroup.com/jira/browse/CPPDEV-300) -
  [documentation] Provide easier access and better overview for API documentation

<a name="a_util_5_8_0_fixes"></a>
#### Fixes
- [CPPDEV-122](https://devstack.vwgroup.com/jira/browse/CPPDEV-122) -
  [variant] Copy Constructor does not correctly copy array values
- [CPPDEV-174](https://devstack.vwgroup.com/jira/browse/CPPDEV-174) -
  Product name in wiki does not match delivery
- [CPPDEV-181](https://devstack.vwgroup.com/jira/browse/CPPDEV-181) -
  [cmake] Redundant include directories and link libraries defined by targets
- [CPPDEV-204](https://devstack.vwgroup.com/jira/browse/CPPDEV-204) -
  [result] Functional test "result_type_from_lib_test" runs the wrong executable
- [CPPDEV-207](https://devstack.vwgroup.com/jira/browse/CPPDEV-207) -
  [build] Missing includes for VS2019 vc142
- [CPPDEV-209](https://devstack.vwgroup.com/jira/browse/CPPDEV-209) -
  [filesystem] `a_util::filesystem::Path::makeRelative()` fails for Root
- [CPPDEV-211](https://devstack.vwgroup.com/jira/browse/CPPDEV-211) -
  [build] Conan recipes not compatible with conan versions \>1.16.2
- [CPPDEV-212](https://devstack.vwgroup.com/jira/browse/CPPDEV-212) -
  [result] Exported CMake config does not work for either 32 bit or 64 bit architectures
- [CPPDEV-215](https://devstack.vwgroup.com/jira/browse/CPPDEV-215) -
  [build] Packaged CMake target 'a\_util' not part of export set

<a name="a_util_5_8_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.8.0%22">Known issues</a></h4>

<a name="a_util_5_7_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.7.0">a_util 5.7.0</a> - 2020/05/20</h3>
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
- [CPPDEV-76](https://devstack.vwgroup.com/jira/browse/CPPDEV-76) -
  [filesystem] Provide means to convert absolute to relative paths and vice versa
- [CPPDEV-93](https://devstack.vwgroup.com/jira/browse/CPPDEV-93) -
  [system] Get path to executable in currently running process
- [CPPDEV-124](https://devstack.vwgroup.com/jira/browse/CPPDEV-124) -
  [filesystem] Appending arbitrary strings to basenames of paths
- [CPPDEV-138](https://devstack.vwgroup.com/jira/browse/CPPDEV-138) -
  [filesystem] `operator<<` required for `a_util::filesystem::Path`
- [CPPDEV-161](https://devstack.vwgroup.com/jira/browse/CPPDEV-161) -
  [build] Generate documentation using conan
- [CPPDEV-176](https://devstack.vwgroup.com/jira/browse/CPPDEV-176) -
  [build] Use the newly defined profiles as base for the delivery packages
- [CPPDEV-182](https://devstack.vwgroup.com/jira/browse/CPPDEV-182) -
  [build] All public header files need to be self contained
- [CPPDEV-183](https://devstack.vwgroup.com/jira/browse/CPPDEV-183) -
  [documentation] Provide thorough documentation for the public API

<a name="a_util_5_7_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.7.0%22">Known issues</a></h4>

<a name="a_util_5_6_1"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.6.1">a_util 5.6.1</a> - 2020/02/12</h3>

<a name="a_util_5_6_1_changes"></a>
#### Changes
- [CPPDEV-163](https://devstack.vwgroup.com/jira/browse/CPPDEV-163) -
  Extend Jenkinsfile for cross platform build of QNX target on Windows host

<a name="a_util_5_6_1_fixes"></a>
#### Fixes
- [CPPDEV-105](https://devstack.vwgroup.com/jira/browse/CPPDEV-105) -
  [Result] Warning "unreferenced formal parameter" arises when including result\_type\_impl.h
- [CPPDEV-157](https://devstack.vwgroup.com/jira/browse/CPPDEV-157) -
  [BuildSystem] Language standard C\+\+11 is not propagated
- [CPPDEV-168](https://devstack.vwgroup.com/jira/browse/CPPDEV-168) -
  [Result] Macros cannot be used with a closing semicolon without compiler warning
- [CPPDEV-169](https://devstack.vwgroup.com/jira/browse/CPPDEV-169) -
  Function findNode of dom.h raises `-Werror=comment` warning on gcc

<a name="a_util_5_6_1_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.6.1%22">Known issues</a></h4>

<a name="a_util_5_6_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.6.0">a_util 5.6.0</a> - 2019/09/06</h3>

<a name="a_util_5_6_0_changes"></a>
#### Changes
- [CPPDEV-139](https://devstack.vwgroup.com/jira/browse/CPPDEV-139) -
  Implement new license mechanism in a_util
- [CPPDEV-150](https://devstack.vwgroup.com/jira/browse/CPPDEV-150) -
  Add Conan recipe and Jenkinsfile to support Jenkins Multibranch Pipeline jobs
- [CPPDEV-155](https://devstack.vwgroup.com/jira/browse/CPPDEV-155) -
  [BuildSystem] Change names of delivered library (\*.lib and \*.a) files

<a name="a_util_5_6_0_fixes"></a>
#### Fixes
- [CPPDEV-142](https://devstack.vwgroup.com/jira/browse/CPPDEV-142) -
  [memory] new allocated buffer is not nullified
- [CPPDEV-149](https://devstack.vwgroup.com/jira/browse/CPPDEV-149) -
  [system] `getCurrentUserName()` doesn't work correctly when running on QNX
- [CPPDEV-160](https://devstack.vwgroup.com/jira/browse/CPPDEV-160) -
  [log] log.h file is not self contained

<a name="a_util_5_6_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.6.0%22">Known issues</a></h4>

<a name="a_util_5_5_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.5.0">a_util 5.5.0</a> - 2019/04/12</h3>

<a name="a_util_5_5_0_changes"></a>
#### Changes
- [CPPDEV-119](https://devstack.vwgroup.com/jira/browse/CPPDEV-119) - Porting to SCALEXIO QNX
- [CPPDEV-121](https://devstack.vwgroup.com/jira/browse/CPPDEV-121) - Remove C\+\+11 compatibility code
  - **Breaking change:** Compilers without C\+\+11 support are not supported anymore. Please check
    the supported compiler section in the documentation.

#### Tasks
- [CPPDEV-123](https://devstack.vwgroup.com/jira/browse/CPPDEV-123) -
  Include the Open Source Disclaimer to the package
- [CPPDEV-131](https://devstack.vwgroup.com/jira/browse/CPPDEV-131) -
  Add install complete reference files

<a name="a_util_5_5_0_fixes"></a>
#### Fixes
- [CPPDEV-126](https://devstack.vwgroup.com/jira/browse/CPPDEV-126) -
  Use of plain signature of `target_link_libraries` on test targets leads to CMake error during
  configuration
- [CPPDEV-129](https://devstack.vwgroup.com/jira/browse/CPPDEV-129) -
  [Changelog] "Known Issues" use the wrong query
- [CPPDEV-133](https://devstack.vwgroup.com/jira/browse/CPPDEV-133) -
  Usage of gtest and gtest_main instead of `GTest::GTest` and `GTest::Main` as link libraries leads
  to linker errors

<a name="a_util_5_5_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.5.0%22">Known issues</a></h4>

<a name="a_util_5_4_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.4.0">a_util 5.4.0</a> - 2018/11/07</h3>

<a name="a_util_5_4_0_changes"></a>
#### Changes
- [CPPDEV-115](https://devstack.vwgroup.com/jira/browse/CPPDEV-115) -
  [common] Remove ADPL and add MPL 2.0 License
  - since the ADPL license is NOT compatible with the terms and conditions of the GPLv3 we will now
    distribute the A Util library under the common MPL 2.0 (Mozilla Public License 2.0). This
    will allow to reuse the library under GPLv3.

<a name="a_util_5_4_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.4.0%22">Known issues</a></h4>

<a name="a_util_5_3_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.3.0">a_util 5.3.0</a> - 2018/09/21</h3>

<a name="a_util_5_3_0_changes"></a>
#### Changes
- [CPPDEV-94](https://devstack.vwgroup.com/jira/browse/CPPDEV-94) -
  [parser] Add basic functionality to parse comma separated value (CSV) files
  - New component `a_util::parser` added with initial implementation to read comma separated value
    (\*.csv) files
  - **Usage:**
    - `find_package(a_util COMPONENTS csv_reader)` includes the csv reader into the current project
    - `target_link_libraries(myProject a_util_csv_reader)` might be used to link against the
      library
    - <tt>\#include "a_util/parser.h"</tt> includes the parser API into your current source file
- [CPPDEV-103](https://devstack.vwgroup.com/jira/browse/CPPDEV-103) -
  [process] Add a component for process control
  - New component `a_util::process` added with initial implementations to run executables within a
    new process as well as retrieve the current process id
  - **Usage:**
    - `find_package(a_util COMPONENTS process)` includes the process component into the current
      project
    - `target_link_libraries(myProject a_util_process)` might be used to link against the
      library
    - <tt>\#include "a_util/process.h"</tt> includes the process API into your current source file
- [CPPDEV-108][] -
  [process] Add reading and writing environment variables to the process
- [CPPDEV-109](https://devstack.vwgroup.com/jira/browse/CPPDEV-109) -
  Changelog must provide more and better readable information

<a name="a_util_5_3_0_fixes"></a>
#### Fixes
- [CPPDEV-99](https://devstack.vwgroup.com/jira/browse/CPPDEV-99) -
  Make odautil compiles out of the box
- [CPPDEV-101](https://devstack.vwgroup.com/jira/browse/CPPDEV-101) -
  [CMAKE] Could not call `find_package(a_util COMPONENTS ...)` more than one time
  - **Important note:** This encourages the usage of CMake package components to include the wanted
    a\_util components into projects. So instead of writing `find_package(a_util_strings)` we
    encourage you to write `find_package(a_util COMPONENTS strings)`

<a name="a_util_5_3_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.3.0%22">Known issues</a></h4>

<!--- Issue links -->
[CPPDEV-108]: https://devstack.vwgroup.com/jira/browse/CPPDEV-108

<a name="a_util_5_2_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.2.0">a_util 5.2.0</a> - 2018/03/30</h3>

<a name="a_util_5_2_0_changes"></a>
#### Changes
- [CPPDEV-3](https://devstack.vwgroup.com/jira/browse/CPPDEV-3) -
  Add Solution for old cCommandLine (AUL-69)
  - [Clara](https://github.com/catchorg/Clara/tree/v1.1.2)
    v1.1.2 was added as 3rdparty command line tool (C\+\+11 and above required)
  - **Usage:** After a successful `find_package(a_util)` consuming projects may first call
    `find_package(Clara)` followed by a `target_link_libraries(myLibrary Clara)`.
    Afterwards, Clara can be used as documented on its
    [project page](https://github.com/catchorg/Clara/tree/v1.1.2)
- [CPPDEV-77](https://devstack.vwgroup.com/jira/browse/CPPDEV-77) -
  [DateTime] Timestamps must be generated with reference to Julian Time or Unix Time
  - `enum TimeStampReference` may be used to choose the wanted behavior in methods `DateTime::set`
    and `DateTime::toTimestamp`
- [CPPDEV-95](https://devstack.vwgroup.com/jira/browse/CPPDEV-95) -
  Use googletest functionality provided by CMake command `find_package(GTest)`
- [CPPDEV-98](https://devstack.vwgroup.com/jira/browse/CPPDEV-98) -
  Add changelog file and initially start documenting changes for version 5.2.0

<a name="a_util_5_2_0_fixes"></a>
#### Fixes
- [CPPDEV-81](https://devstack.vwgroup.com/jira/browse/CPPDEV-81) -
  `Semaphore::wait_for` loops forever if timeout is too big
- [CPPDEV-90](https://devstack.vwgroup.com/jira/browse/CPPDEV-90) -
  [DateTime] Scoped enums and `std::chrono` don't compile with pre-C\+\+11 gcc compilers
- [CPPDEV-97](https://devstack.vwgroup.com/jira/browse/CPPDEV-97) -
  [base] CMake `configure_file()` must not generate config.h into source tree

<a name="a_util_5_2_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.2.0%22">Known issues</a></h4>

<a name="a_util_5_1_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.1.0">a_util 5.1.0</a> - 2017/12/08</h3>

<a name="a_util_5_1_0_changes"></a>
#### Changes
- [CPPDEV-61](https://devstack.vwgroup.com/jira/browse/CPPDEV-61) -
  [Result] Add debug helper for Visual Studio 2012\+ (.natvis file) (AUL-65)
- [CPPDEV-68](https://devstack.vwgroup.com/jira/browse/CPPDEV-68) -
  Remove googletest as 3rdparty dependency (AUL-68)
- [CPPDEV-72](https://devstack.vwgroup.com/jira/browse/CPPDEV-72) -
  Add ADPL License Info to all artefact (Header, CPPs)
- [CPPDEV-73](https://devstack.vwgroup.com/jira/browse/CPPDEV-73) -
  Automatically set C\+\+ standard to the platform maximum
- [CPPDEV-75](https://devstack.vwgroup.com/jira/browse/CPPDEV-75) -
  Add try\_wait method to `a_util::concurrency::semaphore` class

<a name="a_util_5_1_0_fixes"></a>
#### Fixes
- [CPPDEV-10](https://devstack.vwgroup.com/jira/browse/CPPDEV-10) -
  Thread detach() test can fail due to race condition (AUL-67)
- [CPPDEV-66](https://devstack.vwgroup.com/jira/browse/CPPDEV-66) -
  StackPtr Test crashes on U1604\_x64\_gcc54 (AUL-66)

<a name="a_util_5_1_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.1.0%22">Known issues</a></h4>

<a name="a_util_5_0_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fv5.0.0">a_util 5.0.0</a> - 2017/09/01</h3>

<a name="a_util_5_0_0_changes"></a>
#### Changes
- [CPPDEV-12](https://devstack.vwgroup.com/jira/browse/CPPDEV-12) -
  Fix Timer by rewriting Thread (AUL-63)
- [CPPDEV-14](https://devstack.vwgroup.com/jira/browse/CPPDEV-14) -
  [XML] Provide functionality to check for empty DOM and DOMElement objects (AUL-61)
- [CPPDEV-15](https://devstack.vwgroup.com/jira/browse/CPPDEV-15) -
  Split API and implementation of header only libraries into separate files (AUL-60)
- [CPPDEV-16](https://devstack.vwgroup.com/jira/browse/CPPDEV-16) -
  Apply style guidelines (AUL-57)
- [CPPDEV-17](https://devstack.vwgroup.com/jira/browse/CPPDEV-17) -
  [Base] Provide static_assert() backport (AUL-56)
- [CPPDEV-20](https://devstack.vwgroup.com/jira/browse/CPPDEV-20) -
  Test result output path must be configurable in integrated build scenario (AUL-53)
- [CPPDEV-21](https://devstack.vwgroup.com/jira/browse/CPPDEV-21) -
  Adapt API to updated style guidelines (AUL-51)
- [CPPDEV-22](https://devstack.vwgroup.com/jira/browse/CPPDEV-22) -
  Improve DOM FindNodes performance (AUL-50)
- [CPPDEV-26](https://devstack.vwgroup.com/jira/browse/CPPDEV-26) -
  Create a_util_base component (AUL-46)
- [CPPDEV-29](https://devstack.vwgroup.com/jira/browse/CPPDEV-29) -
  Provide interface target pulling in all AUL components at once (AUL-42)
- [CPPDEV-32](https://devstack.vwgroup.com/jira/browse/CPPDEV-32) -
  Provide mechanism to generate GUIDs (AUL-38)
- [CPPDEV-33](https://devstack.vwgroup.com/jira/browse/CPPDEV-33) -
  Provide support for high resolution timing on all platforms (AUL-37)
- [CPPDEV-34](https://devstack.vwgroup.com/jira/browse/CPPDEV-34) -
  Provide access to hostname (AUL-36)
- [CPPDEV-35](https://devstack.vwgroup.com/jira/browse/CPPDEV-35) -
  Disallow deleted CCTORS and assignment operators in concurrency fallback classes (AUL-35)
- [CPPDEV-37](https://devstack.vwgroup.com/jira/browse/CPPDEV-37) -
  [Base] Implement nullptr fallback for compilers not providing an implementation (AUL-33)
- [CPPDEV-38](https://devstack.vwgroup.com/jira/browse/CPPDEV-38) -
  Add basic semaphore implementation (AUL-32)
- [CPPDEV-40](https://devstack.vwgroup.com/jira/browse/CPPDEV-40) -
  Add missing string functionality (AUL-30)
- [CPPDEV-41](https://devstack.vwgroup.com/jira/browse/CPPDEV-41) -
  [Result] Port tResult and error handling mechanism from a_util 4 (AUL-29)
- [CPPDEV-42](https://devstack.vwgroup.com/jira/browse/CPPDEV-42) -
  `a_util::memory::MemoryBuffer` must support wrapping non-owned memory (AUL-28)
- [CPPDEV-43](https://devstack.vwgroup.com/jira/browse/CPPDEV-43) -
  `a_util::filesystem` has to have a rename or move function (AUL-27)
- [CPPDEV-44](https://devstack.vwgroup.com/jira/browse/CPPDEV-44) -
  extend `a_util::memory` for a mem_zero (AUL-26)
- [CPPDEV-45](https://devstack.vwgroup.com/jira/browse/CPPDEV-45) -
  Add copy functionality to `a_util::strings` (AUL-25)
- [CPPDEV-46](https://devstack.vwgroup.com/jira/browse/CPPDEV-46) -
  [System] Provide periodic timer - {Redmine #38883} (AUL-24)
- [CPPDEV-47](https://devstack.vwgroup.com/jira/browse/CPPDEV-47) -
  [System] Provide access to system API - {Redmine #37454} (AUL-22)
- [CPPDEV-48](https://devstack.vwgroup.com/jira/browse/CPPDEV-48) -
  [Filesystem] Provide support for filesytem access - {Redmine #37451} (AUL-20)
- [CPPDEV-49](https://devstack.vwgroup.com/jira/browse/CPPDEV-49) -
  [Chrono] Provide date and time functionality - {Redmine #37446} (AUL-19)
- [CPPDEV-50](https://devstack.vwgroup.com/jira/browse/CPPDEV-50) -
  [Regex] Provide support for regular expressions - {Redmine #37432} (AUL-18)
- [CPPDEV-51](https://devstack.vwgroup.com/jira/browse/CPPDEV-51) -
  [Logging] Provide infrastructure for application logging - {Redmine #37426} (AUL-17)
- [CPPDEV-52](https://devstack.vwgroup.com/jira/browse/CPPDEV-52) -
  [Variant] Provide wrapper to store arbitrary data types - {Redmine #37424} (AUL-16)
- [CPPDEV-53](https://devstack.vwgroup.com/jira/browse/CPPDEV-53) -
  [Strings] Provide extended functionality for `std::string` and C-style strings -
  {Redmine #37423} (AUL-15)
- [CPPDEV-54](https://devstack.vwgroup.com/jira/browse/CPPDEV-54) -
  [XML] Implement XML API - {Redmine #37350} (AUL-13)
- [CPPDEV-55](https://devstack.vwgroup.com/jira/browse/CPPDEV-55) -
  [cmake] Enable a_util_library to be usable as CMake package - {Redmine #37301} (AUL-12)
- [CPPDEV-56](https://devstack.vwgroup.com/jira/browse/CPPDEV-56) -
  Provide solution for last system error (AUL-11)
- [CPPDEV-57](https://devstack.vwgroup.com/jira/browse/CPPDEV-57) -
  [PortableTypes] Provide portable base types for all supported platforms -
  {Redmine #37352} (AUL-10)
- [CPPDEV-58](https://devstack.vwgroup.com/jira/browse/CPPDEV-58) -
  Enable doxygen documentation (AUL-3)
- [CPPDEV-62](https://devstack.vwgroup.com/jira/browse/CPPDEV-62) -
  [cmake] Naming convention of target libraries must reflect their platform dependence and build
  type - {Redmine #37994} (AUL-23)
- [CPPDEV-63](https://devstack.vwgroup.com/jira/browse/CPPDEV-63) -
  [Memory] Provide support for handling raw memory - {Redmine #37452} (AUL-21)
- [CPPDEV-64](https://devstack.vwgroup.com/jira/browse/CPPDEV-64) -
  [Memory] Provide smart pointer implementations for platforms not providing them -
  {Redmine #37353} (AUL-14)
- [CPPDEV-65](https://devstack.vwgroup.com/jira/browse/CPPDEV-65) -
  [Concurrency] Provide threading and synchronization mechanisms - {Redmine #38652} (AUL-4)
- [CPPDEV-70](https://devstack.vwgroup.com/jira/browse/CPPDEV-70) -
  Provide means to make canonical paths in `a_util::filesystem` (AUL-43)
- [CPPDEV-71](https://devstack.vwgroup.com/jira/browse/CPPDEV-71) -
  Provide fallback for C\+\+11 type traits (AUL-41)

<a name="a_util_5_0_0_fixes"></a>
#### Fixes
- [CPPDEV-13](https://devstack.vwgroup.com/jira/browse/CPPDEV-13) -
  [Result] Error definitions must not pollute the global namespace (AUL-62)
- [CPPDEV-18](https://devstack.vwgroup.com/jira/browse/CPPDEV-18) -
  System error test fails on Windows 7 / Visual Studio 2010 (AUL-55)
- [CPPDEV-23](https://devstack.vwgroup.com/jira/browse/CPPDEV-23) -
  `concurrency::thread::detach` crashes on fallback platforms (AUL-49)
- [CPPDEV-24](https://devstack.vwgroup.com/jira/browse/CPPDEV-24) -
  [Test] Overwrite `add_test()` macro instead of providing wrapper `add_util_test()` (AUL-48)
- [CPPDEV-25](https://devstack.vwgroup.com/jira/browse/CPPDEV-25) -
  [Test] Fix disabled installation of googletest (AUL-47)
- [CPPDEV-31](https://devstack.vwgroup.com/jira/browse/CPPDEV-31) -
  [Testing] Setting an alternative working directory (AUL-39)
- [CPPDEV-36](https://devstack.vwgroup.com/jira/browse/CPPDEV-36) -
  [Testing] Remove link dependency to a_util_filesystem for test targets (AUL-34)
- [CPPDEV-39](https://devstack.vwgroup.com/jira/browse/CPPDEV-39) -
  `a_util::variant::Variant` CCTOR doesn't properly copy strings (AUL-31)

<a name="a_util_5_0_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22a_util%205.0.0%22">Known issues</a></h4>


<a name="ddl_library"></a>
## DDL library

<a name="ddl_4_6_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.6.0">ddl library 4.6.0</a> - 2021/02/22</h3>

<a name="ddl_4_6_0_changes"></a>
#### Changes
- [CDDDL-116](https://devstack.vwgroup.com/jira/browse/CDDDL-116) -
  [Header2DDL] Types of hexadecimal values in enums must be interpreted as underlying type
- [CDDDL-117](https://devstack.vwgroup.com/jira/browse/CDDDL-117) -
  [Header2DDL] Basic preprocessor functionality needs to be implemented
- [CDDDL-118](https://devstack.vwgroup.com/jira/browse/CDDDL-118) -
  [Header2DDL] Basic arithmetic operation of constant integers must be implemented
- [CDDDL-132](https://devstack.vwgroup.com/jira/browse/CDDDL-132) -
  Use user "fep" instead of "aev25" in package references of the conan recipes
- [CDDDL-136](https://devstack.vwgroup.com/jira/browse/CDDDL-136) -
  [build] Define and apply whitespace settings in .gitattributes file
- [CDDDL-140](https://devstack.vwgroup.com/jira/browse/CDDDL-140) -
  [build] Use native GTest and CMake functionality
- [CDDDL-141](https://devstack.vwgroup.com/jira/browse/CDDDL-141) -
  [build] Generate API documentation using CoRTEXDocumentationHelper with doxygen
- [CDDDL-142](https://devstack.vwgroup.com/jira/browse/CDDDL-142) -
  [build] Restructure files and folders in source tree compliant to common configuration management
- [CDDDL-146](https://devstack.vwgroup.com/jira/browse/CDDDL-146) -
  Add .clang-format file and apply ruleset on C\+\+ codebase
- [CDDDL-155](https://devstack.vwgroup.com/jira/browse/CDDDL-155) - [test] Use more fitting test macros

<a name="ddl_4_6_0_fixes"></a>
#### Fixes
- [CDDDL-60](https://devstack.vwgroup.com/jira/browse/CDDDL-60) -
  Make cdddl compiles out of the box
- [CDDDL-97](https://devstack.vwgroup.com/jira/browse/CDDDL-97) -
  [ddlcontainer] Segmentation fault on linux during deleteAll()
- [CDDDL-98](https://devstack.vwgroup.com/jira/browse/CDDDL-98) -
  [build] Mutual dependency between conan recipe and CMake build automation of
  'list_used_licenses.md.in'
- [CDDDL-126](https://devstack.vwgroup.com/jira/browse/CDDDL-126) -
  [ddl2header] Test fixture cTesterddlgeneratorCmdLine fails on platforms using gcc
- [CDDDL-134](https://devstack.vwgroup.com/jira/browse/CDDDL-134) -
  [build] Unresolved symbols after making parts of a_util_result header-only
- [CDDDL-138](https://devstack.vwgroup.com/jira/browse/CDDDL-138) -
  [build] Conan recipes not compatible with conan versions \>1.16.2
- [CDDDL-139](https://devstack.vwgroup.com/jira/browse/CDDDL-139) -
  [ddl2header] Might crash with --struct command line option
- [CDDDL-153](https://devstack.vwgroup.com/jira/browse/CDDDL-153) -
  [Mapping] Possible null pointer exception

<a name="ddl_4_6_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.6.0%22">Known issues</a></h4>


<a name="ddl_4_5_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.5.0">ddl library 4.5.0</a> - 2020/05/25</h3>

<a name="ddl_4_5_0_changes"></a>
#### Changes
- [CDDDL-123](https://devstack.vwgroup.com/jira/browse/CDDDL-123) -
  Use the newly defined profiles as base for the delivery packages

<a name="ddl_4_5_0_fixes"></a>
#### Fixes
- [CDDDL-126](https://devstack.vwgroup.com/jira/browse/CDDDL-126) -
  [build] Build errors after update to a_util 5.7.0

<a name="ddl_4_5_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.5.0%22">Known issues</a></h4>


<a name="ddl_4_4_1"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.4.1">ddl library 4.4.1</a> - 2020/02/12</h3>

<a name="ddl_4_4_1_changes"></a>
#### Changes
- [CDDDL-111](https://devstack.vwgroup.com/jira/browse/CDDDL-111) -
  Extend Jenkinsfile for cross platform build of QNX target on Windows host

<a name="ddl_4_4_1_fixes"></a>
#### Fixes
- [CDDDL-119](https://devstack.vwgroup.com/jira/browse/CDDDL-119) -
  Missing semicolons when using a_util `_MAKE_ERROR` lead to error during compile time

<a name="ddl_4_4_1_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.4.1%22">Known issues</a></h4>


<a name="ddl_4_4_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.4.0">ddl library 4.4.0</a> - 2019/09/06</h3>

<a name="ddl_4_4_0_changes"></a>
#### Changes
- [CDDDL-85](https://devstack.vwgroup.com/jira/browse/CDDDL-85) -
  Implement new license mechanism in ddl
- [CDDDL-89](https://devstack.vwgroup.com/jira/browse/CDDDL-89) -
  Implement valid copy and move semantics
- [CDDDL-99](https://devstack.vwgroup.com/jira/browse/CDDDL-99) -
  Add conan recipe and Jenkinsfile to repository
- [CDDDL-102](https://devstack.vwgroup.com/jira/browse/CDDDL-102) -
  [BuildSystem] Change names of delivered libraries (*.lib and *.a) files

<a name="ddl_4_4_0_fixes"></a>
#### Fixes
- [CDDDL-92](https://devstack.vwgroup.com/jira/browse/CDDDL-92) -
  Missing merge of CDDDL-56 into master
- [CDDDL-103](https://devstack.vwgroup.com/jira/browse/CDDDL-103) -
  [BuildSystem] Language standard C\+\+11 is not propagated

<a name="ddl_4_4_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.4.0%22">Known issues</a></h4>


<a name="ddl_4_3_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.3.0">ddl library 4.3.0</a> - 2019/06/14</h3>

<a name="ddl_4_3_0_changes"></a>
#### Changes
- [CDDDL-74](https://devstack.vwgroup.com/jira/browse/CDDDL-74) -
  [Test] Normalize AEV_TEST macro to standard GoogleTest macro
- [CDDDL-81](https://devstack.vwgroup.com/jira/browse/CDDDL-81) -
  [Test] Exchange `aev_add_gtest()` with ordinary CMake command `add_test()``
- [CDDDL-82](https://devstack.vwgroup.com/jira/browse/CDDDL-82) -
  Porting to QNX
- [CDDDL-84](https://devstack.vwgroup.com/jira/browse/CDDDL-84) -
  Add reference files for tester install complete to verify DDL Library package

<a name="ddl_4_3_0_fixes"></a>
#### Fixes
- [CDDDL-65](https://devstack.vwgroup.com/jira/browse/CDDDL-65) -
  [ddlrepresentation] cannot rename sorted DDLContainer item
- [CDDDL-67](https://devstack.vwgroup.com/jira/browse/CDDDL-67) -
  [ddlinspector] Min/max values for tBool hold literals instead of numeric values
- [CDDDL-70](https://devstack.vwgroup.com/jira/browse/CDDDL-70) -
  [DDLDescription] Default min\+max values for tInt8 and tUInt8 are not numbers
- [CDDDL-71](https://devstack.vwgroup.com/jira/browse/CDDDL-71) -
  [DDLCloner] Check for memory leak in cDDLCloner
- [CDDDL-75](https://devstack.vwgroup.com/jira/browse/CDDDL-75) -
  [ddlinspector] Default value of a structure element without bounding limits is not compared with
  datatype bounding limits

<a name="ddl_4_3_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.3.0%22">Known issues</a></h4>


<a name="ddl_4_2_1"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.2.1">ddl library 4.2.1</a> - 2019/03/19</h3>

<a name="ddl_4_2_1_fixes"></a>
#### Fixes
- [CDDDL-77](https://devstack.vwgroup.com/jira/browse/CDDDL-77) -
  DDLResolver uses current date and time in generated description

<a name="ddl_4_2_1_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.2.1%22">Known issues</a></h4>


<a name="ddl_4_2_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.2.0">ddl library 4.2.0</a> - 2018/11/08</h3>

<a name="ddl_4_2_0_changes"></a>
#### Changes
- [CDDDL-61](https://devstack.vwgroup.com/jira/browse/CDDDL-61) -
  Create XSDs for DDL 3 and 4
- [CDDDL-62](https://devstack.vwgroup.com/jira/browse/CDDDL-62) -
  Remove ADPL and add MPL 2.0 License

<a name="ddl_4_2_0_fixes"></a>
#### Fixes
- [CDDDL-64](https://devstack.vwgroup.com/jira/browse/CDDDL-64) -
  DDL version migrate cmake makro throws warnings

<a name="ddl_4_2_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.2.0%22">Known issues</a></h4>


<a name="ddl_4_1_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.1.0">ddl library 4.1.0</a> - 2018/06/30</h3>

<a name="ddl_4_1_0_changes"></a>
#### Changes
- [CDDDL-15](https://devstack.vwgroup.com/jira/browse/CDDDL-15) -
  Change Version Variable of OO DDL from Floating to a struct Major/Minor
- [CDDDL-16](https://devstack.vwgroup.com/jira/browse/CDDDL-16) -
  Change to new Coding Style
- [CDDDL-17](https://devstack.vwgroup.com/jira/browse/CDDDL-17) -
  Reenable Documentation creation
- [CDDDL-24](https://devstack.vwgroup.com/jira/browse/CDDDL-24) -
  Unify commandline arguments with other utilities - commandline tooling must use --my_parameter
  (corresponding -mp)
- [CDDDL-34](https://devstack.vwgroup.com/jira/browse/CDDDL-34) -
  [DOC] DDL Documentation must use Markdown format and the content must be updated
- [CDDDL-39](https://devstack.vwgroup.com/jira/browse/CDDDL-39) -
  Create clang-tidy script for porting usercode from 4.0 to 4.1
- [CDDDL-44](https://devstack.vwgroup.com/jira/browse/CDDDL-44) -
  [Mapping] Add method to mapping engine to check whether a signal has any triggers
- [CDDDL-45](https://devstack.vwgroup.com/jira/browse/CDDDL-45) -
  Add simple possibility to identify dynamic arrays
- [CDDDL-46](https://devstack.vwgroup.com/jira/browse/CDDDL-46) -
  [DDLInspector] Invalid numbers containing multiple '.' must be detected and corrected
- [CDDDL-48](https://devstack.vwgroup.com/jira/browse/CDDDL-48) -
  Check const correctness of DDL Library

<a name="ddl_4_1_0_fixes"></a>
#### Fixes
- [CDDDL-35](https://devstack.vwgroup.com/jira/browse/CDDDL-35) -
  cDDLImporter uses `ArraySizeSource` wrong
- [CDDDL-36](https://devstack.vwgroup.com/jira/browse/CDDDL-36) -
  cDDLCloner cannot clone FEP Driver DDL
- [CDDDL-41](https://devstack.vwgroup.com/jira/browse/CDDDL-41) -
  [DDL] Type names containing '/' are not supported

<a name="ddl_4_1_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.1.0%22">Known issues</a></h4>


<a name="ddl_4_0_0"></a>
<h3><a href="https://devstack.vwgroup.com/bitbucket/projects/OPENDEV/repos/dev_essential/browse?at=refs%2Ftags%2Fddl_library_v4.0.0">ddl library 4.0.0</a> - 2017/12/06</h3>

<a name="ddl_4_0_0_changes"></a>
#### Changes

<a name="ddl_4_0_0_fixes"></a>
#### Fixes

<a name="ddl_4_0_0_known_issues"></a>
<h4><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20type%3D%22Bug%22%20AND%20level%3D%22public%22%20AND%20affectedVersion%20%3D%20%22DDL%204.0.0%22">Known issues</a></h4>


<a name="upcoming"></a>
<h3><a href="https://devstack.vwgroup.com/jira/issues/?jql=project%3D%22Utility%20Library%22%20AND%20level%3D%22public%22%20AND%20status%21%3D%22Done%22%20AND%20status%21%3D%22Rejected%22">Upcoming</a></h3>

Shows the upcoming features.

<!--- external links -->
[SF.12]: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rs-incform
[Keep a Changelog]: https://keepachangelog.com
[Semantic Versioning]: https://semver.org
[CONAN_V2_MODE]: https://docs.conan.io/en/1.48/reference/conan_v2_mode.html#conan-v2-mode
[clipp]: https://github.com/muellan/clipp
[clara]: https://github.com/catchorg/Clara
[argtable3]: https://github.com/argtable/argtable3
