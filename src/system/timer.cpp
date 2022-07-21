/**
 * @file
 * Timer API
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

#include <a_util/system.h>

#ifdef _WIN32

#ifndef __MINGW32__
#define NOMINMAX
#endif // __MINGW32__

#include <Windows.h> // timeSetEvent, timeKillEvent
#elif defined(__QNX__)
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

#include <algorithm>
#include <mutex>

namespace a_util {
namespace system {
struct Timer::Implementation {
    Timer* this_;
    a_util::experimental::NullaryDelegate<void> callback;
    bool is_running;
    std::uint64_t timer_period_us;
    HighResSchedulingSupport highres;
    mutable std::recursive_mutex mutex_timer;
    std::recursive_mutex mutex_callback;

#ifdef _WIN32
    MMRESULT native_timer;

    Implementation()
        : this_(nullptr),
          callback(&Implementation::doNothing),
          is_running(false),
          timer_period_us(0),
          highres(),
          mutex_timer(),
          mutex_callback(),
          native_timer(TIMERR_NOERROR)
    {
    }

    static void CALLBACK TimeProc(UINT uTimerID, UINT, DWORD_PTR dwUser, DWORD_PTR, DWORD_PTR)
    {
        Timer* self = (Timer*)dwUser;
        if (!self) {
            return;
        }

        Implementation* _impl = self->_impl;
        if (uTimerID != _impl->native_timer) {
            return;
        }

        {
            std::unique_lock<std::recursive_mutex> lock(_impl->mutex_callback);
            _impl->callback();
        }

        if (self->getPeriod() == 0) {
            self->stop();
        }
    }

#else
    std::unique_ptr<std::thread> thread;
    volatile bool stop_flag;
    int native_timer;

#ifdef __QNX__
    int channel_id;
    struct sigevent evtime;
    struct sigevent evstop;
    static const int CODE_ITIMER = _PULSE_CODE_MINAVAIL + 1;
    static const int CODE_EVSTOP = _PULSE_CODE_MINAVAIL + 2;

    Implementation()
        : this_(nullptr),
          callback(&Implementation::doNothing),
          is_running(false),
          timer_period_us(0),
          highres(),
          mutex_timer(),
          mutex_callback(),
          thread(),
          stop_flag(false),
          native_timer(-1),
          channel_id(0)
    {
        channel_id = ChannelCreate(0);
        int connect_id = ConnectAttach(ND_LOCAL_NODE, 0, channel_id, _NTO_SIDE_CHANNEL, 0);
        SIGEV_PULSE_INIT(&evtime, connect_id, SIGEV_PULSE_PRIO_INHERIT, CODE_ITIMER, 0);
        SIGEV_PULSE_INIT(&evstop, connect_id, SIGEV_PULSE_PRIO_INHERIT, CODE_EVSTOP, 0);
    }

    ~Implementation()
    {
        ConnectDetach(evtime.sigev_coid);
        ChannelDestroy(channel_id);
    }

#else
    Implementation()
        : this_(nullptr),
          callback(&Implementation::doNothing),
          is_running(false),
          timer_period_us(0),
          highres(),
          mutex_timer(),
          mutex_callback(),
          thread(),
          stop_flag(false),
          native_timer(-1)
    {
    }
#endif // __QNX__

    void ThreadFunc()
    {
        while (!stop_flag) {
            std::unique_lock<std::recursive_mutex> lock(mutex_callback);

#ifdef __QNX__
            struct _pulse pulse;
            while (!stop_flag && MsgReceivePulse(channel_id, &pulse, sizeof(pulse), nullptr) < 0) {
#else
            std::int64_t expirations = 0;
            while (!stop_flag && read(native_timer, &expirations, sizeof(expirations)) < 0) {
#endif // __QNX__
                continue;
            }
#ifdef __QNX__
            if (!stop_flag && pulse.code == CODE_ITIMER) {
#else
            if (!stop_flag) {
#endif // __QNX__
                callback();
            }

            if (timer_period_us == 0) {
                this_->stop();
                break;
            }
        }
    }
#endif // _WIN32
    // do not check for nullptr before every call to callback. use this dummy instead
    inline static void doNothing()
    {
    }
};

void Timer::init()
{
    _impl = new Implementation;
    _impl->is_running = false;
    _impl->timer_period_us = 0;
    _impl->this_ = this;
}

void Timer::setCallback(const a_util::experimental::NullaryDelegate<void>& cb)
{
    std::unique_lock<std::recursive_mutex> lock(_impl->mutex_callback);
    _impl->callback = cb;
}

/// Default CTOR

Timer::Timer()
{
    init();
}

Timer::~Timer()
{
    stop();
    delete _impl;
    _impl = nullptr;
}

/// Sets the callback for the timer

void Timer::setCallback(void (*Function)())
{
    setCallback(a_util::experimental::NullaryDelegate<void>(Function));
}

/// Function CTOR - Call \ref start to start the timer

Timer::Timer(std::uint64_t period_us, void (*Function)())
{
    init();
    setPeriod(period_us);
    setCallback(Function);
}

void Timer::setPeriod(std::uint64_t period_us)
{
    std::unique_lock<std::recursive_mutex> lock(_impl->mutex_timer);
    _impl->timer_period_us = period_us;
    if (_impl->is_running) {
        stop();
        start();
    }
}

std::uint64_t Timer::getPeriod() const
{
    std::unique_lock<std::recursive_mutex> lock(_impl->mutex_timer);
    return _impl->timer_period_us;
}

bool Timer::start()
{
    std::unique_lock<std::recursive_mutex> lock(_impl->mutex_timer);
    if (_impl->is_running) {
        return false;
    }

#ifdef _WIN32
    DWORD_PTR user = (DWORD_PTR)this;
    UINT period = static_cast<UINT>(_impl->timer_period_us / 1000);
    _impl->native_timer =
        timeSetEvent(std::max<UINT>(period, 1), 0, &Implementation::TimeProc, user, TIME_PERIODIC);

    _impl->is_running = _impl->native_timer != 0;

#else
    _impl->stop_flag = false;
#ifdef __QNX__
    if (timer_create(CLOCK_MONOTONIC, &_impl->evtime, &_impl->native_timer) != 0)
        _impl->native_timer = 0;
#else
    _impl->native_timer = timerfd_create(CLOCK_MONOTONIC, 0);
#endif // __QNX__
    _impl->is_running = _impl->native_timer >= 0;

    if (_impl->is_running) {
        time_t secs = _impl->timer_period_us / 1000000;
        long nsecs = std::max(1L, (long)((long)_impl->timer_period_us - (secs * 1000000)) * 1000);

        struct itimerspec timespec;
        if (_impl->timer_period_us == 0) {
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
        timer_settime(_impl->native_timer, 0, &timespec, nullptr);
#else
        timerfd_settime(_impl->native_timer, 0, &timespec, nullptr);
#endif // __QNX__
        _impl->thread.reset(new std::thread(&Implementation::ThreadFunc, _impl));
    }
#endif // _WIN32

    return _impl->is_running;
}

bool Timer::stop()
{
    std::unique_lock<std::recursive_mutex> lock(_impl->mutex_timer);
    if (!_impl->is_running) {
        return false;
    }

#ifdef _WIN32
    timeKillEvent(_impl->native_timer);
    _impl->native_timer = TIMERR_NOERROR;
    _impl->is_running = false;
    lock.unlock();
    std::unique_lock<std::recursive_mutex> lock2(_impl->mutex_callback);
#else
    _impl->stop_flag = true;
#ifdef __QNX__
    timer_delete(_impl->native_timer);
    _impl->native_timer = -1;
    // if stopped from outside the thread send a stop message to wake it up
    if (_impl->thread->get_id() != std::this_thread::get_id()) {
        MsgDeliverEvent(0, &_impl->evstop);
    }
#else
    close(_impl->native_timer);
    _impl->native_timer = -1;
#endif // __QNX__
    _impl->is_running = false;

    // only wait for exit if we're not stopped from within the thread itself
    if (_impl->thread->get_id() != std::this_thread::get_id()) {
        _impl->thread->join();
        _impl->thread.reset();
        _impl->stop_flag = false;
    }
    else {
        // otherwise just detach and leave stop_flag on
        _impl->thread->detach();
        _impl->thread.reset();
    }
#endif // _WIN32

    return true;
}

bool Timer::isRunning() const
{
    std::unique_lock<std::recursive_mutex> lock(_impl->mutex_timer);
    return _impl->is_running;
}

} // namespace system
} // namespace a_util
