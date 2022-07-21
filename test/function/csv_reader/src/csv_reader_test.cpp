/**
 * @file
 * csv_reader test implementation
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

#include <a_util/parser/csv_reader.h>

#include <gtest/gtest.h>

// Test getData from csv_reader
TEST(csv_reader_test, TestGetData)
{
    using a_util::parser::CSVReader;
    CSVReader reader;
    reader.readFile("files/test.csv", ';');

    std::vector<std::string> row;
    std::vector<std::vector<std::string>> matrix;
    row.push_back("11.418851944");
    row.push_back("48.7873250959");
    row.push_back("420.878");
    matrix.push_back(row);
    row.clear();
    row.push_back("11.4188440403");
    row.push_back("48.7873464768");
    row.push_back("420.878");
    matrix.push_back(row);
    row.clear();

    std::vector<std::vector<std::string>> compare_matrix;
    compare_matrix = reader.getData();

    EXPECT_EQ(compare_matrix, matrix);
}

// Test getColumn from csv_reader
TEST(csv_reader_test, TestGetColumn)
{
    using a_util::parser::CSVReader;
    CSVReader reader;
    reader.readFile("files/test.csv", ';');

    std::vector<std::string> column;
    column.push_back("11.418851944");
    column.push_back("11.4188440403");

    std::vector<std::string> compare_column;
    compare_column = reader.getColumn(0);

    EXPECT_EQ(compare_column, column);
}

// Test getRow from csv_reader
TEST(csv_reader_test, TestGetRow)
{
    using a_util::parser::CSVReader;
    CSVReader reader;
    reader.readFile("files/test.csv", ';');

    std::vector<std::string> row;
    row.push_back("11.418851944");
    row.push_back("48.7873250959");
    row.push_back("420.878");

    std::vector<std::string> compare_row;
    compare_row = reader.getRow(0);

    EXPECT_EQ(compare_row, row);
}

// Test getElement from csv_reader
TEST(csv_reader_test, TestGetElement)
{
    using a_util::parser::CSVReader;
    CSVReader reader;
    reader.readFile("files/test.csv", ';');

    std::string element;
    element = "11.418851944";

    std::string compare_element;
    compare_element = reader.getElement(0, 0);

    EXPECT_EQ(compare_element, element);
}
