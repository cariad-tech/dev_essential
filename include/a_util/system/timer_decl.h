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
@endverbatim
 */

#ifndef A_UTIL_UTIL_SYSTEM_TIMER_DECL_INCLUDED
#define A_UTIL_UTIL_SYSTEM_TIMER_DECL_INCLUDED

#include <cstdint>

namespace a_util {
namespace result {
namespace detail {

// forward declare private reference counter
template <typename T, typename U>
class ReferenceCountedObject;

} // namespace detail
} // namespace result

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
     * @param[in] period_us Duration between calls to @c function (in microseconds)
     *                      0 -> One shot timer
     * @param[in] function Pointer to callback function
     */
    Timer(std::uint64_t period_us, void (*function)());

    /**
     * CTOR for methods as callbacks
     *
     * To start the timer, call @ref start
     * @tparam M Method type to call
     * @tparam T Class containing the method
     * @param[in] period_us Duration between calls to @c function (in microseconds).
     *                      0 -> One shot timer
     * @param[in] method Method used as callback
     * @param[in] instance Instance of the class the method to invoke from
     */
    template <typename M, typename T>
    Timer(std::uint64_t period_us, M method, T& instance);

    /**
     * Set the callback for the timer
     * @param[in] function Pointer to callback function
     */
    void setCallback(void (*function)());

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
     * @param[in] period_us Duration in microseconds between invocations of callback function.
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
    void setCallback(const a_util::experimental::NullaryDelegate<void>& cb);

private: // temporary IntrusivePtr implementation until it can be released publically
    template <typename T>
    class IntrusivePtr {
        using reference_counted_object_pointer =
            a_util::result::detail::ReferenceCountedObject<T, T>*;

    public:
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;

        // construction/destruction
        explicit IntrusivePtr(reference_counted_object_pointer reference_counted_object)
            : _reference_counted_object{reference_counted_object}
        {
            // nullptr check currently missing on purpose, has to be added if generalized
            // maybe a nullptr check doesn't make sense in ctor due to move operators setting the
            // member to nullptr anyway ...
            addReference();
        }

        ~IntrusivePtr() noexcept
        {
            removeReference();
        }

        // copy constructor
        IntrusivePtr(const IntrusivePtr& other) : IntrusivePtr{other._reference_counted_object}
        {
        }

        // copy assignment
        IntrusivePtr& operator=(const IntrusivePtr& other)
        {
            removeReference();
            _reference_counted_object = other._reference_counted_object;
            addReference();
            return *this;
        }

        // move constructor
        IntrusivePtr(IntrusivePtr&& other) noexcept
            : _reference_counted_object{other._reference_counted_object}
        {
            other.release();
        }

        // move assignment
        IntrusivePtr& operator=(IntrusivePtr&& other) noexcept
        {
            _reference_counted_object = other._reference_counted_object;
            other.release();
            return *this;
        }

    public: // access/modification
        // maybe return number of references instead of void?
        void addReference() const noexcept
        {
            _reference_counted_object->addReference();
        }

        // maybe return number of references instead of void?
        void removeReference() const noexcept
        {
            _reference_counted_object->removeReference();
        }

        // maybe return pointer to member instead of void?
        void release() noexcept
        {
            _reference_counted_object = nullptr;
        }

        auto operator->() const noexcept -> const_pointer
        {
            return &(_reference_counted_object->getObject());
        }

        auto operator->() noexcept -> pointer
        {
            return &(_reference_counted_object->getObject());
        }

        auto operator*() noexcept -> reference
        {
            return *(this->operator->());
        }

        auto operator*() const noexcept -> const_reference
        {
            return *(this->operator->());
        }

    private:
        reference_counted_object_pointer _reference_counted_object;
    }; // IntrusivePtr

    template <typename T, typename... Args>
    static auto makeIntrusive(Args&&... args) -> IntrusivePtr<T>;

private:
    struct Implementation;
    IntrusivePtr<Implementation> _impl;
};

} // namespace system
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/system/detail/timer_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_SYSTEM_TIMER_DECL_INCLUDED
