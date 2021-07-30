/**
 * @file
 * csv_reader library
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

#include "a_util/parser.h"

#include <fstream>

namespace a_util {
namespace parser {
void CSVReader::splitIntoSubstring(const std::string& data_string,
                                   const std::vector<char>& vector_split_points,
                                   std::vector<std::string>& vector_with_substrings)
{
    std::size_t start_pos = 0;
    std::size_t end_pos = 0;
    bool is_finished = false;

    // Iterate over the complete data string and split at the given splitpoint
    while (!is_finished) {
        // Iterate over the split point symbols and split at the point of first occurence
        std::size_t temporal_end = data_string.length();
        for (std::size_t i = 0; i < vector_split_points.size(); i++) {
            std::size_t temp_pos = data_string.find(vector_split_points.at(i), start_pos);
            if (temp_pos != std::string::npos && temp_pos < temporal_end) {
                temporal_end = temp_pos;
            }
        }

        end_pos = temporal_end;

        is_finished = (data_string.length() == end_pos);
        vector_with_substrings.push_back(data_string.substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + 1;
    }
}

void CSVReader::readFile(const std::string& filename, char delimiter)
{
    std::ifstream file(filename.c_str());
    std::vector<std::vector<std::string>> matrix;
    std::string line;
    // Iterate through each line and split the content using delimiter
    while (getline(file, line)) {
        std::vector<std::string> row;
        std::vector<char> vector_split_points;
        vector_split_points.push_back(delimiter);
        vector_split_points.push_back('\n');
        CSVReader::splitIntoSubstring(line, vector_split_points, row);

        matrix.push_back(row);
    }
    // Save the data internally
    this->_data_matrix = matrix;
}

std::vector<std::vector<std::string>> CSVReader::getData()
{
    return this->_data_matrix;
}

std::vector<std::string> CSVReader::getColumn(size_t column_number)
{
    std::vector<std::string> aux;
    for (size_t i = 0; i < this->_data_matrix.size(); i++) {
        aux.push_back(_data_matrix[i][column_number]);
    }

    return aux;
}

std::vector<std::string> CSVReader::getRow(size_t row_number)
{
    std::vector<std::string> aux;
    for (size_t i = 0; i < this->_data_matrix[0].size(); i++) {
        aux.push_back(_data_matrix[row_number][i]);
    }

    return aux;
}

std::string CSVReader::getElement(size_t column_number, size_t row_number)
{
    return _data_matrix[row_number][column_number];
}

} // namespace parser
} // namespace a_util
