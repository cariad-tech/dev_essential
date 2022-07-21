/**
 * @file
 * Command line processing.
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

#include "commandline.h"

#include "parserhelper.h"

#include <a_util/xml.h>

#include <stdlib.h>
#include <string.h>

namespace rpc {
cCommandLine::cCommandLine()
{
    Initialize();
}

cCommandLine::cCommandLine(const std::string& strCommandLine)
{
    Initialize();

    Set(strCommandLine);
}

cCommandLine::cCommandLine(const cCommandLine& oCommandLine)
{
    Initialize();

    Set(oCommandLine);
}

cCommandLine& cCommandLine::operator=(const cCommandLine& oCommandLine)
{
    if (this != &oCommandLine) {
        Initialize();
        Set(oCommandLine);
    }
    return *this;
}

cCommandLine::cCommandLine(int argc, const char** argv)
{
    Initialize();

    Set(argc, argv);
}

cCommandLine::~cCommandLine()
{
    FreeArgs();
}

void cCommandLine::Initialize()
{
    m_strBuffer.clear();
    m_lstFlags.clear();
    m_lstValues.clear();
    m_mapProperties.clear();
    m_lstArgv.clear();

    m_pArgv = nullptr;
    m_nArgc = 0;
}

void cCommandLine::FreeArgs()
{
    if (m_pArgv != nullptr) {
        free(m_pArgv);
        m_pArgv = nullptr;
        m_lstArgv.clear();
    }
}

void cCommandLine::Set(const cCommandLine& oCommandLine)
{
    FreeArgs();
    m_strBuffer = oCommandLine.m_strBuffer;
    m_lstFlags = oCommandLine.m_lstFlags;
    m_lstValues = oCommandLine.m_lstValues;
    m_mapProperties = oCommandLine.m_mapProperties;
}

void cCommandLine::Set(const std::string& strCommandLine)
{
    FreeArgs();
    m_strBuffer = strCommandLine;
    Parse();
}

void cCommandLine::Set(int argc, const char** argv)
{
    FreeArgs();
    if (argv) {
        std::string strCommandLine;
        for (int nIdx = 0; nIdx < argc; nIdx++) {
            if (nIdx > 0) {
                strCommandLine.append(" ");
            }
            if (argv[nIdx]) {
                if (strchr(argv[nIdx], ' ') != nullptr &&
                    !(strchr(argv[nIdx], '\"') != nullptr || strchr(argv[nIdx], '\'') != nullptr)) {
                    strCommandLine.append("\"");
                    strCommandLine.append(argv[nIdx]);
                    strCommandLine.append("\"");
                }
                else {
                    strCommandLine.append(argv[nIdx]);
                }
            }
        }

        return Set(strCommandLine);
    }
}

void cCommandLine::Parse()
{
    if (!m_strBuffer.empty()) {
        m_lstFlags.clear();
        m_lstValues.clear();
        m_mapProperties.clear();

        const char* pCur = m_strBuffer.c_str();

        if (pCur) {
            std::string strElement;
            std::vector<std::string> lstElements;

            while (parse_get_next_word(pCur, strElement)) {
                const char* pCurElement = strElement.c_str();
                if (*pCurElement == '-') {
                    ++pCurElement;
                    if (*pCurElement == 0) {
                        continue;
                    }
                    if (*pCurElement == '-') {
                        ++pCurElement;
                        if (*pCurElement == 0) {
                            continue;
                        }
                    }

                    std::string strName;
                    std::string strValue;

                    while (*pCurElement != '=' && *pCurElement != 0) {
                        strName.push_back(*pCurElement);
                        ++pCurElement;
                    }

                    if (*pCurElement == '=') // check if it is a property
                    {
                        // skip '=' character
                        ++pCurElement;

                        char cQuote = '\0';
                        if (*pCurElement == '\"' || *pCurElement == '\'') {
                            cQuote = *pCurElement;
                            ++pCurElement;
                        }

                        while (*pCurElement != cQuote && *pCurElement != 0) {
                            strValue.push_back(*pCurElement);
                            ++pCurElement;
                        }
                    }

                    if (!strName.empty() && !strValue.empty()) {
                        // fill property map
                        m_mapProperties[strName] = strValue;
                    }
                    else if (!strName.empty()) {
                        // fill flag list
                        m_lstFlags.push_back(strName);
                    }
                }
                else {
                    m_lstValues.push_back(strElement);
                }
            }
        }
    }
}

const char** cCommandLine::GetArgv() const
{
    if (m_pArgv == nullptr) {
        m_nArgc = 0;
        if (m_strBuffer.empty()) {
            return nullptr;
        }

        const char* ptr = m_strBuffer.c_str();
        if (nullptr == ptr) {
            return nullptr;
        }

        parse_skip_whitespace(ptr);

        while (*ptr != '\0') {
            std::string strActWord;
            if (parse_get_next_word(ptr, strActWord)) {
                m_nArgc++;
                m_lstArgv.push_back(strActWord.c_str());
            }
            else {
                break;
            }
        }
        if (m_nArgc > 0) {
            m_pArgv = (const char**)malloc(m_nArgc * sizeof(char*));
            if (m_pArgv == nullptr) {
                return nullptr;
            }
            int nIdx = 0;
            std::list<std::string>::iterator it = m_lstArgv.begin();
            while (nIdx < m_nArgc && nIdx < (int)m_lstArgv.size() && it != m_lstArgv.end()) {
                m_pArgv[nIdx] = it->c_str();
                nIdx++;
                it++;
            }
            m_nArgc = nIdx;
        }
    }
    return m_pArgv;
}
int cCommandLine::GetArgc() const
{
    if (m_pArgv == nullptr) {
        GetArgv();
    }
    return m_nArgc;
}

std::string cCommandLine::GetString() const
{
    return m_strBuffer;
}

std::string cCommandLine::GetProperty(const std::string& strName,
                                      const std::string& strDefault) const
{
    a_util::xml::DOMAttributes::const_iterator it = m_mapProperties.find(strName);
    if (it != m_mapProperties.end()) {
        return it->second;
    }
    return strDefault;
}

bool cCommandLine::GetFlag(const std::string& strName) const
{
    for (std::vector<std::string>::const_iterator it = m_lstFlags.begin(); it != m_lstFlags.end();
         it++) {
        if (*it == strName) {
            return true;
        }
    }

    return false;
}

std::string cCommandLine::GetValue(int nIdx, const std::string& strDefault) const
{
    if ((0 <= nIdx) && (m_lstValues.size() > static_cast<std::size_t>(nIdx))) {
        return m_lstValues.at(nIdx);
    }
    return strDefault;
}

std::string cCommandLine::GetArg(int nIdx, const std::string& strDefault) const
{
    if (nIdx >= GetArgc() || nIdx < 0) {
        return strDefault;
    }
    return GetArgv()[nIdx];
}

const std::string& cCommandLine::GetBuffer() const
{
    return m_strBuffer;
}

const std::vector<std::string>& cCommandLine::GetFlags() const
{
    return m_lstFlags;
}

const std::vector<std::string>& cCommandLine::GetValues() const
{
    return m_lstValues;
}

const std::map<std::string, std::string>& cCommandLine::GetProperties() const
{
    return m_mapProperties;
}

} // namespace rpc
