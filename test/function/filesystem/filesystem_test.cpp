
/**
 * @file
 * Filesystem test implementation
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

#include <a_util/filesystem/filesystem.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <iostream>
using namespace a_util;
using namespace a_util::filesystem;

// FileSystem Tests ============================
TEST(filesystem_test, TestWorkingDirTemp)
{
    // getWorkingDirectory, setWorkingDirectory, getTempDirectory
    Path current = filesystem::getWorkingDirectory();
    EXPECT_FALSE(current.isEmpty());
    EXPECT_TRUE(filesystem::isDirectory(current));

    Path new_path = filesystem::getTempDirectory();
    EXPECT_TRUE(filesystem::isDirectory(new_path));
    new_path.append("TestWorkingDirTemp");
    filesystem::remove(new_path);
    EXPECT_FALSE(filesystem::isDirectory(new_path));

    ASSERT_TRUE(filesystem::createDirectory(new_path));
    EXPECT_TRUE(filesystem::isDirectory(new_path));

    EXPECT_EQ(filesystem::setWorkingDirectory(new_path), filesystem::OK);
#ifndef __QNX__
    EXPECT_EQ(filesystem::getWorkingDirectory(), new_path);
#else
    // The original test does not work when the /tmp directory is a link
    // and therefore changing into it returns a different path than /tmp.
    // We now assume that this path at least contains /tmp.
    // This is not really a QNX issue, but can happen on all Unix systems.
    EXPECT_NE(filesystem::getWorkingDirectory().toString().find(new_path.toString()),
              std::string::npos);
#endif
    EXPECT_EQ(filesystem::setWorkingDirectory(current), filesystem::OK);
#ifndef __QNX__
    EXPECT_EQ(filesystem::getWorkingDirectory(), current);
#else
    // Same as above.
    EXPECT_NE(filesystem::getWorkingDirectory().toString().find(current.toString()),
              std::string::npos);
#endif

    EXPECT_EQ(filesystem::setWorkingDirectory(""), filesystem::INVALID_PATH);

    filesystem::remove(new_path);
}

TEST(filesystem_test, TestReadWriteTextFile)
{
    // readTextFile, writeTextFile
    Path path = filesystem::getTempDirectory() + "test_file.txt";
    filesystem::remove(path);
    EXPECT_FALSE(filesystem::isFile(path));

    EXPECT_EQ(filesystem::writeTextFile(path, "Blubb\r\nBla"), filesystem::OK);
    EXPECT_TRUE(filesystem::isFile(path));

    std::string str;
    EXPECT_EQ(filesystem::readTextFile(path, str), filesystem::OK);
    EXPECT_EQ(str, "Blubb\r\nBla");

    EXPECT_EQ(filesystem::writeTextFile(path, ""), filesystem::OK);
    EXPECT_EQ(filesystem::readTextFile(path, str), filesystem::OK);
    EXPECT_EQ(str, "");

    filesystem::remove(path);

    EXPECT_EQ(filesystem::writeTextFile("", ""), filesystem::OPEN_FAILED);
    EXPECT_EQ(filesystem::readTextFile("", str), filesystem::OPEN_FAILED);
}

TEST(filesystem_test, TestReadTextLines)
{
    // readTextLines
    Path path = filesystem::getTempDirectory() + "test_file.txt";
    std::string line = "\nline1\nline2\r\nline3\n\nline4";
    EXPECT_EQ(filesystem::writeTextFile(path, line), filesystem::OK);

    std::vector<std::string> lines;
    EXPECT_EQ(filesystem::readTextLines(path, lines), filesystem::OK);
    ASSERT_EQ(lines.size(), 6);
    EXPECT_EQ(lines[0], "");
    EXPECT_EQ(lines[1], "line1");
    EXPECT_EQ(lines[2], "line2");
    EXPECT_EQ(lines[3], "line3");
    EXPECT_EQ(lines[4], "");
    EXPECT_EQ(lines[5], "line4");

    EXPECT_EQ(filesystem::writeTextFile(path, ""), filesystem::OK);
    EXPECT_EQ(filesystem::readTextLines(path, lines), filesystem::OK);
    ASSERT_EQ(lines.size(), 0);

    EXPECT_TRUE(filesystem::remove(path));
    EXPECT_EQ(filesystem::readTextLines(path, lines), filesystem::OPEN_FAILED);
}

TEST(filesystem_test, TestEnumDirectory)
{
    // enumDirectory
    Path base = filesystem::getTempDirectory() + "TestBase";
    filesystem::remove(base);
    EXPECT_FALSE(filesystem::isDirectory(base));

    std::vector<Path> entries;
    EXPECT_EQ(filesystem::enumDirectory(base, entries), filesystem::INVALID_PATH);
    EXPECT_TRUE(entries.empty());

    EXPECT_TRUE(filesystem::createDirectory(base));
    EXPECT_EQ(filesystem::enumDirectory(base, entries), filesystem::OK);
    EXPECT_TRUE(entries.empty());

    EXPECT_TRUE(filesystem::createDirectory(base + "SubDir"));
    EXPECT_EQ(filesystem::enumDirectory(base, entries), filesystem::OK);
    ASSERT_EQ(entries.size(), 1);
    EXPECT_EQ(entries[0], base + "SubDir");

    EXPECT_EQ(filesystem::writeTextFile(base + "file.txt", ""), filesystem::OK);
    EXPECT_EQ(filesystem::enumDirectory(base, entries), filesystem::OK);
    ASSERT_EQ(entries.size(), 2);

    // use sorted string vector to compare, since order of filesystem entries is OS dependant
    std::vector<std::string> strings;
    strings.push_back(entries[0].toString());
    strings.push_back(entries[1].toString());
    std::sort(strings.begin(), strings.end());

    EXPECT_EQ(strings[0], (base + "SubDir").toString());
    EXPECT_EQ(strings[1], (base + "file.txt").toString());

    EXPECT_TRUE(filesystem::remove(base));
}

TEST(filesystem_test, TestExistsCreate)
{
    // exists, create
    Path base = filesystem::getTempDirectory() + "TestBase";
    filesystem::remove(base);
    ASSERT_FALSE(filesystem::isDirectory(base));

    Path dir = base + "Dir";
    Path file = dir + "file.txt";

    EXPECT_FALSE(filesystem::exists(base));
    EXPECT_TRUE(filesystem::createDirectory(dir));
    EXPECT_TRUE(filesystem::exists(base));
    EXPECT_TRUE(filesystem::exists(dir));

    filesystem::remove(dir);
    EXPECT_FALSE(filesystem::exists(dir));

    EXPECT_TRUE(filesystem::createDirectory(file));
    EXPECT_TRUE(filesystem::exists(dir));
    EXPECT_FALSE(filesystem::isFile(file));

    filesystem::remove(base);
}

TEST(filesystem_test, TestIsFileDirectory)
{
    // isFile, isDirectory
    Path base = filesystem::getTempDirectory();
    base.append("TestBase");
    filesystem::remove(base);
    EXPECT_FALSE(filesystem::isDirectory(base));

    ASSERT_FALSE(filesystem::isFile(base));
    ASSERT_FALSE(filesystem::isDirectory(base));

    EXPECT_TRUE(filesystem::createDirectory(base));

    ASSERT_FALSE(filesystem::isFile(base));
    ASSERT_TRUE(filesystem::isDirectory(base));

    Path file = base + "file.txt";
    EXPECT_EQ(filesystem::writeTextFile(file, ""), filesystem::OK);

    ASSERT_TRUE(filesystem::isFile(file));
    ASSERT_FALSE(filesystem::isDirectory(file));

    filesystem::remove(base);
}

TEST(filesystem_test, TestDelete)
{
    // remove
    Path base = filesystem::getTempDirectory();
    base.append("TestBase");
    filesystem::remove(base);
    EXPECT_FALSE(filesystem::isDirectory(base));

    EXPECT_FALSE(filesystem::remove(base));
    EXPECT_TRUE(filesystem::createDirectory(base));
    EXPECT_TRUE(filesystem::isDirectory(base));

    EXPECT_TRUE(filesystem::remove(base));
    EXPECT_FALSE(filesystem::isDirectory(base));

    EXPECT_TRUE(filesystem::createDirectory(base));
    EXPECT_EQ(filesystem::writeTextFile(base + "file.txt", ""), filesystem::OK);
    EXPECT_TRUE(filesystem::createDirectory(base + "SubDir"));
    EXPECT_EQ(filesystem::writeTextFile(base + "SubDir/file.txt", ""), filesystem::OK);
    EXPECT_TRUE(filesystem::isFile(base + "SubDir/file.txt"));

    EXPECT_TRUE(filesystem::remove(base));
    EXPECT_FALSE(filesystem::isDirectory(base));
}

TEST(filesystem_test, TestRename)
{
    // rename
    Path base = filesystem::getTempDirectory();
    base.append("TestBase");
    filesystem::remove(base);
    EXPECT_FALSE(filesystem::isDirectory(base));
    EXPECT_TRUE(filesystem::createDirectory(base));
    EXPECT_TRUE(filesystem::isDirectory(base));
    EXPECT_EQ(filesystem::writeTextFile(base + "file.txt", "a"), filesystem::OK);
    EXPECT_TRUE(filesystem::createDirectory(base + "SubDir"));
    EXPECT_EQ(filesystem::writeTextFile(base + "SubDir/file.txt", "b"), filesystem::OK);

    Path new_base = base.toString() + "2";
    EXPECT_FALSE(filesystem::isDirectory(new_base));

    EXPECT_TRUE(filesystem::rename(base, new_base));
    EXPECT_FALSE(filesystem::isDirectory(base));
    EXPECT_TRUE(filesystem::isDirectory(new_base));
    EXPECT_TRUE(filesystem::isFile(new_base + "file.txt"));
    EXPECT_TRUE(filesystem::isDirectory(new_base + "SubDir"));
    EXPECT_TRUE(filesystem::isFile(new_base + "SubDir/file.txt"));

    std::string content;
    EXPECT_EQ(filesystem::readTextFile(new_base + "SubDir/file.txt", content), filesystem::OK);
    EXPECT_EQ(content, "b");

    EXPECT_TRUE(filesystem::rename(new_base + "SubDir/file.txt", new_base + "file.txt"));
    EXPECT_EQ(filesystem::readTextFile(new_base + "file.txt", content), filesystem::OK);
    EXPECT_EQ(content, "b");

    EXPECT_TRUE(filesystem::remove(new_base));
}

/*
 * @req_id todo
 */
TEST(filesystem_test, compareFiles)
{
    // implement test and add further testcases to keep size of one test case to a minimum
    const Path base = filesystem::getTempDirectory().append("TestBase");
    filesystem::remove(base);
    ASSERT_FALSE(filesystem::isDirectory(base));
    ASSERT_TRUE(filesystem::createDirectory(base));
    ASSERT_TRUE(filesystem::isDirectory(base));

    // test compare binary file with option none
    std::ofstream output_file1;
    std::ofstream output_file2;
    output_file1.open(base + "file1.dat", std::ios::binary | std::ios::out);
    output_file2.open(base + "file2.dat", std::ios::binary | std::ios::out);
    output_file1.write("\x01\x02\x4d\x5a", 4);
    output_file2.write("\x02\x01\x4d\x5a", 4);
    output_file1.close();
    output_file2.close();
    ASSERT_TRUE(filesystem::exists(base + "file1.dat"));
    ASSERT_TRUE(filesystem::exists(base + "file2.dat"));
    EXPECT_EQ(filesystem::compareFiles(base + "file1.dat", base + "file2.dat", DiffOptions::none),
              1);

    // test compare binary file with same contents
    output_file1.open(base + "file1.dat", std::ios::binary | std::ios::out);
    output_file2.open(base + "file2.dat", std::ios::binary | std::ios::out);
    output_file1.write("\x01\x02\x4d\x5a", 4);
    output_file2.write("\x01\x02\x4d\x5a", 4);
    output_file1.close();
    output_file2.close();
    ASSERT_TRUE(filesystem::exists(base + "file1.dat"));
    ASSERT_TRUE(filesystem::exists(base + "file2.dat"));
    EXPECT_EQ(filesystem::compareFiles(base + "file1.dat", base + "file2.dat", DiffOptions::none),
              0);

    // test compare text file with option none
    EXPECT_EQ(filesystem::writeTextFile(base + "file1.txt", "aa aa\nbb"), filesystem::OK);
    EXPECT_EQ(filesystem::writeTextFile(base + "file2.txt", "aa  aa\nbb"), filesystem::OK);
    EXPECT_EQ(filesystem::compareFiles(base + "file1.txt", base + "file2.txt", DiffOptions::none),
              4);

    // test files content are equal and also files are the same
    EXPECT_EQ(filesystem::writeTextFile(base + "file1.txt", "aaaa\nbb"), filesystem::OK);
    EXPECT_EQ(filesystem::writeTextFile(base + "file2.txt", "aaaa\nbb"), filesystem::OK);
    EXPECT_EQ(filesystem::compareFiles(base + "file1.txt", base + "file2.txt"), 0);
    EXPECT_EQ(filesystem::compareFiles(base + "file1.txt", base + "file1.txt"), 0);

    // test files are different
    EXPECT_EQ(filesystem::writeTextFile(base + "file1.txt", "aaaa\nbb"), filesystem::OK);
    EXPECT_EQ(filesystem::writeTextFile(base + "file2.txt", "aaaa\ncb"), filesystem::OK);
    EXPECT_EQ(filesystem::compareFiles(base + "file1.txt", base + "file2.txt"), 6);

    // test files are different size
    EXPECT_EQ(filesystem::writeTextFile(base + "file1.txt", "aaaa\nbb"), filesystem::OK);
    EXPECT_EQ(filesystem::writeTextFile(base + "file2.txt", "aaaa\nbbb"), filesystem::OK);
    EXPECT_EQ(filesystem::compareFiles(base + "file1.txt", base + "file2.txt"), 8);

    // test one file does not exist and path is a directory.
    EXPECT_TRUE(filesystem::remove(base + "file1.txt"));
    EXPECT_EQ(filesystem::compareFiles(base + "file1.txt", base + "file2.txt"), -1);
    EXPECT_EQ(filesystem::compareFiles(base, base + "file2.txt"), -1);

    filesystem::remove(base);
}
