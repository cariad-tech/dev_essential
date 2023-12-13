/**
 * @file
 * Public API for @ref a_util::filesystem "filesystem" functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef _A_UTILS_UTIL_FILESYSTEM_FILESYSTEM_INCLUDED_
#define _A_UTILS_UTIL_FILESYSTEM_FILESYSTEM_INCLUDED_

#include <a_util/filesystem/path.h>

#include <string>
#include <vector>

namespace a_util {
namespace filesystem {
/// Enumerates possible error values for filesystem interaction
typedef enum {
    OK,              //!< No error, basically meaning success
    OPEN_FAILED,     //!< Opening a file or folder from the filesystem failed
    GENERAL_FAILURE, //!< General errors
    IO_ERROR,        //!< Input/output error during filesystem access
    INVALID_PATH,    //!< Accessed path on the filesystem is invalid
    ACCESS_DENIED    //!< Access to file or folder in the filesystem denied
} Error;

/// Enumeration for possible listings in a directory, usable in function enumDirectory()
typedef enum {
    ED_FILES = 0x1,      //!< List files in the directory
    ED_DIRECTORIES = 0x2 //!< List directories in the directory
} EnumDirFlags;

/**
 * Available options for operations comparing files or directories.
 * @todo Satisfies the requirements of a
 *       <a href="https://en.cppreference.com/w/cpp/named_req/BitmaskType">C++ BitmaskType</a>.
 */
enum class DiffOptions : std::uint64_t {
    none = 0 //!< Default behavior; needs to be specified by the using function.
};

/**
 * Retrieve path to the temp directory of the system
 * @return Temp directory of the system
 */
Path getTempDirectory();

/**
 * Retrieve path to the working directory of the current process
 * @return Current working directory path
 */
Path getWorkingDirectory();

/**
 * Set the working directory
 * @param[in] path Path to the new working directory
 * @retval INVALID_PATH The path is invalid/nonexisting or a file path
 * @retval GENERAL_FAILURE The OS failed to set the working directory
 * @retval OK Working directory was successfully set
 */
Error setWorkingDirectory(const Path& path);

/**
 * Read entire content of a text file into a string
 * @param[in] file_path Path to the file to be read
 * @param[out] content Destination string to be filled
 * @retval OPEN_FAILED File could not be opened
 * @retval GENERAL_FAILURE Seeking the file did not succeed
 * @retval IO_ERROR Some I/O error occurred while reading the file
 * @retval OK Content successfully read and copied to out parameter @c content
 */
Error readTextFile(const Path& file_path, std::string& content);

/**
 * Read entire content of a text file and split it every new line, dropping the EOL characters
 * @param[in] file_path Path to the file to be read
 * @param[out] vec_lines Vector of strings with each string containing one line of the file
 * @return Same values documented in @ref readTextFile()
 */
Error readTextLines(const Path& file_path, std::vector<std::string>& vec_lines);

/**
 * Open a file in mode 'w' and write new content to it.
 * @param[in] file_path Path to the file to (over-)write
 * @param[in] content Content to write to the file
 * @retval OPEN_FAILED File could not be opened
 * @retval IO_ERROR Some I/O error occurred while writing the file
 * @retval OK Content successfully read and copied to out parameter @c content
 */
Error writeTextFile(const Path& file_path, const std::string& content);

/**
 * List all entries in a directory
 * @param[in] dir_path Path to the directory to list the entries from
 * @param[out] vec_entries All entries listed in the directory
 * @param[in] flags @ref EnumDirFlags filtering what to list in the directory
 * @retval INVALID_PATH The path does not exist or is not a directory
 * @retval GENERAL_FAILURE Some unknown error while listing entries in the directory occurred
 * @retval OK Entries listed successfully, with the list being copied to @c vec_entries
 */
Error enumDirectory(const Path& dir_path,
                    std::vector<Path>& vec_entries,
                    std::int32_t flags = ED_FILES | ED_DIRECTORIES);

/**
 * Check whether the file or directory exists on the filesystem
 * @param[in] path Path to the file or directory
 * @return @c true if the path exists, @c false otherwise
 */
bool exists(const Path& path);

/**
 * Create a new directory on the filesystem
 * @note All given non-existing subdirectories are created as well
 * @param[in] path Path to the directory to create
 * @return @c true if the directory was created or already exists, @c false otherwise
 */
bool createDirectory(const Path& path);

/**
 * Check whether the path points to a file on the filesystem
 * @param[in] file_path Path to the file to check
 * @return @c true if the path points to a file, @c false otherwise
 */
bool isFile(const Path& file_path);

/**
 * Check whether the path points to a directory on the filesystem
 * @param[in] dir_path Path to the directory to check
 * @return @c true if the path points to a directory, @c false otherwise
 */
bool isDirectory(const Path& dir_path);

/**
 * Remove the file or directory pointed to by @c path.
 * @note Directories and their content are deleted recursively.
 * @param[in] path Path to the element to be deleted.
 * @return @c true if the element could be deleted, @c false otherwise.
 */
bool remove(const Path& path);

/**
 * Rename or move a file or directory from @c path to @c new_path.
 * @param[in] path Path to the file or directory to be renamed or moved.
 * @param[in] new_path Path the file or directory is renamed of moved to.
 * @return @c true on success, @c false otherwise.
 */
bool rename(const Path& path, const Path& new_path);

/**
 * Compare content of two files.
 * @param[in] p1 Path to first file.
 * @param[in] p2 Path to second file.
 * @retval 0 File contents are equal or p1 and p2 are the same file.
 * @retval >0 Position of the first different character in the files.
 * @retval <0 Comparison not possible, e.g. file does not exist or path is a directory.
 * @throw ios_base::failure exception if cannot open the file, or try to access character
 * after the end of file, or no character were written or error was set in ifstream.
 */
std::int64_t compareFiles(const Path& p1, const Path& p2);

/**
 * @param[in] options Options controlling the behavior of the diff operation. Supported options:
 *                    - @ref DiffOptions::none Files are bitwise equal
 * @copydoc compareFiles(const Path& p1, const Path& p2)
 */
std::int64_t compareFiles(const Path& p1, const Path& p2, DiffOptions options);

} // namespace filesystem
} // namespace a_util

#endif // _A_UTILS_UTIL_FILESYSTEM_FILESYSTEM_INCLUDED_
