/**
 * @file
 * Timer API
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifdef _WIN32
// timeSetEvent, timeKillEvent
#include <Windows.h>
#endif // _WIN32

#include <a_util/result/detail/reference_counted_object.h>
#include <a_util/system/timer.h>

#include <algorithm>
#include <atomic>
#include <mutex>

#ifndef _WIN32
#if defined(__QNX__)
#if !defined(_QNX_SOURCE)
#define _QNX_SOURCE
#endif
#include <sys/netmgr.h>   // ND_LOCAL_NODE
#include <sys/neutrino.h> // ChannelCreate(), ChannelDestroy(), ConnectAttach(), ConnectDetach(), ...
#include <sys/siginfo.h>
#include <sys/time.h> // timer_create, itimerspec, timer_settime, timer_delete
#include <thread>
#include <unistd.h>
#else
#include <sys/timerfd.h> // timerfd_create, itimerspec, timerfd_settime
#include <thread>
#include <unistd.h>
#endif
#endif // !_WIN32

namespace a_util {
namespace system {

template <typename T, typename... Args>
auto Timer::makeIntrusive(Args&&... args) -> Timer::IntrusivePtr<T>
{
    return Timer::IntrusivePtr<T>{
        new a_util::result::detail::ReferenceCountedObject<T, T>(std::forward<Args>(args)...)};
}

#ifdef __QNX__
// prevent "error: ignoring attributes on template argument 'std::uint64_t {aka long long
// unsigned int}'"
using timer_period_atomic_value_type = long long unsigned int;
#else  // !__QNX__
using timer_period_atomic_value_type = std::uint64_t;
#endif // __QNX__

struct Timer::Implementation {
    // initialized via initializer list
    Timer* const _timer;
    a_util::experimental::NullaryDelegate<void> _callback;
    std::atomic<timer_period_atomic_value_type> _timer_period_us;

    std::atomic<bool> _is_running = {};
    std::recursive_mutex _mutex_timer = {};
    std::recursive_mutex _mutex_callback = {};

    Implementation(Timer* timer) : Implementation(timer, 0, &Implementation::doNothing)
    {
    }

    Implementation(Timer* timer, std::uint64_t period_us, void (*Function)())
        : _timer{timer}, _callback{Function}, _timer_period_us{period_us}
    {
#ifdef __QNX__
        const auto connect_id = ConnectAttach(ND_LOCAL_NODE, 0, _channel_id, _NTO_SIDE_CHANNEL, 0);
        SIGEV_PULSE_INIT(&_evtime, connect_id, SIGEV_PULSE_PRIO_INHERIT, code_itimer, 0);
        SIGEV_PULSE_INIT(&_evstop, connect_id, SIGEV_PULSE_PRIO_INHERIT, code_evstop, 0);
#endif // __QNX__
    }

    ~Implementation() noexcept
    {
#ifdef __QNX__
        ConnectDetach(_evtime.sigev_coid);
        ChannelDestroy(_channel_id);
#endif // __QNX__
    }

    Implementation(const Implementation&) = delete;
    Implementation& operator=(const Implementation&) = delete;
    Implementation(Implementation&&) = delete;
    Implementation& operator=(Implementation&&) = delete;

#ifdef _WIN32
    MMRESULT _native_timer = {};

    static void CALLBACK TimeProc(UINT uTimerID, UINT, DWORD_PTR dwUser, DWORD_PTR, DWORD_PTR)
    {
        Timer* const self = (Timer*)dwUser;
        if (!self) {
            return;
        }

        auto& impl = self->_impl;
        if (uTimerID != impl->_native_timer) {
            return;
        }

        {
            std::unique_lock<std::recursive_mutex> lock(impl->_mutex_callback);
            impl->_callback();
        }

        if (self->getPeriod() == 0) {
            self->stop();
        }
    }

    void startTimer()
    {
        std::unique_lock<std::recursive_mutex> lock(_mutex_timer);

        DWORD_PTR timer_ptr = (DWORD_PTR)_timer;
        const UINT event_delay_ms = static_cast<UINT>(_timer_period_us / 1000);
        const auto one_shot_or_periodic = (_timer_period_us > 0) ? TIME_PERIODIC : TIME_ONESHOT;
        _native_timer = timeSetEvent(std::max<UINT>(event_delay_ms, 1),
                                     0,
                                     &Implementation::TimeProc,
                                     timer_ptr,
                                     one_shot_or_periodic | TIME_KILL_SYNCHRONOUS);
        // See: https://learn.microsoft.com/en-us/previous-versions/dd757634(v=vs.85)#return-value
        _is_running = NULL != _native_timer;
    }

    void stopTimer()
    {
        std::unique_lock<std::recursive_mutex> lock(_mutex_timer);
        timeKillEvent(_native_timer);
        _native_timer = {};
        _is_running = false;
    }

#else // !_WIN32
    std::unique_ptr<std::thread> _thread = {};
    std::atomic<bool> _stop_flag = {};
    int _native_timer = {-1};

#ifndef __QNX__

    void ThreadFunc()
    {
        while (!_stop_flag) {
            std::unique_lock<std::recursive_mutex> lock(_mutex_callback);
            std::int64_t expirations = 0;
            while (!_stop_flag && read(_native_timer, &expirations, sizeof(expirations)) < 0) {
                continue;
            }
            if (!_stop_flag) {
                _callback();
            }

            if (_timer_period_us == 0) {
                _timer->stop();
                break;
            }
        }
    }

#else  // __QNX__
    const int _channel_id = {ChannelCreate(0)};
    struct sigevent _evtime = {};
    struct sigevent _evstop = {};
    static const int code_itimer = _PULSE_CODE_MINAVAIL + 1;
    static const int code_evstop = _PULSE_CODE_MINAVAIL + 2;

    void ThreadFunc()
    {
        while (!_stop_flag) {
            std::unique_lock<std::recursive_mutex> lock(_mutex_callback);
            struct _pulse pulse;
            while (!_stop_flag &&
                   MsgReceivePulse(_channel_id, &pulse, sizeof(pulse), nullptr) < 0) {
                continue;
            }
            if (!_stop_flag && pulse.code == code_itimer) {
                _callback();
            }

            if (_timer_period_us == 0) {
                _timer->stop();
                break;
            }
        }
    }
#endif // !__QNX__

    void startTimer()
    {
        std::unique_lock<std::recursive_mutex> lock(_mutex_timer);
        _stop_flag = false;
#ifdef __QNX__
        if (timer_create(CLOCK_MONOTONIC, &_evtime, &_native_timer) != 0)
            _native_timer = 0;
#else
        _native_timer = timerfd_create(CLOCK_MONOTONIC, 0);
#endif // __QNX__
        _is_running = _native_timer >= 0;

        if (_is_running) {
            time_t secs = _timer_period_us / 1000000;
            long nsecs = std::max(1L, (long)((long)_timer_period_us - (secs * 1000000)) * 1000);

            struct itimerspec timespec;
            if (_timer_period_us == 0) {
                timespec.it_interval.tv_sec = 0;
                timespec.it_interval.tv_nsec = 0;
            }
            else {
                timespec.it_interval.tv_sec = secs;
                timespec.it_interval.tv_nsec = nsecs;
            }
            timespec.it_value.tv_sec = secs;
            timespec.it_value.tv_nsec = nsecs;

#ifdef __QNX__
            timer_settime(_native_timer, 0, &timespec, nullptr);
#else
            timerfd_settime(_native_timer, 0, &timespec, nullptr);
#endif // __QNX__
            _thread.reset(new std::thread(&Implementation::ThreadFunc, this));
        }
    }

    void stopTimer()
    {
        std::unique_lock<std::recursive_mutex> lock(_mutex_timer);
        _stop_flag = true;
#ifdef __QNX__
        timer_delete(_native_timer);
        _native_timer = -1;
        // if stopped from outside the thread send a stop message to wake it up
        if (_thread->get_id() != std::this_thread::get_id()) {
            MsgDeliverEvent(0, &_evstop);
        }
#else
        close(_native_timer);
        _native_timer = -1;
#endif // __QNX__
        _is_running = false;

        // only wait for exit if we're not stopped from within the thread itself
        if (_thread->get_id() != std::this_thread::get_id()) {
            _thread->join();
            _thread.reset();
            _stop_flag = false;
        }
        else {
            // otherwise just detach and leave _stop_flag on
            _thread->detach();
            _thread.reset();
        }
    }
#endif // _WIN32
    // do not check for nullptr before every call to callback. use this dummy instead
    inline static void doNothing()
    {
    }
};

Timer::Timer() : _impl{makeIntrusive<Implementation>(this)}
{
}

Timer::Timer(std::uint64_t period_us, void (*Function)()) : Timer{}
{
    setPeriod(period_us);
    setCallback(Function);
}

Timer::~Timer()
{
    stop();
}

void Timer::setCallback(void (*Function)())
{
    setCallback(a_util::experimental::NullaryDelegate<void>(Function));
}

void Timer::setCallback(const a_util::experimental::NullaryDelegate<void>& cb)
{
    std::unique_lock<std::recursive_mutex> lock(_impl->_mutex_callback);
    _impl->_callback = cb;
}

void Timer::setPeriod(std::uint64_t period_us)
{
    _impl->_timer_period_us = period_us;
    if (isRunning()) {
        stop();
        start();
    }
}

std::uint64_t Timer::getPeriod() const
{
    return _impl->_timer_period_us;
}

bool Timer::start()
{
    if (isRunning()) {
        return false;
    }
    _impl.addReference();
    _impl->startTimer();
    return isRunning();
}

bool Timer::stop()
{
    if (!isRunning()) {
        return false;
    }
    _impl->stopTimer();
    _impl.removeReference();
    return true;
}

bool Timer::isRunning() const
{
    return _impl->_is_running;
}

} // namespace system
} // namespace a_util
