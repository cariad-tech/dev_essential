/**
 * @file
 * Regex library
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include "a_util/regex/regularexpression.h"

#include "../../extern/3rdparty/pcre/pcre-8.39/pcrecpp.h"

namespace a_util {
namespace regex {
// using this in member initialization list
#pragma warning(disable : 4355)

#define DECLARE_ARG_HELPER(arg)                                                                    \
    pcrecppinternal::Arg arg##_t(&arg);                                                            \
    const pcrecppinternal::Arg& arg##_h =                                                          \
        *(&arg == &RegularExpression::noArg() ? &pcrecppinternal::RE::no_arg : &arg##_t);

// RegularExpression implementation
class RegularExpression::Implementation {
public:
    RegularExpression* _self;
    bool _is_case_sensitive;
    std::unique_ptr<pcrecppinternal::RE> _regex;
    std::string _error_description;

    Implementation(RegularExpression* self)
        : _self(self), _is_case_sensitive(true), _regex(), _error_description()
    {
    }
};

RegularExpression::RegularExpression() : _impl(new Implementation(this))
{
}

RegularExpression::RegularExpression(const std::string& pattern, bool case_sensitive)
    : _impl(new Implementation(this))
{
    setPattern(pattern, case_sensitive);
}

RegularExpression::~RegularExpression()
{
}

RegularExpression::RegularExpression(const RegularExpression& other)
    : _impl(new Implementation(this))
{
    setPattern(other.getPattern(), other._impl->_is_case_sensitive);
}

RegularExpression& RegularExpression::operator=(const RegularExpression& other)
{
    if (&other != this) {
        _impl.reset(new Implementation(this));
        setPattern(other.getPattern(), other._impl->_is_case_sensitive);
    }

    return *this;
}

bool RegularExpression::setPattern(const std::string& pattern, bool case_sensitive)
{
    if (pattern.empty()) {
        _impl->_error_description = "Empty pattern is invalid";
        return false;
    }

    pcrecppinternal::RE_Options opts;
    opts.set_caseless(!case_sensitive);
    std::unique_ptr<pcrecppinternal::RE> regex(new pcrecppinternal::RE(pattern.c_str(), opts));
    if (!regex->error().empty()) {
        _impl->_error_description = _impl->_regex->error();
        return false;
    }

    _impl->_regex.reset(regex.release());
    _impl->_is_case_sensitive = case_sensitive;
    _impl->_error_description.clear();

    return true;
}

std::string RegularExpression::getPattern() const
{
    return _impl->_regex ? _impl->_regex->pattern() : std::string();
}

const std::string& RegularExpression::getError() const
{
    return _impl->_error_description;
}

bool RegularExpression::fullMatch(const std::string& text) const
{
    if (!_impl->_regex)
        return false;
    return _impl->_regex->FullMatch(text);
}

bool RegularExpression::fullMatch(const std::string& text,
                                  std::string& arg1,
                                  std::string& arg2,
                                  std::string& arg3,
                                  std::string& arg4,
                                  std::string& arg5,
                                  std::string& arg6,
                                  std::string& arg7,
                                  std::string& arg8,
                                  std::string& arg9,
                                  std::string& arg10,
                                  std::string& arg11,
                                  std::string& arg12,
                                  std::string& arg13,
                                  std::string& arg14,
                                  std::string& arg15,
                                  std::string& arg16) const
{
    if (!_impl->_regex)
        return false;

    DECLARE_ARG_HELPER(arg1);
    DECLARE_ARG_HELPER(arg2);
    DECLARE_ARG_HELPER(arg3);
    DECLARE_ARG_HELPER(arg4);
    DECLARE_ARG_HELPER(arg5);
    DECLARE_ARG_HELPER(arg6);
    DECLARE_ARG_HELPER(arg7);
    DECLARE_ARG_HELPER(arg8);
    DECLARE_ARG_HELPER(arg9);
    DECLARE_ARG_HELPER(arg10);
    DECLARE_ARG_HELPER(arg11);
    DECLARE_ARG_HELPER(arg12);
    DECLARE_ARG_HELPER(arg13);
    DECLARE_ARG_HELPER(arg14);
    DECLARE_ARG_HELPER(arg15);
    DECLARE_ARG_HELPER(arg16);

    return _impl->_regex->FullMatch(text,
                                    arg1_h,
                                    arg2_h,
                                    arg3_h,
                                    arg4_h,
                                    arg5_h,
                                    arg6_h,
                                    arg7_h,
                                    arg8_h,
                                    arg9_h,
                                    arg10_h,
                                    arg11_h,
                                    arg12_h,
                                    arg13_h,
                                    arg14_h,
                                    arg15_h,
                                    arg16_h);
}

bool RegularExpression::match(const std::string& text, AnchorType anchor, int& consumed) const
{
    if (!_impl->_regex)
        return false;

    pcrecppinternal::RE::Anchor anchor_internal = pcrecppinternal::RE::UNANCHORED;
    if (anchor == AT_Start) {
        anchor_internal = pcrecppinternal::RE::ANCHOR_START;
    }
    else if (anchor == AT_Both) {
        anchor_internal = pcrecppinternal::RE::ANCHOR_BOTH;
    }

    return _impl->_regex->DoMatch(
        pcrecppinternal::StringPiece(text), anchor_internal, &consumed, nullptr, 0);
}

std::string& RegularExpression::noArg()
{
    static std::string no_arg;
    return no_arg;
}

} // namespace regex
} // namespace a_util
