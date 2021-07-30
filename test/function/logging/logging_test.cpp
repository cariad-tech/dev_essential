/**
 * @file
 * Logging test implementation
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

#include "a_util/logging/log.h"
#include "a_util/strings/strings_format.h"

#include <gtest/gtest.h>
using namespace a_util;

struct RAII_Reset {
    ~RAII_Reset()
    {
        logging::setLogger(logging::defaultLogger);
    }
};

struct TestLogger {
    size_t log_count;
    TestLogger() : log_count(0)
    {
    }

    void LogDump(const logging::LogEntry& entry)
    {
        ASSERT_EQ(entry.log_level, logging::Dump);
        ASSERT_EQ(entry.message, "testdump");
        ++log_count;
    }

    void LogInfo(const logging::LogEntry& entry)
    {
        ASSERT_EQ(entry.log_level, logging::Info);
        ASSERT_EQ(entry.message, "testinfo");
        ++log_count;
    }

    void LogWarning(const logging::LogEntry& entry)
    {
        ASSERT_EQ(entry.log_level, logging::Warning);
        ASSERT_EQ(entry.message, "testwarning");
        ++log_count;
    }

    void LogError(const logging::LogEntry& entry)
    {
        ASSERT_EQ(entry.log_level, logging::Error);
        ASSERT_EQ(entry.message, "testerror");
        ++log_count;
    }
};

TEST(logging_test, TestBasics)
{
    RAII_Reset reset;
    LOG_DUMP("testdump");
    LOG_INFO("testinfo");
    LOG_ERROR("testerror");
    LOG_WARNING("testwarning");

#ifdef _DEBUG
    {
        TestLogger tl;
        logging::setLogger(logging::Logger(&TestLogger::LogDump, tl));

        LOG_DUMP("testdump");
        ASSERT_EQ(tl.log_count, 1);
    }
#endif

    {
        TestLogger tl;
        logging::setLogger(logging::Logger(&TestLogger::LogInfo, tl));

        LOG_INFO("testinfo");
        ASSERT_EQ(tl.log_count, 1);
    }

    {
        TestLogger tl;
        logging::setLogger(logging::Logger(&TestLogger::LogWarning, tl));

        LOG_WARNING("testwarning");
        ASSERT_EQ(tl.log_count, 1);
    }

    {
        TestLogger tl;
        logging::setLogger(logging::Logger(&TestLogger::LogError, tl));

        LOG_ERROR("testerror");
        ASSERT_EQ(tl.log_count, 1);
    }
}

struct TestLogger2 : public TestLogger {
    void LogMacro(const logging::LogEntry& entry)
    {
        ASSERT_EQ(entry.message, "test 1");
        ++log_count;
    }
};

TEST(logging_test, TestMacros)
{
    RAII_Reset reset;

    TestLogger2 tl;
    logging::setLogger(logging::Logger(&TestLogger2::LogMacro, tl));

    LOG_ERROR("test %d", 1);
    LOG_WARNING("test %d", 1);
    LOG_INFO("test %d", 1);
    LOG_DUMP("test %d", 1);

    logging::setLogger(logging::defaultLogger);

#ifdef _DEBUG
    ASSERT_EQ(tl.log_count, 4);
#else
    ASSERT_EQ(tl.log_count, 3);
#endif
}

TEST(logging_test, TestEmpty)
{
    RAII_Reset reset;
    logging::setLogger(logging::Logger(nullptr));

    LOG_ERROR("");
    LOG_WARNING("");
    LOG_INFO("");
    LOG_DUMP("");
}
