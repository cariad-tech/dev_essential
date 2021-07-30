# Copyright @ 2021 VW Group. All rights reserved.
#
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
#
# You may add additional accurate notices of copyright ownership.

##See: http://www.boost.org/doc/libs/1_64_0/boost/current_function.hpp
install(DIRECTORY boost_1_64_0 DESTINATION include/3rdparty/boost)
install(FILES boost_1_64_0/boost/current_function.hpp
        DESTINATION doc/license/boost_1_64_0/boost)
