/**
 * @file
 * Public API for @ref a_util::filesystem::Path "Path" type and functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef _A_UTILS_UTIL_FILESYSTEM_PATH_INCLUDED_
#define _A_UTILS_UTIL_FILESYSTEM_PATH_INCLUDED_

#include <memory>
#include <stdexcept>

namespace a_util {
namespace filesystem {
// forward declarations
class Path;
Path getWorkingDirectory();

/// Exception class used by Path methods
class PathException : public std::runtime_error {
public:
    /**
     * CTOR
     * @param[in] message The message the exception distributes.
     */
    PathException(const std::string& message);
};

/**
 * File/Directory path class
 * @details This class represents paths on the filesystem. Only syntactic aspects of paths are
 * handled, the path may or may not exist and might not even be allowed to exist on the current
 * platform.
 */
class Path {
public:
    /// Path separator types
    typedef enum {
        PS_ForwardSlash = 0,  //!< Unix slash
        PS_BackwardSlash = 1, //!< Windows slash
        PS_Native = 2         //!< Native slash, depending on the current platform
    } PathSeparator;

public:
    /// Default CTOR for an empty path
    Path();

    /**
     * Copy CTOR
     * @param[in] path The instance to copy from
     */
    Path(const Path& path);

    /**
     * Assignment Operator
     * @param[in] path The instance to copy from
     * return Reference to @c *this
     */
    Path& operator=(const Path& path);

    /**
     * Initializing CTOR
     * @note If the path is invalid, the instance is initialized to the empty state
     * @param[in] path The file or directory path to initialize the instance
     */
    Path(const std::string& path);

    /**
     * Initializing CTOR
     * @note If the path is invalid, the instance is initialized to the empty state
     * @param[in] path The file or directory path to initialize the instance
     */
    Path(const char* path);

    /// DTOR
    ~Path();

    /**
     * Converts the path to the systems native string representation
     * @retval The string representation of the path (or empty string for empty path),
     *         using the platform native separator
     */
    operator std::string() const;

    /**
     * Get the string representation of the current path using the specified separator
     * @param[in] separator The separator to use
     * @retval The string representation of the path (or empty string for empty path)
     */
    std::string toString(PathSeparator separator = PS_Native) const;

    /**
     * Set the current path of the instance, handling platform aspects or any trailing slashes.
     * @param[in] path The new path
     * @return @c false if the path is invalid and cannot be set, @c true on success
     */
    bool setPath(const std::string& path);

    /**
     * Appends a relative file or directory path or another file extension to the current path
     * @note This method will throw a PathException if path is absolute
     * @param[in] path The (relative) (file-)path or file extension to append
     * @return @c *this
     * @throw PathException If @c path is absolute.
     */
    Path& append(const Path& path);

    /**
     * Get the root path of the directory/file
     * @note: For network paths, the host is returned (e.g. "\\netdrive\test" returns "\\netdrive")
     * @details Usually "C:" or "D:" on Windows, on Unix-like systems usually "/".
     *          e.g.: "/dir/subdir/file.txt" returns "/", "C:/dir" returns "C:"
     * @return A path instance pointing to the root directory
     */
    Path getRoot() const;

    /**
     * Get the parent path of the directory or file
     * @details e.g.: "/dir/subdir/file.txt" returns "/dir/subdir", "/dir/subdir" returns "/dir"
     * @note If called on a root path, *this is returned
     * @return A path instance pointing to the parent directory (or root)
     */
    Path getParent() const;

    /**
     * Get the name of the directory or file
     * @details e.g.: "/dir/subdir/file.txt" returns "file.txt", "/dir/subdir" returns "subdir"
     * @return The last element of the path
     */
    Path getLastElement() const;

    /**
     * Get the extension of the file (without the '.')
     * @details e.g.: "/dir/subdir/file.txt" returns "txt", "/dir/subdir" returns ""
     * @return The extension or an empty string if the path is a directory or has no extension
     */
    std::string getExtension() const;

    /**
     * Append a string to a basename
     * @note If the path is a file path, the string is inserted before the extension.
     * @note If it's a directory path, the string is added.
     * @param[in] str String to append to the basename
     * @retval *this with @c str appended to the basename
     */
    Path& appendToBasename(const std::string& str);

    /**
     * Replace the extension of the path
     * @note If the path is a file path, the extension is replaced. If it's a directory path,
     *       the extension is added, making it a file path in the process
     * @param [in] extension The new extension (without '.')
     * @return @c *this
     */
    Path& replaceExtension(const std::string& extension);

    /**
     * Remove the last part of the path, making it a directory path.
     * @note Does nothing if the current path already is at the root.
     * @return @c *this
     */
    Path& removeLastElement();

    /**
     * Make the current path canonical.
     * @note If the path is already canonical, no changes are made
     * @details This means that all occurences of '.' and '..' are evaluated syntactically.
     *          e.g.: "/./dir/../dir/file.txt" is replaced by "/file.txt".
     * @return @c *this
     */
    Path& makeCanonical();

    /**
     * Convert the path to an absolute path relative to the current working directory
     * @return @c *this containing the absolute path
     */
    Path& makeAbsolute();

    /**
     * Convert the path to an absolute path relative to the given @c base_path
     * @param[in] base_path Reference to which the absolute path is converted
     * @return @c *this containing the absolute path
     */
    Path& makeAbsolute(Path base_path);

    /**
     * Convert the path to a relative path relative to the current working directory
     * @return @c *this containing the relative path
     */
    Path& makeRelative();

    /**
     * Convert the path to a relative path relative to the @c input_path
     * @param[in] input_path Reference to which the relative path is converted
     * @return @c *this containing the relative path
     * @throw PathException If the current path and the @c input_path have different roots
     */
    Path& makeRelative(Path input_path);

    // Makes the current path relative to a specified parent directory (default: working directory)
    // @note If the path is already relative, no changes are made!
    // @param [in] oParent The parent directory
    // @return Returns *this, to be able to chain additional method calls
    // Path& MakeRelative(const Path& oParent = filesystem::getWorkingDirectory());

    // Makes the current path absolute, using a specified parent directory (default: working
    // directory)
    // @note If the path is already absolute, no changes are made!
    // @param [in] oParent The parent directory
    // @return Returns *this, to be able to chain additional method calls
    // Path& MakeAbsolute(const Path& oParent = filesystem::getWorkingDirectory());

    /**
     * Returns whether the current path is relative
     * @retval true if the path is relative, false otherwise
     * @throw PathException if the current path is empty
     */
    bool isRelative() const;

    /**
     * Returns whether the current path is absolute
     * @note This Method throws a PathException is the current path is empty
     * @retval true if the path is absolute, false otherwise
     */
    bool isAbsolute() const;

    /**
     * Returns whether the current path is empty
     * @retval true if the path is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * Clears the path and resets it to the empty state
     * @return Returns *this, to be able to chain additional method calls
     */
    Path& Clear();

private:
    class Implementation;
    std::unique_ptr<Implementation> _impl;
    friend bool operator==(const Path& lhs, const Path& rhs);
};

/**
 * Check two paths for equality
 * @note The paths are compared syntactically, any canonicalization must be done manually!
 * @param[in] lhs Right hand side object
 * @param[in] rhs Left hand side object
 * @retval true if the paths are equal, false otherwise
 */
bool operator==(const Path& lhs, const Path& rhs);

/**
 * Check two paths for inequality
 * @note The paths are compared syntactically, any canonicalization must be done manually!
 * @param[in] lhs Right hand side object
 * @param[in] rhs Left hand side object
 * @retval true if the paths are not equal, false otherwise
 */
bool operator!=(const Path& lhs, const Path& rhs);

/**
 * Performs stream output on the path p
 * @note The implemenation simply calls toString() without arguments.
 * @note This mean that this operator doesn't support a non-default separator
 * @param [in] os the output stream
 * @param [in] p path to print
 * @retval the output stream
 */
std::ostream& operator<<(std::ostream& os, const Path& p);

/**
 * Appends two paths, forming a new path
 * @param [in] a The left path
 * @param [in] b The right path
 * @retval A new path combined from a and b (or empty, in case of an error)
 */
Path operator+(const Path& a, const Path& b);

} // namespace filesystem
} // namespace a_util

#endif // _A_UTILS_UTIL_FILESYSTEM_PATH_INCLUDED_
