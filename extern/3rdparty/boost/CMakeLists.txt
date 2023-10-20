# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

##See: http://www.boost.org/doc/libs/1_64_0/boost/current_function.hpp
install(DIRECTORY boost_1_64_0 DESTINATION include/3rdparty/boost)
install(FILES boost_1_64_0/boost/current_function.hpp
        DESTINATION doc/license/boost_1_64_0/boost)
