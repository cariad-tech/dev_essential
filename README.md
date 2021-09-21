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
# Dev essential

<a name="introduction"></a>
## Introduction
The dev_essential project is the successor of the following projects:
* Utility library aka [a_util](a_util.md)
* Package Remote Procedure Call aka [pkg_rpc](rpc.md)
* Data Definition Language Library aka [ddl](ddl.md)
* ...

The dev_essential project strives for backwards compatibility with the latest versions of these
projects. The following table provides an overview of the versions and the compatibility.

|    Project    |   Supported since   |
| ------------- | ------------------- |
| a_util v5.8.x | dev_essential 1.0.0 |
| pkg_rpc 3.5.x | dev_essential 1.0.0 |
| ddl 4.6.x     | dev_essential 1.0.0 |

However, for downstream projects already using one of the aforementioned projects and porting to the
dev_essential project, the [portation guide](#portation-guide) shall be consulted.

Table of contents:

1. [How to build](#how-to-build)
2. [How to use](#how-to-use)
3. [Supported platforms](#supported-platforms)
4. [Portation guide](#portation-guide)
5. [Changelog](#changelog)
6. [License Information](#license-information)

<a name="how-to-build"></a>
## How to build
### Build with CMake
- Download [CMake](https://cmake.org/) at least in version 3.17.0
- Using [git](https://git-scm.com/), clone the repository and checkout the desired branch (e.g. `master`)
- Use CMake with *CMakeLists.txt* within the root directory
- Do not forget to set `CMAKE_INSTALL_PREFIX`
- Example:
  - `cmake -H<root_dir> -B<build_dir> -G "Visual Studio 16 2019" -DCMAKE_INSTALL_PREFIX=<install_dir>`
  - `cmake --build . --target INSTALL --config RelWithDebInfo`

#### CMake options
**dev_essential_cmake_enable_integrated_tests (default: OFF)**
* Enable tests as integrated build - requires googletest

**dev_essential_cmake_enable_position_independent_code (Default: ON)**
* Enable position independent code for static libraries

**dev_essential_cmake_enable_result_header_only (default: OFF)**
* Whether or not to build header only version of dev_essential_result library

**dev_essential_cmake_enable_warn_on_default_cmake_install_prefix (Default: ON)**
* Warn if `CMAKE_INSTALL_PREFIX` is set to default

<a name="how-to-use"></a>
## How to use
### Use with CMake
To simply use *all components* in your project, use
- [`find_package(dev_essential)`][] with `dev_essential_DIR=<dev_essential_root>/lib/cmake/dev_essential`,
  with `<dev_essential_root>` being the path to the dev_essential installation folder
- [`target_link_libraries(<target> PRIVATE|PUBLIC dev_essential::dev_essential)`][] with
  `<target>` being the name of your library or executable

To only use *specific components* in your project, use
- `find_package(dev_essential COMPONENTS <components>)` with
  `<components>` being a list of components (e.g. `process`, `xml`, ...), separated by spaces
- `target_link_libraries(<target> PRIVATE|PUBLIC dev_essential::<component>)` with
  - `<target>` being the name of your library or executable and
  - `<component>` being the component you want to link against

### Use in your code
To simply use *all functionality* of one component, include the equivalent component header into
your code.
For example, to use all functionality of components `a_util::filesystem` and `a_util::process`:
```
// my_file.h

#include <a_util/filesystem.h> // for a_util::filesystem
#include <a_util/process.h>    // for a_util::process
```

However, all delivered header files are self-contained and so each functionality can be
included individually.

---
**NOTE:** Do not directly include header files ending with `_decl.h` or `_impl.h`.
Those files are meant for implementation specific functionalilty and might change unannounced in future releases.

---

<a name="supported-platforms"></a>
## Supported platforms
The libraries are currently built and tested using the following compilers and operating systems:

### Windows
- Visual C++ (x86\_64): 14.1, 14.2

### Linux
- GCC (-std=c++14, libstdc++11, x86\_64): 5.4.0, 5.5.0. 7.5.0, 8.4.0
- GCC (-std=c++14, libstdc++11, armv8): 5.4.0, 7.5.0

### QNX
- GCC (-std=c++1y, libstdc++11, x86): 5.2.0

<a name="portation-guide"></a>
## Portation guide
The following changes have to be done.

### a_util
#### CMake
- `find_package(a_util REQUIRED)` &rarr; `find_package(dev_essential REQUIRED)`
- `find_package(a_util REQUIRED COMPONENTS <component>)` &rarr; `find_package(dev_essential REQUIRED COMPONENTS <component>)`
  - e.g. `find_package(a_util REQUIRED COMPONENTS xml)` &rarr; `find_package(dev_essential REQUIRED COMPONENTS xml)`
- `target_link_libraries(myLib a_util)` &rarr; `target_link_libraries(myLib dev_essential::dev_essential)`
  _**NOTE:**_ This pulls in _ALL_ components, including those ported from other projects (e.g. pkg_rpc)
- `target_link_libraries(myLib a_util_<component>)` &rarr; `target_link_libraries(myLib dev_essential::<component>)`
  - e.g. `target_link_libraries(myLib a_util_xml)` &rarr; `target_link_libraries(myLib dev_essential::xml)`

### ddl library
#### CMake
- `find_package(ddl REQUIRED)` &rarr; `find_package(dev_essential REQUIRED COMPONENTS ddl)`
- `target_link_libraries(myLib ddl)` &rarr; `target_link_libraries(myLib dev_essential::ddl)`

##### C++
All headers are moved to subfolder _ddl_ which is reflected in the include directories.
So e.g. the following includes:
```cpp
#include <ddl.h>
#include <codec/pkg_codec.h>
//...
```
Become respectively:
```cpp
#include <ddl/ddl.h>
#include <ddl/codec/pkg_codec.h>
//...
```

---

Namespace `mapping::oo` is removed, all implementations went into namespace `mapping`.
Namespace `mapping` is moved into namespace `ddl`.
All references to namespace `mapping` need to be referenced by namespace `ddl::mapping` now.
So the following usage of namespace `mapping`:
```cpp
using MapConfiguration = mapping::oo::MapConfiguration;
```
Becomes:
```cpp
using MapConfiguration = ddl::mapping::MapConfiguration;
```

---

The implementation of the deprecated `ddl::DDLDescription` has been removed and completely
refactored into the new DDL DataDefinition. Please refer to the
[DD documentation](doc/extern/ddl/dd.md) for its usage. A guide to port the deprecated DDL
description to the new DDL DataDefinition is under development.

### pkg_rpc
#### CMake
- `find_package(pkg_rpc REQUIRED)` &rarr; `find_package(dev_essential REQUIRED COMPONENTS pkg_rpc)`
- `target_link_libraries(myLib pkg_rpc)` &rarr; `target_link_libraries(myLib dev_essential::pkg_rpc)`

#### C++
The following includes:
```cpp
#include <pkg_rpc.h>
#include <rpc_pkg/rpc_server.h>
#include <rpc_pkg/json_rpc.h>
```
Become respectively:
```cpp
#include <rpc/rpc.h>
#include <rpc/rpc_server.h>
#include <rpc/json_rpc.h>
```

<a name="changelog"></a>
## Changelog
The changelog can be found [here](doc/extern/changelog.md).

## License Information <a name="license-information"></a>
### Dev essential libraries - License
The dev essential libraries are delivered under the
[MPL - Mozilla Public License - Version 2.0](LICENSE).

### Used software license information
The dev essential libraries are delivered with the following 3rdparty dependencies:
- [Clara 1.1.2](https://github.com/catchorg/Clara/tree/v1.1.2)
  ([License](extern/3rdparty/clara/Clara-1.1.2/LICENSE.txt))
- [current_function of Boost 1.64.0](http://www.boost.org/doc/libs/1_64_0/boost/current_function.hpp)
  ([License](extern/3rdparty/boost/boost_1_64_0/boost/current_function.hpp))
- [PCRE 8.39](https://sourceforge.net/projects/pcre/files/pcre/8.39/)
  ([License](extern/3rdparty/pcre/pcre-8.39/LICENCE))
- [pugixml 1.8](https://github.com/zeux/pugixml/tree/v1.8)
  ([License](extern/3rdparty/pugixml/pugixml-1.8/readme.txt))
- [uuid 1.6.2](http://www.ossp.org/pkg/lib/uuid/)
  ([License](extern/3rdparty/uuid/uuid-1.6.2/license.txt))
- [UTF8-CPP](https://github.com/nemtrif/utfcpp/tree/v2.3.4)
  ([License](extern/3rdparty/utfcpp/utf8_v2_3_4/source/utf8.h))

[`find_package(dev_essential)`]: https://cmake.org/cmake/help/latest/command/find_package.html
[`target_link_libraries(<target> PRIVATE|PUBLIC dev_essential::dev_essential)`]: https://cmake.org/cmake/help/latest/command/target_link_libraries.html
