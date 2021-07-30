/**
 * @file
 * Implements a simple class to gain access to the necessary information provided by a URL
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

#include "url.h"

#include "a_util/regex.h"
#include "a_util/strings/strings_convert.h"
#include "a_util/strings/strings_functions.h"

namespace rpc {

cUrl::cUrl()
{
    m_bIsValidURL = false;
}

cUrl::cUrl(const std::string& strUrl)
{
    m_bIsValidURL = Validate(strUrl);
}

cUrl::~cUrl()
{
    // nothing to do...
}

bool cUrl::IsValid() const
{
    return m_bIsValidURL;
}

bool cUrl::Validate(const std::string& strUrl)
{
    m_bIsValidURL = false;
    // do this in stages - get the uri scheme, the auth data, the location and all the trailing
    // stuff first do an almost entire matching here to be safe about the urls integrity.. note:
    // cRegEx does NOT support cStrings....too bad attention: Watch out for trigraph replacements
    // here (e.g. '??(' substitutes to ']')
    // http://msdn.microsoft.com/de-de/library/bt0y4awe(v=vs.90).aspx
    a_util::regex::RegularExpression oUrlExp(
        "^([^\\+\\-\\.][\\w\\+-_]+[^\\+-\\.])://([\\w:@\\.-]+)/?(/([\\w\\/"
        "\\.%\\-_\\~]+))?\\?\?(\\?([^\\s#]+))?#?(#(\\w+))?$");
    std::string strScheme;    // mandatory
    std::string strAuthority; // mandatory
    std::string strPath;      // optional
    std::string strQuery;     // optional
    std::string strFragment;  // optional
    std::string strDummy;

    if (!(oUrlExp.fullMatch(strUrl,
                            strScheme,
                            strAuthority,
                            strDummy,
                            strPath,
                            strDummy,
                            strQuery,
                            strDummy,
                            strFragment))) {
        // if we cannot match a valid STD-66 URL convert any proprietary MB query to STD-66 and
        // re-run this.
        a_util::regex::RegularExpression oMBUrlExp("^.*{([\\w=,]*)}.*$");
        std::string strStdMBQuery;
        if (!(oMBUrlExp.fullMatch(strUrl, strStdMBQuery))) {
            return false;
        }
        std::string strMBQuery(strStdMBQuery);
        a_util::strings::replace(strMBQuery, ",", "&");
        strMBQuery = "?" + strMBQuery;

        oMBUrlExp = a_util::regex::RegularExpression("{.*}");
        std::string strQueryPcs(strMBQuery);
        std::string strStrUrl(strUrl);

        //????????
        /*  if (!(oMBUrlExp.Replace(strQueryPcs, &strStrUrl)))
          {
              RETURN_ERROR(ERR_UNEXPECTED);
          }   */
        return Validate(strStrUrl);
    }

    if (strAuthority.empty()) {
        return false;
    }

    if (strScheme.empty()) {
        return false;
    }

    m_sComponents.strPath = std::string(strPath);
    m_sComponents.strScheme = strScheme;
    m_sComponents.strFragment = std::string(strFragment);

    m_sComponents.oAuthority = cAuthority(std::string(strAuthority));
    if (!m_sComponents.oAuthority.IsValid()) {
        return false;
    }

    m_sComponents.oQuery = cQuery(strQuery);
    if (!m_sComponents.oQuery.IsValid()) {
        return false;
    }

    m_bIsValidURL = true;

    m_strFullUriString = strUrl; // make a local copy
    return true;
}

cUrl::tURIComponents cUrl::GetComponents() const
{
    return m_sComponents;
}

std::string cUrl::GetScheme() const
{
    return m_sComponents.strScheme;
}

cUrl::cAuthority cUrl::GetAuthority() const
{
    return m_sComponents.oAuthority;
}

std::string cUrl::GetPath() const
{
    return m_sComponents.strPath;
}

cUrl::cQuery cUrl::GetQuery() const
{
    return m_sComponents.oQuery;
}

std::string cUrl::GetFragment() const
{
    return m_sComponents.strFragment;
}

std::string cUrl::AsString() const
{
    return m_strFullUriString;
}

// cUrl::cAuthority
//#####################################################################

cUrl::cAuthority::cAuthority(const std::string& strAuthority)
{
    m_strAuthority = strAuthority;
    m_sAuthority.nPort = 0;
    m_bIsValid = Validate(m_strAuthority);
}

cUrl::cAuthority::~cAuthority()
{
    m_bIsValid = false;
}

bool cUrl::cAuthority::Validate(const std::string& strAuthority)
{
    if (strAuthority.empty()) {
        return false;
    }

    a_util::regex::RegularExpression oUrlExp("^((\\w+)(:(\\w+))?@)?([\\w\\.-]+)(:([\\d]+))?$");
    std::string strUser;     // optional
    std::string strPassword; // optional
    std::string strHost;     // mandatory
    std::string strPort;     // mandatory
    std::string strDummy;

    if (!(oUrlExp.fullMatch(
            strAuthority, strDummy, strUser, strDummy, strPassword, strHost, strDummy, strPort))) {
        return false;
    }

    if (strHost.empty() || strHost == " ") {
        return false;
    }

    m_sAuthority.strLocation = strHost;
    m_sAuthority.strUser = strUser;
    m_sAuthority.strPassword = strPassword;
    m_sAuthority.nPort = a_util::strings::toUInt16(strPort);

    return true;
}

std::string cUrl::cAuthority::GetUser() const
{
    return m_sAuthority.strUser;
}

std::string cUrl::cAuthority::GetPassword() const
{
    return m_sAuthority.strPassword;
}

std::string cUrl::cAuthority::GetHost() const
{
    return m_sAuthority.strLocation;
}

uint16_t cUrl::cAuthority::GetPort() const
{
    return m_sAuthority.nPort;
}

bool cUrl::cAuthority::IsValid() const
{
    return m_bIsValid;
}

std::string cUrl::cAuthority::AsString() const
{
    return m_strAuthority;
}

bool cUrl::cAuthority::IsValidIPv4() const
{
    a_util::regex::RegularExpression oReg(
        "\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25["
        "0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    bool bIsIp4 = oReg.fullMatch(m_sAuthority.strLocation);
    return bIsIp4;
}

// cUrl::cQuery
//#####################################################################

cUrl::cQuery::cQuery(const std::string& strQuery)
{
    m_strQueryFull = strQuery;
    m_bIsValid = Validate(strQuery);
}

cUrl::cQuery::~cQuery()
{
    // nothing to do here...
}

bool cUrl::cQuery::Validate(const std::string& strQuery)
{
    if (strQuery.empty()) {
        return true;
    }

    std::string strParam; // Option storage
    std::string strValue; // Value storage
    std::string strDummy; // Dummy

    a_util::regex::RegularExpression oQueryExp("([\\w\\d]+)(=([\\w\\d]+))?&?");
    std::string strQueryPieces(strQuery);
    int nCount = 0;
    while (oQueryExp.fullMatch(strQueryPieces, strParam, strDummy, strValue)) {
        m_mapQuery.insert(
            std::make_pair<std::string, std::string>(std::string(strParam), std::string(strValue)));
        nCount++;
    }

    if (0 == nCount) // && !strQuery.IsEmpty()
    {
        return false;
    }

    return true;
}

std::string cUrl::cQuery::AsString() const
{
    return m_strQueryFull;
}

std::string cUrl::cQuery::GetValue(const std::string& strParam, const std::string& strDefault) const
{
    std::map<std::string, std::string>::const_iterator it = m_mapQuery.find(strParam);
    if (m_mapQuery.end() != it) {
        return it->second;
    }
    else {
        return strDefault;
    }
}

bool cUrl::cQuery::IsValid() const
{
    return m_bIsValid;
}

} // namespace rpc
