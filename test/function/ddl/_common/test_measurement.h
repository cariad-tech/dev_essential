/**
 * @file
 * Test DataDefinition Helper for time measurement
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_MEASUREMENT_H_INCLUDED
#define TEST_MEASUREMENT_H_INCLUDED

#include <chrono>
#include <sstream>
#include <vector>

struct Measuremment {
    void reset()
    {
        _measured_durations.clear();
    }
    void start()
    {
        _current_begin_of_measurement = std::chrono::high_resolution_clock::now();
        if (_measured_durations.empty()) {
            _first_time = _current_begin_of_measurement;
        }
    }
    void stop()
    {
        _last_time = std::chrono::high_resolution_clock::now();
        const auto current_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            _last_time - _current_begin_of_measurement);
        _measured_durations.push_back(current_duration);
        if (current_duration.count() == 0) {
            int test = 0;
            (void)test;
        }
    }
    struct MeasurementResult {
        std::chrono::microseconds duration = {};
        int64_t average_duration = {};
        std::chrono::nanoseconds min_duration = {};
        size_t zero_count = 0;
        size_t increasing_time_count = 0;
        size_t decreasing_time_count = 0;
        size_t count = 0;
    };
    MeasurementResult getResult() const
    {
        MeasurementResult result;
        result.count = _measured_durations.size();
        result.duration =
            std::chrono::duration_cast<std::chrono::microseconds>(_last_time - _first_time);
        bool min_duration_set = false;
        bool average_duration_set = false;
        bool last_duration_set = false;
        std::chrono::nanoseconds last_duration;
        for (auto current_duration: _measured_durations) {
            if (last_duration_set) {
                if (last_duration < current_duration) {
                    ++result.decreasing_time_count;
                }
                else {
                    ++result.increasing_time_count;
                }
                last_duration = current_duration;
            }
            else {
                last_duration = current_duration;
                last_duration_set = true;
            }
            if (current_duration.count() != 0) {
                if (min_duration_set) {
                    if (current_duration < result.min_duration) {
                        result.min_duration = current_duration;
                    }
                }
                else {
                    result.min_duration = current_duration;
                    min_duration_set = true;
                }
                if (average_duration_set) {
                    result.average_duration = result.average_duration + current_duration.count();
                }
                else {
                    result.average_duration = current_duration.count();
                    average_duration_set = true;
                }
            }
            else {
                ++result.zero_count;
            }
        }
        if (result.zero_count < _measured_durations.size()) {
            result.average_duration =
                result.average_duration / (_measured_durations.size() - result.zero_count);
        }
        return result;
    }

    static std::string getResultAsString(const MeasurementResult& measurement_result, double factor)
    {
        std::stringstream out_stream;
        out_stream << "average(ns): " << measurement_result.average_duration
                   << " ---  min(ns):" << measurement_result.min_duration.count() << " --- min * "
                   << factor << "(ns): " << (measurement_result.min_duration.count() * factor)
                   << " --- all(ms): " << measurement_result.duration.count() << std::endl
                   << " count : " << measurement_result.count
                   << " zero_count : " << measurement_result.zero_count
                   << " inc_count : " << measurement_result.increasing_time_count
                   << " dec_count : " << measurement_result.decreasing_time_count << std::endl;
        return out_stream.str();
    }

    std::chrono::high_resolution_clock::time_point _first_time;
    std::chrono::high_resolution_clock::time_point _last_time;
    std::chrono::high_resolution_clock::time_point _current_begin_of_measurement;
    std::vector<std::chrono::nanoseconds> _measured_durations;
};

#endif // TEST_MEASUREMENT_H_INCLUDED
