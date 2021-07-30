/**
 * @file
 * Parser helper functions.
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

#ifndef _PARSER_HELPER_CLASS_HEADER_
#define _PARSER_HELPER_CLASS_HEADER_

#include <string>

namespace rpc {

/**
 * Advances the pointer to point at a position after the next whitespace sequence.
 *
 * @param p [inout] The current position.
 * @param pAdditionalWhitechars [in] Additional characters which are to be skipped.
 * @return void
 * @rtsafe
 */
void parse_skip_whitespace(const char*& p, const char* pAdditionalWhitechars = nullptr);

/**
 * Copies the next word in a string into a string object.
 *
 * @param pSrc                 [in] The source string.
 * @param strDest              [out] The destination string.
 * @param pAdditionalSeparator [in] Additional separator characters.
 * @param bUseEscape           [in] Uses the \\ character to detect a word too
 *
 * @return tTrue if successful, otherwise tFalse.
 */
bool parse_get_next_word(const char*& pSrc,
                         std::string& strDest,
                         const char* pAdditionalSeparator = nullptr,
                         bool bUseEscape = true);

/**
 * Advances a pointer in a string to the next occurance of specified characters.
 * @param p [inout] The current position.
 * @param pChars [in] The characters to look for.
 * @param bIgnoreQuotes [in] Whether or not to ignore occurences which occur between quotes.
 * @return void
 * @rtsafe
 */
void parse_seek_chars(const char*& p, const char* pChars, bool bIgnoreQuotes = false);

/**
 * Advances a pointer in a string to the next occurcance of a string.
 * @param p [inout] The current position.
 * @param pString [in] The string to look for.
 * @param ptrComment [out] Optional buffer that will be filled with the skipped characters.
 * @param bIgnoreQuotes [in] Whether or not to ignore occurences which occur between quotes.
 * @return void
 * @rtsafe
 */
void parse_seek_string(const char*& p,
                       const char* pString,
                       char* ptrComment = nullptr,
                       bool bIgnoreQuotes = false);

} // namespace rpc

#endif // _PARSER_HELPER_CLASS_HEADER_
