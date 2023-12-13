/**
 * @file
 * Public API for @ref a_util::parser::CSVReader "CSVReader" type
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTILS_UTIL_CSV_READER_HEADER_INCLUDED_
#define A_UTILS_UTIL_CSV_READER_HEADER_INCLUDED_

#include <string>
#include <vector>

namespace a_util {
namespace parser {
/**
 * Read, store and retrieve data from a comma separated value file (csv)
 */
class CSVReader {
private:
    std::vector<std::vector<std::string>> _data_matrix;

public:
    /**
     * Read data from a csv file and internally store it in a vector of string vectors
     * @param[in] filename Full path to the file
     * @param[in] delimiter The delimiter separating the data cells (';' or ',' and '\n')
     */
    void readFile(const std::string& filename, char delimiter);

    /**
     * Get data from csv file as a vector of string vectors
     * @return The data, organized in a 2 dimensional vector (first index is the row,
     *         second index the column)
     */
    std::vector<std::vector<std::string>> getData();

    /**
     * Get the contents of the column identified by @c column_number
     * @param[in] column_number Number of the column, first column being 0
     * @return Data of the entire column.
     */
    std::vector<std::string> getColumn(size_t column_number);

    /**
     * Get the contents of the row identified by @c row_number
     * @param[in] row_number Number of the row, first row being 0
     * @return Data of the entire row
     */
    std::vector<std::string> getRow(size_t row_number);

    /**
     * Get the content of a cell, identified by @c column_number and @c row_number
     * @param[in] column_number Number of the column, first column being 0
     * @param[in] row_number Number of the row, first row being 0
     * @return Content of the specified cell
     */
    std::string getElement(size_t column_number, size_t row_number);

private:
    /**
     * Split data from one line into vector elements according to the separator
     * @param[in] data_string Reference to the string to be separated
     * @param[in] vector_split_point Reference to a vector of strings with the separators of data,
     *                               (';' or ',') and line separator '\n'
     * @param[in] vector_with_substrings Reference to a vector of strings with the data separated
     *                                   as vector elements
     */
    void splitIntoSubstring(const std::string& data_string,
                            const std::vector<char>& vector_split_point,
                            std::vector<std::string>& vector_with_substrings);
};

} // namespace parser
} // namespace a_util

#endif // A_UTILS_UTIL_CSV_READER_HEADER_INCLUDED_
