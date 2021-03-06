/**
 * TimerH (https://github.com/UnexomWid/timerh)
 *
 * This project is licensed under the MIT license.
 * Copyright (c) 2018-2019 UnexomWid (https://uw.exom.dev)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef TIMERH_TIMER_H
#define TIMERH_TIMER_H

#include <chrono>
#include <string>

#define CHRONOMETER std::chrono::high_resolution_clock::time_point
#define ULL unsigned long long

// Gets the current time.
CHRONOMETER time_now();

// Gets the execution time in hours.
ULL get_exec_time_h(CHRONOMETER start);
// Gets the execution time in minutes.
ULL get_exec_time_m(CHRONOMETER start);
// Gets the execution time in seconds.
ULL get_exec_time_s(CHRONOMETER start);
// Gets the execution time in milliseconds.
ULL get_exec_time_ms(CHRONOMETER start);
// Gets the execution time in microseconds.
ULL get_exec_time_mis(CHRONOMETER start);
// Gets the execution time in nanoseconds.
ULL get_exec_time_ns(CHRONOMETER start);

// Formats a number representing milliseconds as a string.
std::string format_time_ms(ULL milliseconds);
// Formats a number representing microseconds as a string.
std::string format_time_mis(ULL microseconds);
// Formats a number representing nanoseconds as a string.
std::string format_time_ns(ULL nanoseconds);

// Gets the formatted execution time with the precision of a millisecond.
std::string getf_exec_time_ms(CHRONOMETER start);
// Gets the formatted execution time with the precision of a microsecond.
std::string getf_exec_time_mis(CHRONOMETER start);
// Gets the formatted execution time with the precision of a nanosecond.
std::string getf_exec_time_ns(CHRONOMETER start);

#endif