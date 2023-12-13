/**
 * @file
 * Public API for @ref a_util::experimental::NullaryDelegate "NullaryDelegate" and
 * @ref a_util::experimental::UnaryDelegate "UnaryDelegate" types
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_DELEGATE_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_DELEGATE_DECL_HEADER_INCLUDED

namespace a_util {
/**
 * @addtogroup a_util_core
 * @{
 * @cond INTERNAL_DOCUMENTATION
 */
namespace detail {
/// Contains delegate base class and types for delegates
namespace delegates {
template <typename ReturnType>
class DelegateInterface {
public:
    virtual ~DelegateInterface();
    virtual ReturnType invoke() = 0;
    virtual DelegateInterface<ReturnType>* clone() = 0;
};

template <typename ReturnType>
class FunctionDelegate : public DelegateInterface<ReturnType> {
    typedef ReturnType (*Function)();

public:
    FunctionDelegate(Function f);
    virtual ReturnType invoke();
    virtual DelegateInterface<ReturnType>* clone();

private:
    Function _func;
};

template <typename ReturnType, typename Method, typename Instance>
class MethodDelegate : public DelegateInterface<ReturnType> {
public:
    MethodDelegate(Method method, Instance& instance);
    virtual ReturnType invoke();
    virtual DelegateInterface<ReturnType>* clone();

private:
    Method _method;
    Instance& _instance;
};
} // namespace delegates

/// Contains delegate base class and types for functions
namespace functions {
template <typename ReturnType, typename ParamType>
class DelegateInterface {
public:
    virtual ~DelegateInterface();
    virtual ReturnType invoke(ParamType) = 0;
    virtual DelegateInterface<ReturnType, ParamType>* clone() = 0;
};

template <typename ReturnType, typename ParamType>
class FunctionDelegate : public DelegateInterface<ReturnType, ParamType> {
    typedef ReturnType (*Function)(ParamType);

public:
    FunctionDelegate(Function f);
    virtual ReturnType invoke(ParamType param);
    virtual DelegateInterface<ReturnType, ParamType>* clone();

private:
    Function _func;
};

template <typename ReturnType, typename ParamType, typename Method, typename Instance>
class MethodDelegate : public DelegateInterface<ReturnType, ParamType> {
public:
    MethodDelegate(Method method, Instance& inst);
    virtual ReturnType invoke(ParamType param);
    virtual DelegateInterface<ReturnType, ParamType>* clone();

private:
    Method _method;
    Instance& _instance;
};

} // namespace functions

/// Base class for delegates and functions (manages the actual callee object)
template <typename DelegateType>
class DelegateBase {
protected: // construction/destruction only available through specialized class
    ~DelegateBase();
    DelegateBase();
    DelegateBase(const DelegateBase& other);
    DelegateBase& operator=(const DelegateBase& other);

    void setDelegate(DelegateType* delegate);
    DelegateType* getDelegate();
    const DelegateType* getDelegate() const;

public:
    operator bool() const;

private:
    DelegateType* _ptr_to_callee;
};

} // namespace detail
/** @endcond */

namespace experimental {
/**
 * General delegate template: encapsulates a function or a method without a parameter
 * @tparam ReturnType Return type of the function to delegate
 */
template <typename ReturnType>
class NullaryDelegate
    : public detail::DelegateBase<detail::delegates::DelegateInterface<ReturnType>> {
public:
    /**
     * CTOR to delegate a function
     * @param[in] Function Pointer to the function to delegate
     */
    NullaryDelegate(ReturnType (*Function)());
    /**
     * CTOR to delegate a class method
     * @tparam Method Pointer to the method
     * @tparam Instance Class the method shall be invoked from
     * @param[in] method Pointer to the method to delegate
     * @param[in] instance Reference to the class instance the method is member of
     */
    template <typename Method, typename Instance>
    NullaryDelegate(Method method, Instance& instance);

    /**
     * Invocation operator - invokes the delegated method or function
     * @return Return value of the invoked function or method
     */
    ReturnType operator()();

private:
    typedef detail::DelegateBase<detail::delegates::DelegateInterface<ReturnType>> Base;
};

/**
 * General function template: encapsulates a function or a method with a single parameter
 * @tparam ReturnType Return type of the function to delegate
 * @tparam Parameter type of the method or function
 */
template <typename ReturnType, typename ParamType>
class UnaryDelegate
    : public detail::DelegateBase<detail::functions::DelegateInterface<ReturnType, ParamType>> {
public:
    /**
     * CTOR to delegate an unary function
     * @param[in] Function Pointer to the function to delegate
     */
    UnaryDelegate(ReturnType (*Function)(ParamType));

    /**
     * CTOR to delegate an unary class method
     * @tparam Method Pointer to the method
     * @tparam Instance Class the method shall be invoked from
     * @param[in] method Pointer to the method to delegate
     * @param[in] instance Reference to the class instance the method is member of
     */
    template <typename Method, typename Instance>
    UnaryDelegate(Method method, Instance& instance);

    /**
     * Invocation operator - invokes the delegated method or function
     * @param[in] param Parameter forwarded to the delegated function or method
     * @return Return value of the invoked function or method
     */
    ReturnType operator()(ParamType param);

private:
    typedef detail::DelegateBase<detail::functions::DelegateInterface<ReturnType, ParamType>> Base;
};
/** @} */ //

} // namespace experimental
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
#include <a_util/base/detail/delegate_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_DELEGATE_DECL_HEADER_INCLUDED
