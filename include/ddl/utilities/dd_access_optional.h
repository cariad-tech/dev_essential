/**
 * @file
 * OO DataDefinition Optional Implementation
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef DD_DDL_ACCESS_OPTIONAL_H_INCLUDED
#define DD_DDL_ACCESS_OPTIONAL_H_INCLUDED

namespace ddl {
namespace dd {
namespace utility {

/**
 * @brief An optional template as long as the std::optional is not available here.
 *
 * @tparam T the value type
 */
template <typename T>
struct Optional {
    /**
     * @brief default CTOR
     *
     */
    Optional(){};
    /**
     * @brief move CTOR
     *
     */
    Optional(Optional&&) = default;
    /**
     * @brief copy CTOR
     *
     */
    Optional(const Optional&) = default;
    /**
     * @brief move assignment operator
     *
     * @return Optional&
     */
    Optional& operator=(Optional&&) = default;
    /**
     * @brief copy assignment operator
     *
     * @return Optional&
     */
    Optional& operator=(const Optional&) = default;
    /**
     * @brief DTOR
     *
     */
    ~Optional() = default;

    /// defintion of the value type of this optional
    typedef T value_type;

    /**
     * @brief special copy CTOR
     *
     * @param value the value to copy
     */
    Optional(const value_type& value) : _valid(true), _value(value)
    {
    }
    /**
     * @brief copy assignment operator
     *
     * @param value the value to copy
     * @return Optional&
     */
    Optional& operator=(const value_type& value)
    {
        _valid = true;
        _value = value;
        return *this;
    }

    /**
     * @brief move CTOR
     *
     * @param value the value to move
     */
    Optional(value_type&& value) : _valid(true), _value(value)
    {
    }

    /**
     * @brief move assignment operator
     *
     * @param value the value to move
     * @return Optional&
     */
    Optional& operator=(value_type&& value)
    {
        _valid = true;
        _value = value;
        return *this;
    }

    /**
     * @brief validation status of the optional
     *
     * @return true the value is valid
     * @return false the value is not valid
     */
    operator bool() const
    {
        return _valid;
    }

    /**
     * @brief conversion to the value type
     *
     * @return value_type
     */
    operator value_type() const
    {
        return _value;
    }

    /**
     * @brief referenced access to the value
     *
     * @return value_type&
     */
    value_type& operator*()
    {
        return _value;
    }

    /**
     * @brief dereferenced access of the value
     *
     * @return const value_type
     */
    const value_type operator*() const
    {
        return _value;
    }

    /**
     * @brief invalidates the value
     *
     */
    void reset()
    {
        _valid = false;
    }
    /**
     * @brief return the value type as copy
     *
     * @return value_type
     */
    value_type get() const
    {
        return _value;
    }
    /**
     * @brief equality operator.
     *
     * @param other the other Optional to compare this Optional to.
     * @return true  the Optionals are equal (value and validity)
     * @return false the Optionals are not equal.
     */
    bool operator==(const Optional& other) const
    {
        if (_valid) {
            return other._valid && (_value == other._value);
        }
        else {
            return !(other._valid);
        }
    }

    /**
     * @brief non equality operator.
     *
     * @param other the other Optional to compare this Optional to.
     * @return false the Optionals are equal (value and validity)
     * @return true  the Optionals are not equal.
     */
    bool operator!=(const Optional& other) const
    {
        return !operator==(other);
    }
    /// is valid or not
    bool _valid = false;
    /// the value if it is valid
    value_type _value = {};
};

} // namespace utility
} // namespace dd
} // namespace ddl

#endif // DD_DDL_ACCESS_OPTIONAL_H_INCLUDED