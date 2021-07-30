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

#ifndef _COMMAND_LINE_CLASS_HEADER_
#define _COMMAND_LINE_CLASS_HEADER_

#include <list>
#include <map>
#include <string>
#include <vector>

namespace rpc {

/**
 *
 * Command line processing.
 *
 *
 * <B>Important things to be aware of:</B>
 *
 * Backslashes are considered escape characters and may lead to unexpected
 * behavior if used incorrectly in the input of cCommandLine.
 * Particular care should be taken when backslashes are used before quote characters.
 * For example in the command line
 * @code
 * -path="C:\Test\" -output="C:\Output"
 * @endcode
 * the backslash after "Test" will escape the double quotes, which
 * will lead to "path" incorrectly becoming the only property and
 * with an incorrect value.
 * To remedy this, the backslash after "Test" needs to be escaped itself
 * using another backslash before it:
 * @code
 * -path="C:\Test\\" -output="C:\Output"
 * @endcode
 * The same problem affects single quotes after backslashes, for example:
 * @code
 * -path='C:\Test\' -output='C:\Output'
 * @endcode
 * When no quotes are used, parameter values ending
 * in backslashes are not a problem,
 * for example the following will be parsed correctly:
 * @code
 * -path=C:\Test\ -output="C:\Output"
 * @endcode
 *
 * <B>Summary:</B>
 *
 * If you want to use backslashes in a commandline,
 * then either use NO quotes or DOUBLE backslashes to escape it
 * (at least directly before a quote character).
 *
 */
class cCommandLine {
protected:
    /// Internal buffer
    std::string m_strBuffer;
    /// List of flags
    std::vector<std::string> m_lstFlags;
    /// List of values
    std::vector<std::string> m_lstValues;
    /// Property map of the commandline
    std::map<std::string, std::string> m_mapProperties;

    /// Argument list
    mutable std::list<std::string> m_lstArgv;
    /// Argument pointer
    mutable const char** m_pArgv;
    /// Argument count
    mutable int m_nArgc;

public:
    /**
     * Default constructor.
     */
    cCommandLine();

    /**
     * Constructor that parses a given command line string.
     * @param strCommandLine [in] The command line string.
     */
    cCommandLine(const std::string& strCommandLine);

    /**
     * Copy constructor.
     * @param oCommandLine [in] The object to copy from.
     */
    cCommandLine(const cCommandLine& oCommandLine);

    /**
     * Assignment operator
     * @param oCommandLine [in] The object to copy from.
     */
    cCommandLine& operator=(const cCommandLine& oCommandLine);

    /**
     * Constructor for standard commandline parameters.
     * @param argc [in] The size of the argv array.
     * @param argv [in] The array of commandline parameters.
     */
    cCommandLine(int argc, const char** argv);

    /**
     * Destructor.
     */
    virtual ~cCommandLine();

    /**
     * Parses a given command line string
     * @param strCommandLine [in] The command line string.
     * @return Standard result.
     */
    void Set(const std::string& strCommandLine);

    /**
     * Copy data from another command line objects.
     * @param oCommandLine [in] The object to copy from.
     * @return Standard result.
     */
    void Set(const cCommandLine& oCommandLine);

    /**
     * Set data using standard commandline parameters.
     * @param argc [in] The size of the argv array.
     * @param argv [in] An array of commandline parameters.
     * @return Standard result.
     */
    void Set(int argc, const char** argv);

    /**
     * Builds a command line string by concatenating all parameters.
     * @return The command line string.
     */
    std::string GetString() const;

    /**
     * Returns a property (-propertyname=value).
     * @param strName [in] The name of the property.
     * @param strDefault [in] A default value that is returned if the property was not specified on
     * the commandline.
     * @return The property value.
     */
    std::string GetProperty(const std::string& strName,
                            const std::string& strDefault = std::string()) const;

    /**
     * Returns whether a flag (-flagname) was specified or not.
     * @param strName [in] The name of the flag.
     * @return tTrue if the flag was specified, tFalse otherwise.
     * @rtsafe
     */
    bool GetFlag(const std::string& strName) const;

    /**
     * Returns a command line parameter that is no flag or property.
     * @param nIdx [in] The index of the parameter to retrieve.
     * @param strDefault [in] A default value that is returned if no parameter with the index was
     * specified.
     * @return [in] The commandline parameter.
     */
    std::string GetValue(int nIdx, const std::string& strDefault = std::string()) const;

    /**
     * Returns a command line argument.
     * @param nIdx [in] The index of the argument to retrieve.
     * @param strDefault [in] A default value that is returned if no argument with the index was
     * specified.
     * @return [in] The commandline argument.
     */
    std::string GetArg(int nIdx, const std::string& strDefault = std::string()) const;

public:
    /**
     * Returns a reference to the internal commandline buffer.
     * @return A reference to the internal commandline buffer.
     * @rtsafe
     */
    const std::string& GetBuffer() const;
    /**
     * Returns a list of all specified flags.
     * @return A list of all specified flags.
     * @rtsafe
     */
    const std::vector<std::string>& GetFlags() const;

    /**
     * Returns a list of all specified values.
     * @return A list of all specified values.
     * @rtsafe
     */
    const std::vector<std::string>& GetValues() const;

    /**
     * Returns a map of all specified properties.
     * @return A map of all specified properties.
     * @rtsafe
     */
    const std::map<std::string, std::string>& GetProperties() const;

    /**
     * Returns a pointer to a standard style command line parameter array.
     * @return An array of commandline parameters.
     */
    const char** GetArgv() const;

    /**
     * Returns the size of the array retreived by @ref GetArgv().
     * @return The size of the array retreived by @ref GetArgv().
     */
    int GetArgc() const;

protected:
    /**
     * Parses the internal buffer
     * @return Standard result.
     */
    void Parse();

    /**
     * Initializes internal parameters
     * @return Standard result.
     */
    void Initialize();

    /**
     * Frees the argument list
     * @return Standard result.
     */
    void FreeArgs();
};

} // namespace rpc

#endif // _COMMAND_LINE_CLASS_HEADER_
