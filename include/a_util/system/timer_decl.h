/**
 * @file
 * Public API for @ref a_util::system::Timer "Timer" class
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef A_UTIL_UTIL_SYSTEM_TIMER_DECL_INCLUDED
#define A_UTIL_UTIL_SYSTEM_TIMER_DECL_INCLUDED

#include <cstdint>

namespace a_util {
namespace experimental {
// forward declaration
template <typename T>
class NullaryDelegate;

} // namespace experimental
namespace system {
/**
 * Periodic timer running in a separate thread
 *
 * - Invokes a callback periodically
 * - If an invocation takes longer than the period, any missed expirations are lost!
 * - The callback is invoked from a different thread, take care about thread safety!
 */
class Timer {
public:
    /// Default CTOR
    Timer();

    /// DTOR - Calls @ref stop()
    ~Timer();

    /**
     * CTOR for functions as callbacks
     *
     * To start the timer, call @ref start
     * @param[in] period_us Duration between calls to @c Function (in microseconds)
     *                      0 -> One shot timer
     * @param[in] Function Function used as callback
     */
    Timer(std::uint64_t period_us, void (*Function)());

    /**
     * CTOR for methods as callbacks
     *
     * To start the timer, call @ref start
     * @tparam M Method type to call
     * @tparam T Class containing the method
     * @param[in] period_us Duration between calls to @c Function (in microseconds).
     *                      0 -> One shot timer
     * @param[in] method Method used as callback
     * @param[in] instance Instance of the class the method to invoke from
     */
    template <typename M, typename T>
    Timer(std::uint64_t period_us, M method, T& instance);

    /**
     * Set the callback for the timer
     * @param[in] Function Function used as a callback
     */
    void setCallback(void (*Function)());

    /**
     * Set the callback for the timer
     * @tparam M Method type to call
     * @tparam T Class containing the method
     * @param[in] method Method used as callback
     * @param[in] instance Instance of the class the method to invoke from
     */
    template <typename M, typename T>
    void setCallback(M method, T& instance);

    /**
     * Set the period used by this timer, restarting the timer if already running
     * @param[in] period_us Duration between calls to @c Function (in microseconds).
     *                      0 -> One shot timer
     */
    void setPeriod(std::uint64_t period_us);

    /**
     * Get the currently configured period of the timer
     * @return Period of the timer in microseconds
     */
    std::uint64_t getPeriod() const;

    /**
     * Start the timer
     * @return @c true on success, @c false if the timer is already running
     */
    bool start();

    /**
     * Stop the timer - blocks until the callback returns
     * @return @c true on success, @c false if the timer is already stopped
     */
    bool stop();

    /**
     * Check whether the timer is currently running
     * @return @c true if timer is running, @c false otherwise
     */
    bool isRunning() const;

private:
    Timer(const Timer&);            // = delete;
    Timer& operator=(const Timer&); // = delete;
    void init();
    void setCallback(const a_util::experimental::NullaryDelegate<void>& cb);

private:
    struct Implementation;
    Implementation* _impl;
};

} // namespace system
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include "a_util/system/detail/timer_impl.h"
/** @endcond */

#endif // A_UTIL_UTIL_SYSTEM_TIMER_DECL_INCLUDED
