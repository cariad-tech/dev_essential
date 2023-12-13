/**
 * @file
 * OO DataDefinition Redesign
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_ERROR_H_INCLUDED
#define DD_ERROR_H_INCLUDED

#include <stdexcept>
#include <vector>

namespace ddl {

namespace dd {

/**
 * @brief Problem to report to find the corresponding item name in a simple way
 *
 */
struct Problem {
    /// the item name
    std::string item_name = {};
    /// the validation problem in text
    std::string problem_message = {};
};

/**
 * @brief transforms the Problem List to a simple vector of strings.
 * \n Simple usage:
 * @code
 * //...
 *
 * using namespace ddl;
 * dd::DataDefinition my_dd;
 * //... adding ... loading ... using
 * std::cout <<
 * a_util::strings::join(dd::transformProblemList(my_dd.getValidationProtocol()),"\n");
 *
 * @endcode
 *
 * @param problems the problems to tranform.
 * @return std::vector<std::string>
 */
std::vector<std::string> transformProblemList(const std::vector<Problem>& problems);

/**
 * @brief Exception helper class to collect information while parsing, adding DD Objects or other
 * failed operatons.
 *
 */
class Error : public std::exception {
private:
    static std::string joinArgs(const std::vector<std::string>& oo_operation_args);

public:
    /**
     * @brief CTOR
     *
     * @param oo_type_operation name of the operation
     * @param message the message to set
     */
    Error(const std::string& oo_type_operation, const std::string& message)
        : _message(oo_type_operation + ": " + message)
    {
        _problems.push_back({oo_type_operation, message});
    }

    /**
     * @brief CTOR with arguments
     *
     * @param oo_type_operation name of the operation
     * @param oo_operation_args list of the given arguments
     * @param message the message to set
     */
    Error(const std::string& oo_type_operation,
          const std::vector<std::string>& oo_operation_args,
          const std::string& message)
        : _message(oo_type_operation + "(" + joinArgs(oo_operation_args) + ")" + ": " + message)
    {
        _problems.push_back({oo_type_operation + "(" + joinArgs(oo_operation_args) + ")", message});
    }

    /**
     * @brief CTOR with arguments
     *
     * @param oo_type_operation name of the operation
     * @param oo_operation_args list of the given arguments
     * @param message the message to set
     * @param problems the problems (usually the validation protocol)
     */
    Error(const std::string& oo_type_operation,
          const std::vector<std::string>& oo_operation_args,
          const std::string& message,
          const std::vector<Problem>& problems)
        : _message(oo_type_operation + "(" + joinArgs(oo_operation_args) + ")" + ": " + message),
          _problems(problems)
    {
    }
    /**
     * get the problems
     * @return std::vector<Problem>&
     */
    const std::vector<Problem>& problems() const
    {
        return _problems;
    }

    /**
     * get the problem message
     * @return const char*
     */
    char const* what() const noexcept override
    {
        return _message.c_str();
    }

private:
    std::string _message;
    std::vector<Problem> _problems;
};

} // namespace dd
} // namespace ddl

#endif // DD_ERROR_H_INCLUDED