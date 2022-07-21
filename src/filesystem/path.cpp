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

#include <a_util/filesystem.h>

#include <algorithm>
#include <iostream>

using namespace a_util;
using namespace a_util::filesystem;

static const char BACKWARD_SLASH = '\\';
static const char FORWARD_SLASH = '/';

// native slash definition
#ifdef WIN32
static const char NATIVE_SLASH = BACKWARD_SLASH;
static const char* NATIVE_DOUBLE_SLASHES = "\\\\";
static const Path::PathSeparator NATIVE_SEPARATOR = Path::PS_BackwardSlash;
#else
static const char NATIVE_SLASH = FORWARD_SLASH;
static const char* NATIVE_DOUBLE_SLASHES = "//";
static const Path::PathSeparator NATIVE_SEPARATOR = Path::PS_ForwardSlash;
#endif

// static helper functions
static bool SlashPredicate(char left, char right);
static void NormalizePath(std::string& path);

// PathException implementation
PathException::PathException(const std::string& message) : std::runtime_error(message.c_str())
{
}

// using this in member initialization list
#pragma warning(disable : 4355)

// Path implementation
class Path::Implementation {
public:
    Path* self;
    std::string path;

    Implementation(Path* self_) : self(self_)
    {
    }
};

Path::Path() : _impl(new Implementation(this))
{
}

Path::Path(const Path& path) : _impl(new Implementation(*path._impl))
{
    _impl->self = this;
}

Path& Path::operator=(const Path& path)
{
    if (this != &path) {
        *_impl = *path._impl;
        _impl->self = this;
    }
    return *this;
}

Path::Path(const std::string& path) : _impl(new Implementation(this))
{
    setPath(path);
}

Path::Path(const char* path) : _impl(new Implementation(this))
{
    if (path) {
        setPath(path);
    }
}

Path::~Path()
{
}

Path::operator std::string() const
{
    // path is in native format
    return _impl->path;
}

std::string Path::toString(PathSeparator separator) const
{
    if (separator == NATIVE_SEPARATOR || separator == Path::PS_Native) {
        return _impl->path;
    }

    std::string new_path = _impl->path;

    // we need to switch slashes
    if (separator == Path::PS_ForwardSlash) {
        std::replace(new_path.begin(), new_path.end(), BACKWARD_SLASH, FORWARD_SLASH);
    }
    else {
        std::replace(new_path.begin(), new_path.end(), FORWARD_SLASH, BACKWARD_SLASH);
    }

    return new_path;
}

static bool SlashPredicate(char left, char right)
{
    return left == NATIVE_SEPARATOR && left == right;
}

static void NormalizePath(std::string& path)
{
    // convert slashes to native
    constexpr auto native_slash_is_forward_slash = (NATIVE_SEPARATOR == Path::PS_ForwardSlash);
    if (native_slash_is_forward_slash) {
        std::replace(path.begin(), path.end(), BACKWARD_SLASH, FORWARD_SLASH);
    }
    else {
        std::replace(path.begin(), path.end(), FORWARD_SLASH, BACKWARD_SLASH);
    }

    // collapse multiple occurences of slashes into one occurence (except the first one, for network
    // paths)
    std::string::iterator it_begin = path.begin();
    if (path.size() >= 2 && path[0] == NATIVE_SLASH && path[1] == NATIVE_SLASH) {
        std::advance(it_begin, 2);
    }

    std::string::iterator it_end = std::unique(it_begin, path.end(), SlashPredicate);

    // it_end now points to the new end if the collapsed string content, erase everything after it
    path.erase(it_end, path.end());

    // remove any trailing slash (except the root)
    if (path.size() > 1 && path[path.size() - 1] == NATIVE_SLASH) {
        path.erase(path.size() - 1);
    }
}

bool Path::setPath(const std::string& path)
{
    _impl->path = path;
    NormalizePath(_impl->path);

    return true;
}

Path& Path::append(const Path& path)
{
    if (path.isEmpty()) {
        return *this;
    }

    if (path.isAbsolute()) {
        throw PathException("path is absolute");
    }

    if (!_impl->path.empty() && (_impl->path.size() != 1 || _impl->path[0] != NATIVE_SLASH)) {
        _impl->path.push_back(NATIVE_SLASH);
    }
    _impl->path.append(path._impl->path.begin(), path._impl->path.end());

    return *this;
}

Path Path::getRoot() const
{
    if (!_impl->path.empty()) {
        std::string::size_type pos = _impl->path.find(':');
        if (pos != std::string::npos) {
            return _impl->path.substr(0, pos + 1);
        }

        pos = _impl->path.find(NATIVE_DOUBLE_SLASHES);
        if (pos == 0) // only at the beginning
        {
            std::string::size_type pos_end = _impl->path.find(NATIVE_SLASH, 2);
            if (pos_end != std::string::npos) {
                return _impl->path.substr(0, pos_end);
            }
        }

        if (_impl->path[0] == NATIVE_SLASH) {
            return Path("/");
        }
    }
    return Path();
}

Path Path::getParent() const
{
    if (!_impl->path.empty()) {
        std::string::size_type pos_r = _impl->path.rfind(NATIVE_SLASH);
        if (pos_r != std::string::npos) {
            Path copy = *this;

            // special case for //ci-aev
            std::string::size_type pos_l = _impl->path.find(NATIVE_DOUBLE_SLASHES);
            if (pos_l != std::string::npos && pos_l + 1 == pos_r) {
                return copy;
            }

            // special case for /
            if (_impl->path[0] == NATIVE_SLASH && _impl->path.size() == 1) {
                return copy;
            }

            copy._impl->path.erase(pos_r);
            return copy;
        }
        else {
            // special case for C:
            std::string::size_type pos = _impl->path.rfind(':');
            if (pos != std::string::npos) {
                return *this;
            }
        }
    }
    return Path();
}

Path Path::getLastElement() const
{
    if (!_impl->path.empty()) {
        std::string::size_type pos_r = _impl->path.rfind(NATIVE_SLASH);
        if (pos_r != std::string::npos) {
            Path copy = *this;

            // special case for //ci-aev
            std::string::size_type pos_l = _impl->path.find(NATIVE_DOUBLE_SLASHES);
            if (pos_l != std::string::npos && pos_l + 1 == pos_r) {
                return Path();
            }

            // special case for /
            if (_impl->path[0] == NATIVE_SLASH && _impl->path.size() == 1) {
                return Path();
            }

            copy._impl->path.erase(0, pos_r + 1);
            return copy;
        }
        else {
            // special case for C:
            std::string::size_type pos = _impl->path.rfind(':');
            if (pos != std::string::npos) {
                return Path();
            }
            else {
                return *this;
            }
        }
    }
    return Path();
}

std::string Path::getExtension() const
{
    if (!_impl->path.empty()) {
        std::string::size_type pos_slash = _impl->path.rfind(NATIVE_SLASH);
        std::string::size_type pos_r = _impl->path.rfind('.');
        if (pos_r != std::string::npos && (pos_slash == std::string::npos || pos_slash < pos_r)) {
            return _impl->path.substr(pos_r + 1);
        }
    }
    return std::string();
}

Path& Path::appendToBasename(const std::string& str)
{
    if (!_impl->path.empty()) {
        std::string::size_type pos_slash = _impl->path.rfind(NATIVE_SLASH);
        std::string::size_type pos_r = _impl->path.rfind('.');
        if (pos_r != std::string::npos && (pos_slash == std::string::npos || pos_slash < pos_r)) {
            _impl->path.insert(pos_r, str);
        }
        else {
            // special case for /
            if (_impl->path[0] == NATIVE_SLASH && _impl->path.size() == 1) {
                return *this;
            }

            // special case for c:
            if (_impl->path[_impl->path.size() - 1] == ':') {
                return *this;
            }

            // special case for windows network drives (//foobar/)
            std::string::size_type pos_l = _impl->path.find(NATIVE_DOUBLE_SLASHES);
            if (pos_l != std::string::npos && pos_l + 1 == pos_slash) {
                return *this;
            }

            _impl->path.append(str);
        }
    }
    else {
        _impl->path.append(str);
    }
    return *this;
}

Path& Path::replaceExtension(const std::string& extension)
{
    if (!_impl->path.empty()) {
        std::string::size_type pos_slash = _impl->path.rfind(NATIVE_SLASH);
        std::string::size_type pos_r = _impl->path.rfind('.');
        if (pos_r != std::string::npos && (pos_slash == std::string::npos || pos_slash < pos_r)) {
            _impl->path.replace(pos_r + 1, std::string::npos, extension);
        }
        else {
            // special case for /
            if (_impl->path[0] == NATIVE_SLASH && _impl->path.size() == 1) {
                return *this;
            }

            // special case for c:
            if (_impl->path[_impl->path.size() - 1] == ':') {
                return *this;
            }

            // special case for //ci-aev
            std::string::size_type pos_l = _impl->path.find(NATIVE_DOUBLE_SLASHES);
            if (pos_l != std::string::npos && pos_l + 1 == pos_slash) {
                return *this;
            }

            _impl->path.push_back('.');
            _impl->path.append(extension);
        }
    }
    return *this;
}

Path& Path::removeLastElement()
{
    if (!_impl->path.empty()) {
        std::string::size_type pos_slash = _impl->path.rfind(NATIVE_SLASH);
        if (pos_slash != std::string::npos) {
            // special case for /
            if (_impl->path[0] == NATIVE_SLASH && _impl->path.size() == 1) {
                return *this;
            }

            // special case for //ci-aev
            std::string::size_type pos_l = _impl->path.find(NATIVE_DOUBLE_SLASHES);
            if (pos_l != std::string::npos && pos_l + 1 == pos_slash) {
                return *this;
            }

            _impl->path.erase(pos_slash);
        }
        else {
            // special case for c:
            if (_impl->path[_impl->path.size() - 1] == ':') {
                return *this;
            }

            _impl->path.clear();
        }
    }
    return *this;
}

Path& Path::makeCanonical()
{
    if (!_impl->path.empty()) {
        std::string::size_type idx = _impl->path.size() - 1;
        for (;;) {
            // are we at a '.' that
            // -is not at the beginning
            // -the next character is a slash or the end?
            if (_impl->path[idx] == '.' && idx > 0 &&
                (idx + 1 == _impl->path.size() || _impl->path[idx + 1] == NATIVE_SLASH)) {
                int parts_to_skip = 0;

                // is it a '..'?
                if (_impl->path[idx - 1] == '.') {
                    // stop if the '..' is at the beginning
                    if (idx - 1 == 0) {
                        idx = 0;
                        break;
                    }

                    parts_to_skip++;
                }

                if (idx > 0) {
                    // now we need to find the ending slash:
                    // XXX/XXX/.. or XXX/XXX/.
                    //    ^     ^           ^^
                    //    ^     idx         ^idx
                    //     end              end
                    std::string::size_type end = std::string::npos;
                    bool was_dotdot = false;
                    std::string::size_type cur = idx - 1;
                    for (;;) {
                        char c = _impl->path[cur];
                        if (c == NATIVE_SLASH) {
                            if (was_dotdot) {
                                was_dotdot = false;
                                goto next;
                            }

                            if (parts_to_skip > 0) {
                                parts_to_skip--;
                                goto next;
                            }

                            // special case for \\km-aev...
                            if (cur == 1 && _impl->path[0] == NATIVE_SLASH) {
                                cur = 0;
                            }

                            end = cur;
                            break;
                        }
                        else if (c == '.' && _impl->path[cur - 1] == '.') {
                            // we need to take care about more '..'
                            parts_to_skip++;
                            cur--;
                            was_dotdot = true;
                        }

                    next:
                        if (cur == 0) {
                            break;
                        }
                        cur--;
                    }

                    if (end == std::string::npos) {
                        end = 0;
                    }

                    _impl->path.erase(_impl->path.begin() + end, _impl->path.begin() + idx + 1);
                    idx = end;
                }
            }

            if (idx > 0) {
                idx -= 1;
            }
            else {
                break;
            }
        }
    }
    return *this;
}

Path& Path::makeAbsolute()
{
    if (!isAbsolute()) {
        Path p = a_util::filesystem::getWorkingDirectory();
        makeAbsolute(p);
    }
    return *this;
}

Path& Path::makeAbsolute(Path base_path)
{
    if (isEmpty()) {
        this->setPath(base_path);
    }
    if (!isAbsolute()) {
        base_path.append(*this);
        this->setPath(base_path);
    }
    this->makeCanonical();
    return *this;
}

Path& Path::makeRelative()
{
    // By default, make relative to the current working directory.
    return makeRelative(a_util::filesystem::getWorkingDirectory());
}

Path& Path::makeRelative(Path ref_path)
{
    // to compute relative path I work with absolute path
    this->makeAbsolute();
    this->makeCanonical();

    ref_path.makeAbsolute();
    ref_path.makeCanonical();

    if (ref_path.getRoot() != getRoot()) {
        throw PathException("Paths have different roots");
    }

    Path result_path;
    while (this->toString(Path::PS_Native).find(ref_path.toString(Path::PS_Native)) != 0) {
        ref_path = ref_path.getParent();
        result_path.append("..");
        if (ref_path.isEmpty()) {
            break;
        }
    }

    // Remove common path in front of our path
    std::string tail = _impl->path.substr(ref_path.toString(Path::PS_Native).length());

    // Remove a leading separator
    if (!tail.empty() && tail[0] == NATIVE_SLASH) {
        tail.erase(0, 1);
    }

    // Join both pathes, and respect if one of them is empty
    _impl->path = result_path.append(tail);
    return *this;
}

bool Path::isRelative() const
{
    if (isEmpty()) {
        throw PathException("Empty path is neither relative nor absolute");
    }
    return getRoot().isEmpty();
}

bool Path::isAbsolute() const
{
    if (isEmpty()) {
        throw PathException("Empty path is neither relative nor absolute");
    }
    return !getRoot().isEmpty();
}

bool Path::isEmpty() const
{
    return _impl->path.empty();
}

Path& Path::Clear()
{
    _impl->path.clear();
    return *this;
}

bool a_util::filesystem::operator==(const Path& a, const Path& b)
{
    return a._impl->path == b._impl->path;
}

bool a_util::filesystem::operator!=(const Path& a, const Path& b)
{
    return !(a == b);
}

std::ostream& a_util::filesystem::operator<<(std::ostream& os, const Path& p)
{
    os << p.toString();
    return os;
}

a_util::filesystem::Path a_util::filesystem::operator+(const Path& a, const Path& b)
{
    Path c = a;
    try {
        c.append(b);
    }
    catch (...) {
        c.Clear();
    }
    return c;
}
