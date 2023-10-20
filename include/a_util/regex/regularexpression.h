/**
 * @file
 * Public API for @ref a_util::regex::RegularExpression "RegularExpression" type
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTILS_UTIL_REGEX_SOURCE_INCLUDED_
#define A_UTILS_UTIL_REGEX_SOURCE_INCLUDED_

#include <memory>
#include <string>

namespace a_util {
namespace regex {
/// Basic regular expression matching
class RegularExpression {
public:
    /// Anchor type
    typedef enum {
        AT_Unanchored, //!< No anchoring
        AT_Start,      //!< Anchor at start only
        AT_Both        //!< Anchor at start and end
    } AnchorType;

public:
    /**
     * Default constructor
     */
    RegularExpression();

    /**
     * Constructor with a preinitialized pattern
     *
     * @param[in] pattern The regular expression pattern
     * @param[in] case_sensitive Whether or not the match is case sensitive
     * @note On error, the instance is initialized to the default state/the pattern is not set
     */
    RegularExpression(const std::string& pattern, bool case_sensitive = true);

    /**
     * DTOR
     */
    ~RegularExpression();

    /**
     * Copy constructor
     * @param[in] other The object to copy.
     */
    RegularExpression(const RegularExpression& other);

    /**
     * Copy assignment operator
     * @param[in] other The element to copy to.
     * @return A reference of the local object.
     */
    RegularExpression& operator=(const RegularExpression& other);

    /**
     * Set the regular expression pattern
     * @param[in] pattern The regular expression string
     * @param[in] case_sensitive Whether or not the match is case sensitive
     * @return @c false if the pattern is invalid, see @ref getError for details
     */
    bool setPattern(const std::string& pattern, bool case_sensitive = true);

    /**
     * Get the current pattern (expression).
     * @return The current pattern.
     */
    std::string getPattern() const;

    /**
     * Get any error description for the pattern.
     * @return The current error. Might be empty if the pattern is valid.
     */
    const std::string& getError() const;

    /**
     * Check if the whole string matches the pattern.
     * @param[in] text The string to match
     * @return @c true if the string matches, @c false otherwise
     */
    bool fullMatch(const std::string& text) const;

    /**
     * Check if the whole string matches the pattern, extracting up to 16 capture groups
     *
     * @param[in] text The string to match
     * @param[out] arg1 Capture argument 1
     * @param[out] arg2 Capture argument 2
     * @param[out] arg3 Capture argument 3
     * @param[out] arg4 Capture argument 4
     * @param[out] arg5 Capture argument 5
     * @param[out] arg6 Capture argument 6
     * @param[out] arg7 Capture argument 7
     * @param[out] arg8 Capture argument 8
     * @param[out] arg9 Capture argument 9
     * @param[out] arg10 Capture argument 10
     * @param[out] arg11 Capture argument 11
     * @param[out] arg12 Capture argument 12
     * @param[out] arg13 Capture argument 13
     * @param[out] arg14 Capture argument 14
     * @param[out] arg15 Capture argument 15
     * @param[out] arg16 Capture argument 16
     * @return @ true if the string matches, @c false otherwise
     */
    bool fullMatch(const std::string& text,
                   std::string& arg1,
                   std::string& arg2 = noArg(),
                   std::string& arg3 = noArg(),
                   std::string& arg4 = noArg(),
                   std::string& arg5 = noArg(),
                   std::string& arg6 = noArg(),
                   std::string& arg7 = noArg(),
                   std::string& arg8 = noArg(),
                   std::string& arg9 = noArg(),
                   std::string& arg10 = noArg(),
                   std::string& arg11 = noArg(),
                   std::string& arg12 = noArg(),
                   std::string& arg13 = noArg(),
                   std::string& arg14 = noArg(),
                   std::string& arg15 = noArg(),
                   std::string& arg16 = noArg()) const;

    /**
     * Matches a string
     * @param[in] text The string to match
     * @param[in] anchor See @ref AnchorType
     * @param[out] consumed The length of the match
     * @return @c true if the string matches, @c false otherwise
     */
    bool match(const std::string& text, AnchorType anchor, int& consumed) const;

private: // static helpers
    /// Empty argument
    static std::string& noArg();

private: // data
    class Implementation;
    std::unique_ptr<Implementation> _impl;
};

} // namespace regex
} // namespace a_util

#endif // A_UTILS_UTIL_REGEX_SOURCE_INCLUDED_
