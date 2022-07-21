/**
 * @file
 * Private implementation for @ref a_util::experimental::NullaryDelegate "NullaryDelegate" and
 * @ref a_util::experimental::UnaryDelegate "UnaryDelegate" types
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

#ifndef A_UTIL_UTIL_DELEGATE_IMPL_HEADER_INCLUDED
#define A_UTIL_UTIL_DELEGATE_IMPL_HEADER_INCLUDED

#include <a_util/base/detail/delegate_decl.h>

#include <stdexcept>

namespace a_util {
namespace detail {
namespace delegates {
template <typename ReturnType>
inline DelegateInterface<ReturnType>::~DelegateInterface()
{
}

template <typename ReturnType>
inline FunctionDelegate<ReturnType>::FunctionDelegate(Function f) : _func(f)
{
}

template <typename ReturnType>
inline ReturnType FunctionDelegate<ReturnType>::invoke()
{
    return _func();
}

template <typename ReturnType>
inline DelegateInterface<ReturnType>* FunctionDelegate<ReturnType>::clone()
{
    return new FunctionDelegate(_func);
}

template <typename ReturnType, typename Method, typename Instance>
inline MethodDelegate<ReturnType, Method, Instance>::MethodDelegate(Method method,
                                                                    Instance& instance)
    : _method(method), _instance(instance)
{
}

template <typename ReturnType, typename Method, typename Instance>
inline ReturnType MethodDelegate<ReturnType, Method, Instance>::invoke()
{
    return (_instance.*_method)();
}

template <typename ReturnType, typename Method, typename Instance>
inline DelegateInterface<ReturnType>* MethodDelegate<ReturnType, Method, Instance>::clone()
{
    return new MethodDelegate(_method, _instance);
}

} // namespace delegates

namespace functions {
template <typename ReturnType, typename ParamType>
DelegateInterface<ReturnType, ParamType>::~DelegateInterface()
{
}

template <typename ReturnType, typename ParamType>
inline FunctionDelegate<ReturnType, ParamType>::FunctionDelegate(Function f) : _func(f)
{
}
template <typename ReturnType, typename ParamType>
inline ReturnType FunctionDelegate<ReturnType, ParamType>::invoke(ParamType param)
{
    return _func(param);
}

template <typename ReturnType, typename ParamType>
inline DelegateInterface<ReturnType, ParamType>* FunctionDelegate<ReturnType, ParamType>::clone()
{
    return new FunctionDelegate(_func);
}

template <typename ReturnType, typename ParamType, typename Method, typename Instance>
MethodDelegate<ReturnType, ParamType, Method, Instance>::MethodDelegate(Method method,
                                                                        Instance& inst)
    : _method(method), _instance(inst)
{
}

template <typename ReturnType, typename ParamType, typename Method, typename Instance>
inline ReturnType MethodDelegate<ReturnType, ParamType, Method, Instance>::invoke(ParamType param)
{
    return (_instance.*_method)(param);
}

template <typename ReturnType, typename ParamType, typename Method, typename Instance>
inline DelegateInterface<ReturnType, ParamType>* MethodDelegate<ReturnType,
                                                                ParamType,
                                                                Method,
                                                                Instance>::clone()
{
    return new MethodDelegate(_method, _instance);
}

} // namespace functions

template <typename DelegateType>
inline DelegateBase<DelegateType>::~DelegateBase()
{
    delete _ptr_to_callee;
}

template <typename DelegateType>
inline DelegateBase<DelegateType>::DelegateBase() : _ptr_to_callee(nullptr)
{
}

template <typename DelegateType>
inline DelegateBase<DelegateType>::DelegateBase(const DelegateBase& other) : _ptr_to_callee(nullptr)
{
    if (other._ptr_to_callee) {
        _ptr_to_callee = other._ptr_to_callee->clone();
    }
}

template <typename DelegateType>
inline DelegateBase<DelegateType>& DelegateBase<DelegateType>::operator=(const DelegateBase& other)
{
    if (&other != this) {
        delete _ptr_to_callee;
        _ptr_to_callee = nullptr;
        if (other._ptr_to_callee) {
            _ptr_to_callee = other._ptr_to_callee->clone();
        }
    }
    return *this;
}

template <typename DelegateType>
inline void DelegateBase<DelegateType>::setDelegate(DelegateType* delegate)
{
    _ptr_to_callee = delegate;
}

template <typename DelegateType>
inline DelegateType* DelegateBase<DelegateType>::getDelegate()
{
    return _ptr_to_callee;
}

template <typename DelegateType>
inline const DelegateType* DelegateBase<DelegateType>::getDelegate() const
{
    return _ptr_to_callee;
}

template <typename DelegateType>
inline DelegateBase<DelegateType>::operator bool() const
{
    return getDelegate() != nullptr;
}

} // namespace detail

namespace experimental {
template <typename ReturnType>
inline NullaryDelegate<ReturnType>::NullaryDelegate(ReturnType (*Function)())
{
    if (nullptr != Function) {
        using namespace detail::delegates;
        this->setDelegate(new FunctionDelegate<ReturnType>(Function));
    }
}

template <typename ReturnType>
template <typename Method, typename Instance>
inline NullaryDelegate<ReturnType>::NullaryDelegate(Method method, Instance& instance)
{
    if (Method(nullptr) != method) {
        using namespace detail::delegates;
        this->setDelegate(new MethodDelegate<ReturnType, Method, Instance>(method, instance));
    }
}

template <typename ReturnType>
inline ReturnType NullaryDelegate<ReturnType>::operator()()
{
    if (!(*this))
        throw std::runtime_error("Uninitialized Delegate invoked!");
    return this->getDelegate()->invoke();
}

template <typename ReturnType, typename ParamType>
inline UnaryDelegate<ReturnType, ParamType>::UnaryDelegate(ReturnType (*Function)(ParamType))
{
    if (nullptr != Function) {
        using namespace detail::functions;
        this->setDelegate(new FunctionDelegate<ReturnType, ParamType>(Function));
    }
}

template <typename ReturnType, typename ParamType>
template <typename Method, typename Instance>
inline UnaryDelegate<ReturnType, ParamType>::UnaryDelegate(Method method, Instance& instance)
{
    if (Method(nullptr) != method) {
        using namespace detail::functions;
        this->setDelegate(
            new MethodDelegate<ReturnType, ParamType, Method, Instance>(method, instance));
    }
}

template <typename ReturnType, typename ParamType>
inline ReturnType UnaryDelegate<ReturnType, ParamType>::operator()(ParamType param)
{
    if (!(*this)) {
        throw std::runtime_error("Uninitialized function invoked!");
    }
    return this->getDelegate()->invoke(param);
}

} // namespace experimental
} // namespace a_util

#endif // A_UTIL_UTIL_DELEGATE_IMPL_HEADER_INCLUDED
