/**
 * @file
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

#include "parserhelper.h"

#include <a_util/strings.h>

#include <string.h>

namespace rpc {

void parse_skip_whitespace(const char*& p, const char* pAdditionalWhitechars)
{
    if (nullptr == p) {
        return;
    }
    if (pAdditionalWhitechars != nullptr) {
        while (a_util::strings::white_space.find(*p) != std::string::npos ||
               (*p != '\0' && strchr(pAdditionalWhitechars, *p) != nullptr)) {
            p++;
        }
    }
    else {
        while (a_util::strings::white_space.find(*p) != std::string::npos) {
            p++;
        }
    }
}

bool parse_get_next_word(const char*& pSrc,
                         std::string& strDest,
                         const char* pAdditionalSeparator,
                         bool bUseEscape)
{
    if (nullptr == pSrc) {
        return false;
    }
    strDest.clear();

    parse_skip_whitespace(pSrc);

    if (*pSrc == '\0') {
        return false;
    }

    char bEscapeActive = false;
    char cLaschar = '\0';
    char cQuote = '\0';

    if (*pSrc == '\"' || *pSrc == '\'') {
        cQuote = *(pSrc++);
        const char* pSrcStart = pSrc;

        while (*pSrc != '\0' && (bEscapeActive || *pSrc != cQuote)) {
            bEscapeActive = bUseEscape && (*pSrc == '\\' && cLaschar != '\\'); // escape next char?
            cLaschar = *pSrc;
            pSrc++;
        }
        strDest.assign(pSrcStart, pSrc - pSrcStart);
        // strDest.set(pSrcStart, pSrc-pSrcStart);

        if (*pSrc == cQuote) {
            pSrc++;
        }
    }
    else {
        const char* pSrcStart = pSrc;

        if (pAdditionalSeparator == nullptr) {
            while (*pSrc != '\0' && a_util::strings::white_space.find(*pSrc) == std::string::npos) {
                pSrc++;
                if (*pSrc == '\"' || *pSrc == '\'') {
                    cQuote = *(pSrc);

                    do {
                        bEscapeActive = bUseEscape && (*pSrc == '\\' && cLaschar != '\\');
                        cLaschar = *pSrc;
                        pSrc++;
                    } while (*pSrc != '\0' && (bEscapeActive || *pSrc != cQuote));
                }
            }

            strDest.assign(pSrcStart, pSrc - pSrcStart);
        }
        else {
            while (*pSrc != '\0' &&
                   (a_util::strings::white_space.find(*pSrc) == std::string::npos &&
                    strchr(pAdditionalSeparator, *pSrc) == nullptr)) {
                pSrc++;
            }

            strDest.assign(pSrcStart, pSrc - pSrcStart);
        }
    }

    return true;
}

void parse_seek_chars(const char*& p, const char* pChars, bool bIgnoreQuotes)
{
    if ((nullptr == p) || (nullptr == pChars)) {
        return;
    }

    char bEscapeActive = false;
    char cLaschar = '\0';
    bool bInsideQuotes = false;
    char cQuote = '\0';

    while (*p != '\0' && (bEscapeActive || bInsideQuotes || (strchr(pChars, *p) == nullptr))) {
        if (!bIgnoreQuotes) {
            if (!bEscapeActive && (*p == '\"' || *p == '\'')) {
                if (bInsideQuotes && cQuote == *p) {
                    cQuote = '\0';
                    bInsideQuotes = false;
                }
                else {
                    cQuote = *p;
                    bInsideQuotes = true;
                }
            }
        }
        else {
            if (!bEscapeActive && (*p == '\"' || *p == '\'')) {
                cQuote = '\0';
            }
        }

        bEscapeActive = (*p == '\\' && cLaschar != '\\'); // escape next char?
        cLaschar = *p;
        p++;
    }
}

void parse_seek_string(const char*& p, const char* pString, char* ptrComment, bool bIgnoreQuotes)
{
    if ((nullptr == p) || (nullptr == pString)) {
        return;
    }
    bool bEscapeActive = false;
    char cLaschar = '\0';
    bool bInsideQuotes = false;
    char cQuote = '\0';
    size_t nStrLen = a_util::strings::getLength(pString);
    int nChar = 0;

    if (ptrComment) {
        ptrComment[nChar] = '\0';
    }
    while (*p != '\0') {
        if (!bIgnoreQuotes) {
            if (!bEscapeActive && !bInsideQuotes && *p == *pString) {
                if (a_util::strings::compare(p, pString, nStrLen) == 0) {
                    if (ptrComment) {
                        ptrComment[nChar] = '\0';
                    }
                    return;
                }
            }

            if (!bEscapeActive && (*p == '\"' || *p == '\'')) {
                if (bInsideQuotes && cQuote == *p) {
                    cQuote = '\0';
                    bInsideQuotes = false;
                }
                else {
                    cQuote = *p;
                    bInsideQuotes = true;
                }
            }
        }
        else {
            if (!bEscapeActive && *p == *pString) {
                if (a_util::strings::compare(p, pString, nStrLen) == 0) {
                    if (ptrComment) {
                        ptrComment[nChar] = '\0';
                    }
                    return;
                }
            }
        }
        bEscapeActive = (*p == '\\' && cLaschar != '\\'); // escape next char?
        cLaschar = *p;
        if (ptrComment) {
            ptrComment[nChar] = *p;
        }
        ++p;
        nChar++;
    }
    if (ptrComment) {
        ptrComment[nChar] = '\0';
    }
}

} // namespace rpc
