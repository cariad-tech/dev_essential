/**
 * @file
 * Implements a simple class to gain access to the necessary information provided by a URL
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __URL_PARSER_
#define __URL_PARSER_

#include <map>
#include <string>

namespace rpc {
/**
 * @brief Class for general URL representation, handling and verification against
 *        the IETF-STD66
 *
 * @details The URL class simplifies the access to the values of any URL, passed
 * as cString. It has to fulfill the following scheme pattern:
 * @code
 * $uri-scheme$://$user$:$password$@$host_or_address$:$port$/?$option1$&$option2$=$value2$#$fragment$
 * @endcode
 *
 * @see http://tools.ietf.org/html/std66 for the full IETF specification.
 *
 * This class aims to handle any URL complying to STD66. However, for reasons of
 * backwards compatibility, the proprietary syntax of the URLs introduced in
 * earlier utils pattern is supported as well.<br>
 * STD-66 URLs might bear the following structure:
 * @li http://127.0.0.1:80/?myquery=myvalue&setvalue#myfragment
 * @li ssh+tcp://user:pw@1.2.3.4:22/?keepalive=true
 * @li scp+tcp://1.2.3.4/?timeout=1
 *
 * Interpretation of the individual parts, however, depend on upper application
 * layers linking against the A_UTILS library.
 *
 * In contrast to STD-66 URLs, proprietary URLs with following scheme are supported
 * @li tcp://127.0.0.1:5001{timeout=1000,max_pkg_size=65000}
 * @li udp://127.0.0.1:5002{timeout=1000,max_pkg_size=65000}
 *
 * Usage:
 * After construction you should call function @ref cUrl::IsValid() to determine
 * the URL's validity. If it is found to be valid, @ref cUrl::cQuery and @ref
 * cUrl::cAuthority along with the Getter methods can be used to extract the
 * required URL parts and subsets.
 */
class cUrl {
public:
    /**
     * Subclass representing and handling the query subset as part of @ref cUrl
     * @param strQuery  The query part of a particular URL
     * @note A Query consists of single parameters (simple 'set' or 'not set')
     *       and parameters with an associated value in the style of
     *       '?paramter1&parameter1=value'
     */
    class cQuery {
        friend class cUrl;

    public:
        /**
         * Constructor for a Query subset of any URL
         * @param strQuery The query part of a particular URL
         */
        cQuery(const std::string& strQuery = "");
        /**
         * Destructor
         */
        ~cQuery();

    public:
        /**
         * Validation procedure for the query subset
         * @param strQuery The query part of a particular URL if not already
         *        given through the constructor
         * @retval true A valid Query has been given.
         * @retval false The query is syntactically invalid
         */
        bool Validate(const std::string& strQuery);

        /**
         * Method to read individual values of the query by their parameters
         * @param strParameter The parameter to read from
         * @param strDefault Default return value
         * @return The value associated with the given parameter name; if the
         *         parameter has not been found \p strDefault is returned.
         */
        std::string GetValue(const std::string& strParameter,
                             const std::string& strDefault = "") const;

        /**
         * Getter method to retrieve the raw query string
         * @return Raw query data.
         */
        std::string AsString() const;

        /** Checks for the query's validity
         * @retval tFalse if query subset is invalid or has not yet been
         *              validated.
         * @retval tTrue if the query subset is valid and the values could
         *              be extracted successfully.
         */
        bool IsValid() const;

    private:
        ///< Boolean flag used to determine if the url subset has already been verified
        bool m_bIsValid;
        ///< The unparsed query string
        std::string m_strQueryFull;
        ///< Map to store the the query's value paris in
        std::map<std::string, std::string> m_mapQuery;
    };

    /**
     * Subclass representing and handling the authority subset as part of @ref cUrl
     * @param strAuthority The authority subset of any given URL
     * @note The authority subset includes user authentication (optional) and
     *       the host that is referred to by cUrl (mandatory)in the style of
     *       \code 'user:password@host:port' \endcode where the password and
     *       the user can be omitted. If the host is not specified through a
     *       IPvX address but any other authority identifier (e.g. SHM Key)
     *       the port can be omitted.
     */
    class cAuthority {
        friend class cUrl;

    public:
        /**
         * Constructor of the authority subset of any URL
         * @param strAuthority the authority part of a particular URL
         */
        cAuthority(const std::string& strAuthority = "");

        /**
         * Destructor
         */
        ~cAuthority();

    public:
        /**
         * Validation method for the authority subset
         * @param strAuthority The (mandatory) authority bit of particular URL
         *        if not already given through the constructor
         * @retval @ref a_util::result::SUCCESS A valid authority has been given.
         * @retval ERR_INVALID_ARG The authority is syntactically invalid
         *              or incomplete
         */
        bool Validate(const std::string& strAuthority);
        // tBool isValidIPv6(); // ipv6 is enclosed in square brackets [ IPv6 ]

        /**
         * Retrieves the username if set.
         * @return The username if any. Otherwise @ref std::string::Empty
         */
        std::string GetUser() const;

        /**
         * Retrieves the password if set.
         * @return  The password if any. Otherwise @ref cString::Empty
         */
        std::string GetPassword() const;

        /**
         * Retrieves the host bit of the given URL
         * @return The host address without port; @ref cString::Empty if authority
         *         subset is invalid.
         */
        std::string GetHost() const;
        /**
         * The Host is a valid 1Pv4 adress.
         */
        bool IsValidIPv4() const;
        /**
         * Retrieves the host port of the given URL is applicable
         * @return The host port; 0 if authority subset is invalid or not applicable
         */
        uint16_t GetPort() const;

        /** Checks for the authority's validity
         * @retval tFalse if authority subset is invalid or has not yet been
         *              validated.
         * @retval tTrue if the authority subset is valid and the values
         *              could be extracted successfully.
         */
        bool IsValid() const;

        /**
         * Getter method to retrieve the raw query string
         * @return Raw query data.
         */
        std::string AsString() const;

    private:
        ///< The raw, unparsed authority string
        std::string m_strAuthority;
        ///< Boolean flag used to determine if the url subset has already been verified
        bool m_bIsValid;
        ///< Working data representing the individual parts of any URL's authority subset
        struct sAuthority {
            ///< The username if any
            std::string strUser;
            ///< The password if any
            std::string strPassword;
            ///< The location e.g. 127.0.0.1:80 or 0x1234
            std::string strLocation;
            ///< The port if applicable
            uint16_t nPort;
        } m_sAuthority;
    };

public:
    /** URL component datatype */
    struct tURIComponents {
        tURIComponents(){};
        /**
         * The URL scheme (e.g. http, tcp, etc) - mandatory.
         */
        std::string strScheme;
        /**
         * The authority bit of the URL including user authentication and the hosts
         * address parameters.
         */
        cAuthority oAuthority;
        /**
         * The optional resource path on the referenced host
         */
        std::string strPath;
        /**
         * An optional query related to the referenced host or the referenced
         * resource thereon
         */
        cQuery oQuery;
        /**
         * An optional related to the referenced resource
         */
        std::string strFragment;
    };

public:
    /**
     * Constructor.
     * @param strUrl The full URL that shall be abstracted and validated.
     */
    cUrl(const std::string& strUrl);

    /**
     * Default constructor
     * @note When using the default constructor, @ref cUrl::Validate() has to be
     *       called separately.
     */
    cUrl();

    /**
     * Destructor.
     */
    ~cUrl();

public:
    /**
     * @brief Validation of the URL object. If given url is valid it will
     *        be also set for current object.
     *
     * @post The values are abstracted and stored by @ref cUrl::cAuthority and
     *       @ref cUrl::cQuery
     * @param  strUrl The URL string to set and validate.
     * @retval @ref a_util::result::SUCCESS A valid URL has been given and it's subset have
     *              successfully been read
     * @retval ERR_INVALID_ARG The URL is syntactically invalid or incomplete
     */
    bool Validate(const std::string& strUrl);

    /**
     * Checks for the URL's validity
     * @retval tFalse if the URL or any of it's subsets are invalid or have
     *              not yet been validated.
     * @retval tTrue if the URL is valid and it's values could be extracted
     *              successfully.
     */
    bool IsValid() const;

    /**
     * Getter method to read the components of the currently abstracted URL
     * @return The components.
     * @note Using this method, the developer has to assure the validity of the
     *       URL object separately.
     */
    tURIComponents GetComponents() const;

    /**
     * Getter method to read the scheme bit of the URL object
     * @return The scheme string.
     */
    std::string GetScheme() const;

    /**
     * Getter method to read the authority  bit of the URL object
     * @return The authority string (not validated).
     */
    cAuthority GetAuthority() const;

    /**
     * Getter method to read the path bit of the URL object
     * @return The path string if any.
     */
    std::string GetPath() const;

    /**
     * Getter method to read the query bit of the URL object
     * @return The query string if any (not validated).
     */
    cQuery GetQuery() const;

    /**
     * Getter method to read the fragment bit of the URL object
     * @return The fragment string if any.
     */
    std::string GetFragment() const;

    /**
     * Exports the URL back as string.
     * @return The url string.
     */
    std::string AsString() const;

private:
    ///< Working data representing the individual parts of any full URL's
    tURIComponents m_sComponents;
    ///< Boolean flag used to determine if the URL has already been verified
    bool m_bIsValidURL;
    ///< The URL as string representation
    std::string m_strFullUriString;
};

} // namespace rpc

#endif // _A_UTILS_URL_PARSER_
