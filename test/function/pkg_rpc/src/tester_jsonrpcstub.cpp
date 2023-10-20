/**
 * @file
 * RPC jsonstubgenerator functional tester implementation file.
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/filesystem/filesystem.h>

#include <gtest/gtest.h>

#include <cstdlib>

#ifndef STUBGENERATOR_EXE
#error 'STUBGENERATOR_EXE' must be defined for these tests
#endif // !STUBGENERATOR_EXE

#ifndef SPECIFICATION_FILE
#error 'SPECIFICATION_FILE' must be defined for these tests
#endif // !SPECIFICATION_FILE

#ifndef TEST_FILES_WRITE_DIR
#error 'TEST_FILES_WRITE_DIR' must be defined for these tests
#endif // !TEST_FILES_WRITE_DIR

namespace {

// for readibility in actual test case calls
struct Command {
    static const std::string jsonrpcstub;
    static const std::string jsonrpcstub_with_specfile;
    static const std::string cd_test_files_write_dir_and;
    static const std::string no_option;

    static auto option(const char* opt) -> std::string
    {
        return {opt};
    }

    static auto options(const char* opt1, const char* opt2) -> std::string
    {
        return option(opt1) + ' ' + opt2;
    }

    static auto options(const char* opt1, const char* opt2, const char* opt3) -> std::string
    {
        return options(opt1, opt2) + ' ' + opt3;
    }
};

// for readibility in actual test case calls
struct Expect {
    static const a_util::filesystem::Path no_file;
    static const std::string no_stdout;
    static const std::string verbose_stdout;

    static auto file(const char* filename) -> a_util::filesystem::Path
    {
        return a_util::filesystem::Path(TEST_FILES_WRITE_DIR).append(filename);
    }
    static auto output(const char* expected_stdout) -> std::string
    {
        return {expected_stdout};
    }
    static auto verbose(const char* expected_stdout) -> std::string
    {
        return verbose_stdout + expected_stdout + '\n';
    }
};

const std::string Command::jsonrpcstub = STUBGENERATOR_EXE;
const std::string Command::jsonrpcstub_with_specfile = STUBGENERATOR_EXE " " SPECIFICATION_FILE;
const std::string Command::cd_test_files_write_dir_and = "cd " TEST_FILES_WRITE_DIR " && ";
const std::string Command::no_option = {};

const a_util::filesystem::Path Expect::no_file = {};
const std::string Expect::no_stdout = {};
const std::string Expect::verbose_stdout = "Found 7 procedures in " SPECIFICATION_FILE "\n\t"
                                           "[Method]         GetInteger\n\t"
                                           "[Method]         Concat\n\t"
                                           "[Method]         GetIntegerAsString\n\t"
                                           "[Method]         GetResult\n\t"
                                           "[Method]         RegisterObject\n\t"
                                           "[Method]         UnregisterObject\n\t"
                                           "[Method]         UnregisterSelf\n\n";

// captures stdout/stderr for all test cases on construction
class StubGeneratorTestFixtureBase {
protected:
    StubGeneratorTestFixtureBase()
    {
        // all test cases check stdout and stderr of jsonstubgenerator
        ::testing::internal::CaptureStdout();
        ::testing::internal::CaptureStderr();
    }
};

// executes command and captures stdout/stderr on construction
class StubGeneratorTestRunner : public StubGeneratorTestFixtureBase, public ::testing::Test {
protected:
    StubGeneratorTestRunner() : StubGeneratorTestRunner(Command::jsonrpcstub)
    {
    }

    StubGeneratorTestRunner(const std::string& command)
        : StubGeneratorTestFixtureBase(),
          ::testing::Test(),
          _return_value(std::system(command.c_str())), // executes command and saves retval
          _captured_stdout(::testing::internal::GetCapturedStdout()), // saves stdout of command
          _captured_stderr(::testing::internal::GetCapturedStderr())  // saves stderr of command
    {
    }

protected:
    const int _return_value;
    const std::string _captured_stdout;
    const std::string _captured_stderr;
};

class StubGeneratorParametrizedTestRunner
    : public StubGeneratorTestRunner,
      public ::testing::WithParamInterface<std::tuple<std::string, // command to execute
                                                      std::string, // command line parameter to add
                                                      std::string, // expected stdout
                                                      a_util::filesystem::Path>> // expected file
{
protected: // ctor
    StubGeneratorParametrizedTestRunner()
        // executes command on construction
        : StubGeneratorTestRunner(std::get<0>(GetParam()) + ' ' + std::get<1>(GetParam())),
          _expected_stdout(std::get<2>(GetParam())),
          _expected_file(std::get<3>(GetParam()))
    {
    }

protected: // data members
    const std::string& _expected_stdout;
    const a_util::filesystem::Path& _expected_file;

private: // functions
    void TearDown() override
    {
        if (!_expected_file.isEmpty() && a_util::filesystem::exists(_expected_file)) {
            ASSERT_TRUE(a_util::filesystem::isFile(_expected_file))
                << _expected_file << ": Not a file, afraid to delete. Aborting test ...";
            ASSERT_TRUE(a_util::filesystem::remove(_expected_file));
        }
    }
};

/**
 * Test execution without specification file prints an error message and exits
 */
TEST_F(StubGeneratorTestRunner, printsErrorWithoutSpecFile)
{
    ASSERT_NE(0, _return_value);
    EXPECT_EQ(_captured_stderr, "Invalid arguments: specfile must be provided.\n");
    EXPECT_TRUE(_captured_stdout.empty()) << _captured_stdout;
}

TEST_P(StubGeneratorParametrizedTestRunner, generatesExpectedOutput)
{
    EXPECT_EQ(0, _return_value);
    EXPECT_EQ(_expected_stdout, _captured_stdout);
    EXPECT_TRUE(_captured_stderr.empty()) << _captured_stderr;

    if (!_expected_file.isEmpty()) {
        // check file was written if expected
        EXPECT_TRUE(a_util::filesystem::isFile(_expected_file))
            << _expected_file << ": Not a file.";
    }
    else {
        // check folder test files write dir is empty and NO file was written
        auto test_files_write_dir = std::vector<a_util::filesystem::Path>{};
        EXPECT_EQ(a_util::filesystem::Error::OK,
                  a_util::filesystem::enumDirectory(TEST_FILES_WRITE_DIR, test_files_write_dir));
        EXPECT_TRUE(test_files_write_dir.empty()) << TEST_FILES_WRITE_DIR << ": Not empty!";
    }
}

using std::make_tuple;

INSTANTIATE_TEST_SUITE_P(
    StubGeneratorTests,
    StubGeneratorParametrizedTestRunner,
    ::testing::Values(
        make_tuple(Command::jsonrpcstub,
                   Command::option("--version"),
                   Expect::output("jsonrpcstub version 1.0.0\n"),
                   Expect::no_file),
        make_tuple(Command::jsonrpcstub,
                   Command::option("--help"),
                   Expect::output("Usage: " STUBGENERATOR_EXE " "),
                   Expect::no_file),
        make_tuple(Command::jsonrpcstub,
                   Command::option("-h"),
                   Expect::output("Usage: " STUBGENERATOR_EXE " "),
                   Expect::no_file),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::no_option,
                   Expect::no_stdout,
                   Expect::no_file),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::option("--verbose"),
                   Expect::verbose_stdout,
                   Expect::no_file),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::option("-v"),
                   Expect::verbose_stdout,
                   Expect::no_file),
        make_tuple(Command::cd_test_files_write_dir_and + Command::jsonrpcstub_with_specfile,
                   Command::option("--cpp-server=TestServerStub1"),
                   Expect::no_stdout,
                   Expect::file("testserverstub1.h")),
        make_tuple(Command::cd_test_files_write_dir_and + Command::jsonrpcstub_with_specfile,
                   Command::options("--cpp-server=TestServerStub2", "--verbose"),
                   Expect::verbose("Generating C++ Serverstub to: testserverstub2.h"),
                   Expect::file("testserverstub2.h")),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::options("--cpp-server=TestServerStub",
                                    "--cpp-server-file=" TEST_FILES_WRITE_DIR "testserverstub3.h"),
                   Expect::no_stdout,
                   Expect::file("testserverstub3.h")),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::options("--cpp-server=TestServerStub",
                                    "--cpp-server-file=" TEST_FILES_WRITE_DIR "testserverstub4.h",
                                    "--verbose"),
                   Expect::verbose("Generating C++ Serverstub to: " TEST_FILES_WRITE_DIR
                                   "testserverstub4.h"),
                   Expect::file("testserverstub4.h")),
        make_tuple(Command::cd_test_files_write_dir_and + Command::jsonrpcstub_with_specfile,
                   Command::option(" --cpp-client=TestClientStub1"),
                   Expect::no_stdout,
                   Expect::file("testclientstub1.h")),
        make_tuple(Command::cd_test_files_write_dir_and + Command::jsonrpcstub_with_specfile,
                   Command::options("--cpp-client=TestClientStub2", "--verbose"),
                   Expect::verbose("Generating C++ Clientstub to: testclientstub2.h"),
                   Expect::file("testclientstub2.h")),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::options("--cpp-client=TestClientStub",
                                    "--cpp-client-file=" TEST_FILES_WRITE_DIR "testclientstub3.h"),
                   Expect::no_stdout,
                   Expect::file("testclientstub3.h")),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::options("--cpp-client=TestClientStub",
                                    "--cpp-client-file=" TEST_FILES_WRITE_DIR "testclientstub4.h",
                                    "--verbose"),
                   Expect::verbose("Generating C++ Clientstub to: " TEST_FILES_WRITE_DIR
                                   "testclientstub4.h"),
                   Expect::file("testclientstub4.h")),
        make_tuple(Command::cd_test_files_write_dir_and + Command::jsonrpcstub_with_specfile,
                   Command::option(" --js-client=TestClientStub1"),
                   Expect::no_stdout,
                   Expect::file("testclientstub1.h")),
        make_tuple(Command::cd_test_files_write_dir_and + Command::jsonrpcstub_with_specfile,
                   Command::options("--js-client=TestClientStub2", "--verbose"),
                   Expect::verbose("Generating JavaScript Clientstub to: testclientstub2.h"),
                   Expect::file("testclientstub2.h")),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::options("--js-client=TestClientStub",
                                    "--js-client-file=" TEST_FILES_WRITE_DIR "testclientstub3.h"),
                   Expect::no_stdout,
                   Expect::file("testclientstub3.h")),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::options("--js-client=TestClientStub",
                                    "--js-client-file=" TEST_FILES_WRITE_DIR "testclientstub4.h",
                                    "--verbose"),
                   Expect::verbose("Generating JavaScript Clientstub to: " TEST_FILES_WRITE_DIR
                                   "testclientstub4.h"),
                   Expect::file("testclientstub4.h")),
        // Without either --cpp-server, --cpp-client or --js-client no output is generated
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::option("--cpp-server-file=" TEST_FILES_WRITE_DIR "TestStub"),
                   Expect::no_stdout,
                   Expect::no_file),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::option("--cpp-client-file=" TEST_FILES_WRITE_DIR "TestStub"),
                   Expect::no_stdout,
                   Expect::no_file),
        make_tuple(Command::jsonrpcstub_with_specfile,
                   Command::option("--js-client-file=" TEST_FILES_WRITE_DIR "TestStub"),
                   Expect::no_stdout,
                   Expect::no_file)));

} // namespace