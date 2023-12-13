/**
 * @file
 * Fallback implementation for variadic templates support
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_BASE_VARIADIC_HEADER_INCLUDED
#define A_UTIL_UTIL_BASE_VARIADIC_HEADER_INCLUDED

#include <a_util/base/std/types.h>

namespace a_util {
namespace base {

// forward declaration
template <typename T, typename Next>
struct variadic;

namespace detail {

struct variadic_end;

template <typename VaArgs, typename U>
struct variadic_result;

template <typename U>
struct variadic_result<variadic_end, U> {
    typedef variadic<U, variadic_end> type;
};

template <typename T, typename U>
struct variadic_result<variadic<T, variadic_end>, U> {
    typedef variadic<T, variadic<U, variadic_end>> type;
};

template <typename T, typename Next, typename U>
struct variadic_result<variadic<T, Next>, U> {
    typedef variadic<T, typename variadic_result<Next, U>::type> type;
};

struct variadic_end {
    template <typename U>
    variadic<U, variadic_end> operator()(U u)
    {
        return variadic<U, variadic_end>(u, variadic_end());
    }
};

// template<typename T0,
//         typename T1 = void,
//         typename T2 = void,
//         typename T3 = void,
//         typename T4 = void,
//         typename T5 = void,
//         typename T6 = void,
//         typename T7 = void,
//         typename T8 = void,
//         typename T9 = void
//         >
// struct variadic_type
//{
//    typedef variadic<T9, typename variadic_type<T8>::type>    type;
//};

// template<typename T0>
// struct variadic_type<T0, void, void, void, void, void, void, void, void, void>
//{
//    typedef variadic<T0, variadic_end>                        type;
//};

// template<typename T0,
//         typename T1>
// struct variadic_type<T0, T1, void, void, void, void, void, void, void, void>
//{
//    typedef variadic<T1, typename variadic_type<T0>::type>    type;
//};

// template<typename T0,
//         typename T1,
//         typename T2>
// struct variadic_type<T0, T1, T2, void, void, void, void, void, void, void>
//{
//    typedef variadic<T2, typename variadic_type<T1>::type>    type;
//};

// template<typename T0,
//         typename T1,
//         typename T2,
//         typename T3>
// struct variadic_type<T0, T1, T2, T3, void, void, void, void, void, void>
//{
//    typedef variadic<T3, typename variadic_type<T2>::type>    type;
//};

// template<typename T0,
//         typename T1,
//         typename T2,
//         typename T3,
//         typename T4>
// struct variadic_type<T0, T1, T2, T3, T4, void, void, void, void, void>
//{
//    typedef variadic<T4, typename variadic_type<T3>::type>    type;
//};

// template<typename T0,
//         typename T1,
//         typename T2,
//         typename T3,
//         typename T4,
//         typename T5>
// struct variadic_type<T0, T1, T2, T3, T4, T5, void, void, void, void>
//{
//    typedef variadic<T5, typename variadic_type<T4>::type>    type;
//};

// template<typename T0,
//         typename T1,
//         typename T2,
//         typename T3,
//         typename T4,
//         typename T5,
//         typename T6>
// struct variadic_type<T0, T1, T2, T3, T4, T5, T6, void, void, void>
//{
//    typedef variadic<T6, typename variadic_type<T5>::type>    type;
//};

// template<typename T0,
//         typename T1,
//         typename T2,
//         typename T3,
//         typename T4,
//         typename T5,
//         typename T6,
//         typename T7>
// struct variadic_type<T0, T1, T2, T3, T4, T5, T6, T7, void, void>
//{
//    typedef variadic<T7, typename variadic_type<T6>::type>    type;
//};

// template<typename T0,
//         typename T1,
//         typename T2,
//         typename T3,
//         typename T4,
//         typename T5,
//         typename T6,
//         typename T7,
//         typename T8>
// struct variadic_type<T0, T1, T2, T3, T4, T5, T6, T7, T8, void>
//{
//    typedef variadic<T8, typename variadic_type<T7>::type>    type;
//};

template <typename T, std::uint32_t Argn>
struct get_arg;

template <typename T>
struct get_arg<T, 0> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.value;
    }
};

template <typename T>
struct get_arg<T, 1> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.value;
    }
};

template <typename T>
struct get_arg<T, 2> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.value;
    }
};

template <typename T>
struct get_arg<T, 3> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.next.value;
    }
};

template <typename T>
struct get_arg<T, 4> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.next.next.value;
    }
};

template <typename T>
struct get_arg<T, 5> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.next.next.next.value;
    }
};

template <typename T>
struct get_arg<T, 6> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.next.next.next.next.value;
    }
};

template <typename T>
struct get_arg<T, 7> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.next.next.next.next.next.value;
    }
};

template <typename T>
struct get_arg<T, 8> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.next.next.next.next.next.next.value;
    }
};

template <typename T>
struct get_arg<T, 9> {
    template <typename VaArgs>
    static T from(VaArgs& args)
    {
        return args.next.next.next.next.next.next.next.next.next.value;
    }
};

} // namespace detail

template <typename T, typename Next>
struct variadic {
    variadic(T t, Next n) : value(t), next(n)
    {
    }

    T value;
    Next next;

    template <typename U>
    typename detail::variadic_result<variadic, U>::type operator()(U u)
    {
        typedef typename detail::variadic_result<variadic, U>::type Result;
        return Result(value, next(u));
    }
};

template <typename T>
variadic<T, detail::variadic_end> make_variadic(T t)
{
    return variadic<T, detail::variadic_end>(t, detail::variadic_end());
}

template <typename T, typename U, typename Next>
variadic<T, variadic<U, Next>> make_variadic(T t, const variadic<U, Next>& args)
{
    return variadic<T, variadic<U, Next>>(t, args);
}

template <typename Ret, typename T0>
Ret make_from_variadic(const variadic<T0, detail::variadic_end>& args)
{
    return Ret(args.value);
}

template <typename Ret, typename T0, typename T1>
Ret make_from_variadic(const variadic<T0, variadic<T1, detail::variadic_end>>& args)
{
    return Ret(detail::get_arg<T0, 0>::from(args), detail::get_arg<T1, 1>::from(args));
}

template <typename Ret, typename T0, typename T1, typename T2>
Ret make_from_variadic(const variadic<T0, variadic<T1, variadic<T2, detail::variadic_end>>>& args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args));
}

template <typename Ret, typename T0, typename T1, typename T2, typename T3>
Ret make_from_variadic(
    const variadic<T0, variadic<T1, variadic<T2, variadic<T3, detail::variadic_end>>>>& args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args),
               detail::get_arg<T3, 3>::from(args));
}

template <typename Ret, typename T0, typename T1, typename T2, typename T3, typename T4>
Ret make_from_variadic(
    const variadic<T0,
                   variadic<T1, variadic<T2, variadic<T3, variadic<T4, detail::variadic_end>>>>>&
        args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args),
               detail::get_arg<T3, 3>::from(args),
               detail::get_arg<T4, 4>::from(args));
}

template <typename Ret,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5>
Ret make_from_variadic(
    const variadic<
        T0,
        variadic<T1, variadic<T2, variadic<T3, variadic<T4, variadic<T5, detail::variadic_end>>>>>>&
        args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args),
               detail::get_arg<T3, 3>::from(args),
               detail::get_arg<T4, 4>::from(args),
               detail::get_arg<T5, 5>::from(args));
}

template <typename Ret,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6>
Ret make_from_variadic(
    const variadic<
        T0,
        variadic<
            T1,
            variadic<
                T2,
                variadic<T3, variadic<T4, variadic<T5, variadic<T6, detail::variadic_end>>>>>>>&
        args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args),
               detail::get_arg<T3, 3>::from(args),
               detail::get_arg<T4, 4>::from(args),
               detail::get_arg<T5, 5>::from(args),
               detail::get_arg<T6, 6>::from(args));
}

template <typename Ret,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7>
Ret make_from_variadic(
    const variadic<
        T0,
        variadic<
            T1,
            variadic<
                T2,
                variadic<
                    T3,
                    variadic<T4,
                             variadic<T5, variadic<T6, variadic<T7, detail::variadic_end>>>>>>>>&
        args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args),
               detail::get_arg<T3, 3>::from(args),
               detail::get_arg<T4, 4>::from(args),
               detail::get_arg<T5, 5>::from(args),
               detail::get_arg<T6, 6>::from(args),
               detail::get_arg<T7, 7>::from(args));
}

template <typename Ret,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8>
Ret make_from_variadic(
    const variadic<
        T0,
        variadic<
            T1,
            variadic<
                T2,
                variadic<
                    T3,
                    variadic<
                        T4,
                        variadic<
                            T5,
                            variadic<T6, variadic<T7, variadic<T8, detail::variadic_end>>>>>>>>>&
        args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args),
               detail::get_arg<T3, 3>::from(args),
               detail::get_arg<T4, 4>::from(args),
               detail::get_arg<T5, 5>::from(args),
               detail::get_arg<T6, 6>::from(args),
               detail::get_arg<T7, 7>::from(args),
               detail::get_arg<T8, 8>::from(args));
}

template <typename Ret,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9>

Ret make_from_variadic(
    const variadic<
        T0,
        variadic<
            T1,
            variadic<
                T2,
                variadic<
                    T3,
                    variadic<
                        T4,
                        variadic<
                            T5,
                            variadic<
                                T6,
                                variadic<T7,
                                         variadic<T8, variadic<T9, detail::variadic_end>>>>>>>>>>&
        args)
{
    return Ret(detail::get_arg<T0, 0>::from(args),
               detail::get_arg<T1, 1>::from(args),
               detail::get_arg<T2, 2>::from(args),
               detail::get_arg<T3, 3>::from(args),
               detail::get_arg<T4, 4>::from(args),
               detail::get_arg<T5, 5>::from(args),
               detail::get_arg<T6, 6>::from(args),
               detail::get_arg<T7, 7>::from(args),
               detail::get_arg<T8, 8>::from(args),
               detail::get_arg<T9, 9>::from(args));
}

} // namespace base
} // namespace a_util

#endif // A_UTIL_UTIL_BASE_VARIADIC_HEADER_INCLUDED
