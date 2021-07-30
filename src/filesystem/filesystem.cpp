/**
 * @file
 * Class for file system access. This class wraps all function calls for file system access.
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

// for Read/writeTextFile, until File class is available
#include "a_util/filesystem.h"

#include "a_util/preprocessor/detail/disable_warnings.h"
#include "a_util/strings/strings_functions.h"
#include "a_util/strings/unicode.h"

#if defined(__QNX__) && !defined(_QNX_SOURCE) // scandir, alphasort
#define _QNX_SOURCE
#endif

#ifdef WIN32
// needed before including "a_util/filesystem.h"
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else

#include <dirent.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif // WIN32

#include <cstdio> // rename
#include <fstream>
#include <iostream>
#include <limits> // std::numeric_limits

using namespace a_util;
using namespace a_util::filesystem;

namespace a_util {
namespace filesystem {

// Internal linkage
namespace {

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

/// Enumeration of file attributes
enum FileAttributeFlag : std::int32_t {
#ifdef WIN32
    // map to Win32 definitions
    ATTRIBUTE_READONLY = 0x00000001,
    ATTRIBUTE_FOLDER = 0x00000010,
    ATTRIBUTE_HIDDEN = 0x00000002,
    ATTRIBUTE_SYSTEM = 0x00000004,
    ATTRIBUTE_ARCHIVE = 0x00000020,
    ATTRIBUTE_COMPRESSED = 0x00000800,
    ATTRIBUTE_ENCRYPTED = 0x00004000,
    ATTRIBUTE_OFFLINE = 0x00001000,
#else
    ATTRIBUTE_READONLY = 1,
    ATTRIBUTE_FOLDER = 2,
    ATTRIBUTE_HIDDEN = 4,
    ATTRIBUTE_SYSTEM = 8,
    ATTRIBUTE_ARCHIVE = 16,
    ATTRIBUTE_COMPRESSED = 32,
    ATTRIBUTE_ENCRYPTED = 64,
    ATTRIBUTE_OFFLINE = 128,
#endif
    ATTRIBUTE_SYMLINK = 256,
    ATTRIBUTE_PIPE = 512,
    ATTRIBUTE_BLOCKDEVICE = 1024,
    ATTRIBUTE_CHARDEVICE = 2048,
    ATTRIBUTE_INVALID =
        std::numeric_limits<typename std::underlying_type<FileAttributeFlag>::type>::max()
};

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

int32_t GetFSAttributes(const Path& path)
{
    std::string file_path = path.toString();

#ifdef WIN32
    int32_t file_attribs = ::GetFileAttributesW(strings::unicode::utf8ToUtf16(path).c_str());
    if (file_attribs == 0xFFFFFFFF)
        return ATTRIBUTE_INVALID;

    return file_attribs;

#else

    int32_t file_attribs = 0;
    struct stat64 file_info = {};
    int result = stat64(file_path.c_str(), &file_info);

    if (result == -1) {
        return ATTRIBUTE_INVALID;
    }

    if (S_ISDIR(file_info.st_mode)) {
        file_attribs |= ATTRIBUTE_FOLDER;
    }

    if (S_ISBLK(file_info.st_mode)) {
        file_attribs |= ATTRIBUTE_BLOCKDEVICE | ATTRIBUTE_SYSTEM;
    }

    if (S_ISCHR(file_info.st_mode)) {
        file_attribs |= ATTRIBUTE_CHARDEVICE | ATTRIBUTE_SYSTEM;
    }

    if (S_ISFIFO(file_info.st_mode)) {
        file_attribs |= ATTRIBUTE_PIPE | ATTRIBUTE_SYSTEM;
    }

    if (S_ISLNK(file_info.st_mode)) {
        file_attribs |= ATTRIBUTE_SYMLINK;
    }

    if (access(file_path.c_str(), W_OK) != 0) {
        file_attribs |= ATTRIBUTE_READONLY;
    }

    return file_attribs;

#endif
}

} // namespace
} // namespace filesystem
} // namespace a_util

Path filesystem::getTempDirectory()
{
#ifdef WIN32
    TCHAR buffer[MAX_PATH + 1];
    if (!::GetTempPath(sizeof(buffer), buffer)) {
        return Path();
    }

    return Path(buffer);
#else
    return Path("/tmp");
#endif
}

Path filesystem::getWorkingDirectory()
{
#ifdef WIN32
    DWORD required = ::GetCurrentDirectoryW(0, NULL);
    std::wstring buffer(required - 1, 0x0000);

    if (!::GetCurrentDirectoryW(required, &buffer[0])) {
        return Path();
    }
    return Path(a_util::strings::unicode::utf16ToUtf8(buffer));

#else
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        return Path();
    }
    return Path(buffer);
#endif
}

Error filesystem::setWorkingDirectory(const Path& path)
{
    if (isDirectory(path)) {
#ifdef WIN32
        if (!::SetCurrentDirectoryW(strings::unicode::utf8ToUtf16(path.toString()).c_str())) {
            return GENERAL_FAILURE;
        }
#else
        if (chdir(path.toString().c_str()) == -1) {
            return GENERAL_FAILURE;
        }
#endif
        return OK;
    }
    else {
        return INVALID_PATH;
    }
}

Error filesystem::readTextFile(const Path& file_path, std::string& content)
{
#ifdef WIN32
    FILE* file = {};
    {
        const auto result =
            _wfopen_s(&file, strings::unicode::utf8ToUtf16(file_path.toString()).c_str(), L"rb");
        if (0 != result) {
            return OPEN_FAILED;
        }
    }
#else
    FILE* file = fopen(file_path.toString().c_str(), "rb");
    if (file == nullptr) {
        return OPEN_FAILED;
    }
#endif

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return GENERAL_FAILURE;
    }
    size_t size = ftell(file);
    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return GENERAL_FAILURE;
    }

    content.resize(size);

    size_t bytes_read = 0;
    while (bytes_read < size && feof(file) == 0) {
        size_t actually_read = fread(&content[bytes_read], 1, size - bytes_read, file);
        if (actually_read == 0 && ferror(file) != 0) {
            fclose(file);
            return IO_ERROR;
        }

        bytes_read += actually_read;
    }

    fclose(file);
    return OK;
}

Error filesystem::readTextLines(const Path& file_path, std::vector<std::string>& vec_lines)
{
    std::string content;
    Error ret = readTextFile(file_path, content);
    if (ret != OK) {
        return ret;
    }

    vec_lines.clear();

    std::string line;
    for (size_t i = 0; i < content.size(); ++i) {
        char c = content[i];
        switch (c) {
        case '\r':
            // might be followed by \n, treated as a single line separator
            if (i + 1 < content.size() && content[i + 1] == '\n') {
                i++;
            }
            // fall through
        case '\n':
            vec_lines.push_back(line);
            line.clear();
            break;
        default:
            line.push_back(c);
            break;
        }
    }

    if (!content.empty()) {
        vec_lines.push_back(line);
    }

    return OK;
}

Error filesystem::writeTextFile(const Path& file_path, const std::string& content)
{
#ifdef WIN32
    FILE* file = {};
    {
        const auto result =
            _wfopen_s(&file, strings::unicode::utf8ToUtf16(file_path.toString()).c_str(), L"wb");
        if (0 != result) {
            return OPEN_FAILED;
        }
    }
#else
    FILE* file = fopen(file_path.toString().c_str(), "wb");
    if (file == nullptr) {
        return OPEN_FAILED;
    }
#endif

    size_t bytes_written = 0;
    while (bytes_written < content.size()) {
        size_t actually_written =
            fwrite(&content[bytes_written], 1, content.size() - bytes_written, file);
        if (actually_written == 0 && ferror(file) != 0) {
            fclose(file);
            return IO_ERROR;
        }

        bytes_written += actually_written;
    }

    fclose(file);
    return OK;
}

Error filesystem::enumDirectory(const Path& dir_path,
                                std::vector<Path>& vec_entries,
                                int32_t flags /*= ED_FILES | ED_DIRECTORIES*/)
{
    vec_entries.clear();

    if (!isDirectory(dir_path)) {
        return INVALID_PATH;
    }

#ifdef WIN32
    WIN32_FIND_DATAW find_data;

    Path tmp_dir = dir_path + "*";

    HANDLE handle_find =
        ::FindFirstFileW(strings::unicode::utf8ToUtf16(tmp_dir.toString()).c_str(), &find_data);
    if (handle_find == INVALID_HANDLE_VALUE || handle_find == nullptr) {
        return GENERAL_FAILURE;
    }

    do {
        if ((flags & ED_DIRECTORIES) != 0 &&
            (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            if (0 != wcscmp(find_data.cFileName, L".") && 0 != wcscmp(find_data.cFileName, L"..")) {
                vec_entries.push_back(dir_path +
                                      strings::unicode::utf16ToUtf8(find_data.cFileName));
            }
        }
        else if ((flags & ED_FILES) != 0 &&
                 (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            vec_entries.push_back(dir_path + strings::unicode::utf16ToUtf8(find_data.cFileName));
        }
    } while (::FindNextFileW(handle_find, &find_data));

    ::FindClose(handle_find);
#else
    struct dirent** name_list;
    int count =
        scandir(const_cast<char*>(dir_path.toString().c_str()), &name_list, nullptr, &alphasort);

    if (count < 0) {
        return GENERAL_FAILURE;
    }
    else {
        for (int idx = 0; idx < count; ++idx) {
            char* char_ = name_list[idx]->d_name;

            if (fnmatch("*", char_, 0) == 0) {
                if ((flags & ED_DIRECTORIES) != 0) {
                    if (0 != strings::compare(char_, ".") && 0 != strings::compare(char_, "..")) {
                        struct stat64 buffer;
                        Path tmp_dir = dir_path + char_;
                        if (0 == stat64(tmp_dir.toString().c_str(), &buffer)) {
                            if (S_ISDIR(buffer.st_mode)) {
                                // is a directory
                                vec_entries.push_back(tmp_dir);
                            }
                        }
                    }
                }

                if ((flags & ED_FILES) != 0) {
                    struct stat64 buffer;
                    Path tmp_dir = dir_path + char_;
                    if (0 == stat64(tmp_dir.toString().c_str(), &buffer)) {
                        if (S_ISREG(buffer.st_mode)) {
                            // is a normal file
                            vec_entries.push_back(tmp_dir);
                        }
                    }
                }
            }
            free(name_list[idx]);
        }
        free(name_list);
    }
#endif

    return OK;
}

bool filesystem::exists(const Path& path)
{
    int32_t attribs = GetFSAttributes(path);
    if (attribs == filesystem::ATTRIBUTE_INVALID) {
        return false;
    }

    return true;
}

bool filesystem::createDirectory(const Path& path)
{
    if (isFile(path)) {
        return false;
    }

    if (isDirectory(path)) {
        return true;
    }

    Path parent = path.getParent();
    if (parent != path.getRoot() && !isDirectory(parent)) {
        createDirectory(parent);
    }

#ifdef WIN32
    bool status = ::CreateDirectoryW(strings::unicode::utf8ToUtf16(path.toString()).c_str(),
                                     nullptr) == TRUE ?
                      true :
                      false;
#else
    bool status =
        mkdir(path.toString().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 ? true : false;
#endif

    return status;
}

bool filesystem::isFile(const Path& file_path)
{
    int32_t attribs = GetFSAttributes(file_path);

    if (attribs == filesystem::ATTRIBUTE_INVALID || (attribs & filesystem::ATTRIBUTE_FOLDER)) {
        return false;
    }

    return true;
}

bool filesystem::isDirectory(const Path& dir_path)
{
    int32_t attribs = GetFSAttributes(dir_path);

    if (attribs == filesystem::ATTRIBUTE_INVALID || !(attribs & filesystem::ATTRIBUTE_FOLDER)) {
        return false;
    }

    return true;
}

bool filesystem::remove(const Path& path)
{
    if (isFile(path)) {
#ifdef WIN32
        BOOL status = ::DeleteFileW(strings::unicode::utf8ToUtf16(path.toString()).c_str());
        if (!status) {
            return false;
        }
#else
        int nStatus = unlink(path.toString().c_str());
        if (nStatus == -1) {
            return false;
        }
#endif
    }
    else if (isDirectory(path)) {
        // delete contents first
        std::vector<Path> vec_children;
        enumDirectory(path, vec_children, ED_DIRECTORIES | ED_FILES);
        for (std::vector<Path>::const_iterator it = vec_children.begin(); it != vec_children.end();
             ++it) {
            remove(*it);
        }

#ifdef WIN32
        BOOL status = ::RemoveDirectoryW(strings::unicode::utf8ToUtf16(path.toString()).c_str());
        if (!status) {
            return false;
        }
#else
        int result = rmdir(path.toString().c_str());
        if (result == -1) {
            return false;
        }
#endif
    }
    else {
        return false;
    }

    return true;
}

bool a_util::filesystem::rename(const Path& path, const Path& new_path)
{
    if (!exists(path)) {
        return false;
    }

    if (path == new_path) {
        return true;
    }

#ifdef WIN32
    DWORD move_flags =
        MOVEFILE_COPY_ALLOWED | // Allows Windows to use a Copy+Delete (can be faster)
        MOVEFILE_WRITE_THROUGH; // Ensures that all changes are flushed to disk before returning

    if (!isDirectory(path) && !isDirectory(new_path)) {
        move_flags |= MOVEFILE_REPLACE_EXISTING; // Replace existing file with new contents
    }

    BOOL res = ::MoveFileExW(strings::unicode::utf8ToUtf16(path.toString()).c_str(),
                             strings::unicode::utf8ToUtf16(new_path.toString()).c_str(),
                             move_flags);

    return res != 0;
#else
    int res = ::rename(path.toString().c_str(), new_path.toString().c_str());

    return res == 0;
#endif
}

std::int64_t a_util::filesystem::compareFiles(const Path& p1, const Path& p2)
{
    if (p1 == p2) {
        return 0;
    }

    if (isDirectory(p1) || isDirectory(p2)) {
        return -1;
    }

    std::ifstream first_file(p1.toString(), std::ios::in);
    std::ifstream second_file(p2.toString(), std::ios::in);

    if (!first_file.is_open() || !second_file.is_open()) {
        return -1;
    }

    char first_file_char = {};
    char second_file_char = {};
    std::int64_t position = {};
    while (!first_file.eof() && !second_file.eof()) {
        first_file.get(first_file_char);
        second_file.get(second_file_char);
        position++;
        if (first_file_char != second_file_char) {
            return position;
        }
    }

    const bool both_files_reach_end = first_file.eof() && second_file.eof();
    return both_files_reach_end ? 0 : position;
}

std::int64_t a_util::filesystem::compareFiles(const Path& p1, const Path& p2, DiffOptions options)
{
    std::int64_t position = 0;

    switch (options) {
    case DiffOptions::none:
        position = compareFiles(p1, p2);
        break;
    }

    return position;
}
