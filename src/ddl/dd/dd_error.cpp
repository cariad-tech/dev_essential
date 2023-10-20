/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <ddl/dd/dd_error.h>

namespace ddl {

namespace dd {

std::vector<std::string> transformProblemList(const std::vector<Problem>& problems)
{
    std::vector<std::string> res;
    for (auto& ref: problems) {
        res.push_back(std::string(ref.item_name + ": " + ref.problem_message));
    }
    return res;
}

std::string Error::joinArgs(const std::vector<std::string>& oo_operation_args)
{
    std::string joined_string;
    auto current = oo_operation_args.begin();
    auto end_of_vec = (current == oo_operation_args.end());
    while (!end_of_vec) {
        joined_string += *current;
        ++current;
        if (current == oo_operation_args.end()) {
            end_of_vec = true;
        }
        else {
            joined_string += ", ";
        }
    }
    return joined_string;
}

} // namespace dd
} // namespace ddl
