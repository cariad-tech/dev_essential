
// This is a generated file. Do not edit!

#ifndef A_UTIL_ENABLE_COMPILER_DETECTION_H
#define A_UTIL_ENABLE_COMPILER_DETECTION_H

#ifdef __cplusplus
#define A_UTIL_ENABLE_COMPILER_IS_Comeau 0
#define A_UTIL_ENABLE_COMPILER_IS_Intel 0
#define A_UTIL_ENABLE_COMPILER_IS_PathScale 0
#define A_UTIL_ENABLE_COMPILER_IS_Embarcadero 0
#define A_UTIL_ENABLE_COMPILER_IS_Borland 0
#define A_UTIL_ENABLE_COMPILER_IS_Watcom 0
#define A_UTIL_ENABLE_COMPILER_IS_OpenWatcom 0
#define A_UTIL_ENABLE_COMPILER_IS_SunPro 0
#define A_UTIL_ENABLE_COMPILER_IS_HP 0
#define A_UTIL_ENABLE_COMPILER_IS_Compaq 0
#define A_UTIL_ENABLE_COMPILER_IS_zOS 0
#define A_UTIL_ENABLE_COMPILER_IS_XLClang 0
#define A_UTIL_ENABLE_COMPILER_IS_XL 0
#define A_UTIL_ENABLE_COMPILER_IS_VisualAge 0
#define A_UTIL_ENABLE_COMPILER_IS_PGI 0
#define A_UTIL_ENABLE_COMPILER_IS_Cray 0
#define A_UTIL_ENABLE_COMPILER_IS_TI 0
#define A_UTIL_ENABLE_COMPILER_IS_Fujitsu 0
#define A_UTIL_ENABLE_COMPILER_IS_GHS 0
#define A_UTIL_ENABLE_COMPILER_IS_SCO 0
#define A_UTIL_ENABLE_COMPILER_IS_ARMCC 0
#define A_UTIL_ENABLE_COMPILER_IS_AppleClang 0
#define A_UTIL_ENABLE_COMPILER_IS_ARMClang 0
#define A_UTIL_ENABLE_COMPILER_IS_Clang 0
#define A_UTIL_ENABLE_COMPILER_IS_GNU 0
#define A_UTIL_ENABLE_COMPILER_IS_MSVC 0
#define A_UTIL_ENABLE_COMPILER_IS_ADSP 0
#define A_UTIL_ENABLE_COMPILER_IS_IAR 0
#define A_UTIL_ENABLE_COMPILER_IS_MIPSpro 0

#if defined(__COMO__)
#undef A_UTIL_ENABLE_COMPILER_IS_Comeau
#define A_UTIL_ENABLE_COMPILER_IS_Comeau 1

#elif defined(__INTEL_COMPILER) || defined(__ICC)
#undef A_UTIL_ENABLE_COMPILER_IS_Intel
#define A_UTIL_ENABLE_COMPILER_IS_Intel 1

#elif defined(__PATHCC__)
#undef A_UTIL_ENABLE_COMPILER_IS_PathScale
#define A_UTIL_ENABLE_COMPILER_IS_PathScale 1

#elif defined(__BORLANDC__) && defined(__CODEGEARC_VERSION__)
#undef A_UTIL_ENABLE_COMPILER_IS_Embarcadero
#define A_UTIL_ENABLE_COMPILER_IS_Embarcadero 1

#elif defined(__BORLANDC__)
#undef A_UTIL_ENABLE_COMPILER_IS_Borland
#define A_UTIL_ENABLE_COMPILER_IS_Borland 1

#elif defined(__WATCOMC__) && __WATCOMC__ < 1200
#undef A_UTIL_ENABLE_COMPILER_IS_Watcom
#define A_UTIL_ENABLE_COMPILER_IS_Watcom 1

#elif defined(__WATCOMC__)
#undef A_UTIL_ENABLE_COMPILER_IS_OpenWatcom
#define A_UTIL_ENABLE_COMPILER_IS_OpenWatcom 1

#elif defined(__SUNPRO_CC)
#undef A_UTIL_ENABLE_COMPILER_IS_SunPro
#define A_UTIL_ENABLE_COMPILER_IS_SunPro 1

#elif defined(__HP_aCC)
#undef A_UTIL_ENABLE_COMPILER_IS_HP
#define A_UTIL_ENABLE_COMPILER_IS_HP 1

#elif defined(__DECCXX)
#undef A_UTIL_ENABLE_COMPILER_IS_Compaq
#define A_UTIL_ENABLE_COMPILER_IS_Compaq 1

#elif defined(__IBMCPP__) && defined(__COMPILER_VER__)
#undef A_UTIL_ENABLE_COMPILER_IS_zOS
#define A_UTIL_ENABLE_COMPILER_IS_zOS 1

#elif defined(__ibmxl__) && defined(__clang__)
#undef A_UTIL_ENABLE_COMPILER_IS_XLClang
#define A_UTIL_ENABLE_COMPILER_IS_XLClang 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ >= 800
#undef A_UTIL_ENABLE_COMPILER_IS_XL
#define A_UTIL_ENABLE_COMPILER_IS_XL 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ < 800
#undef A_UTIL_ENABLE_COMPILER_IS_VisualAge
#define A_UTIL_ENABLE_COMPILER_IS_VisualAge 1

#elif defined(__PGI)
#undef A_UTIL_ENABLE_COMPILER_IS_PGI
#define A_UTIL_ENABLE_COMPILER_IS_PGI 1

#elif defined(_CRAYC)
#undef A_UTIL_ENABLE_COMPILER_IS_Cray
#define A_UTIL_ENABLE_COMPILER_IS_Cray 1

#elif defined(__TI_COMPILER_VERSION__)
#undef A_UTIL_ENABLE_COMPILER_IS_TI
#define A_UTIL_ENABLE_COMPILER_IS_TI 1

#elif defined(__FUJITSU) || defined(__FCC_VERSION) || defined(__fcc_version)
#undef A_UTIL_ENABLE_COMPILER_IS_Fujitsu
#define A_UTIL_ENABLE_COMPILER_IS_Fujitsu 1

#elif defined(__ghs__)
#undef A_UTIL_ENABLE_COMPILER_IS_GHS
#define A_UTIL_ENABLE_COMPILER_IS_GHS 1

#elif defined(__SCO_VERSION__)
#undef A_UTIL_ENABLE_COMPILER_IS_SCO
#define A_UTIL_ENABLE_COMPILER_IS_SCO 1

#elif defined(__ARMCC_VERSION) && !defined(__clang__)
#undef A_UTIL_ENABLE_COMPILER_IS_ARMCC
#define A_UTIL_ENABLE_COMPILER_IS_ARMCC 1

#elif defined(__clang__) && defined(__apple_build_version__)
#undef A_UTIL_ENABLE_COMPILER_IS_AppleClang
#define A_UTIL_ENABLE_COMPILER_IS_AppleClang 1

#elif defined(__clang__) && defined(__ARMCOMPILER_VERSION)
#undef A_UTIL_ENABLE_COMPILER_IS_ARMClang
#define A_UTIL_ENABLE_COMPILER_IS_ARMClang 1

#elif defined(__clang__)
#undef A_UTIL_ENABLE_COMPILER_IS_Clang
#define A_UTIL_ENABLE_COMPILER_IS_Clang 1

#elif defined(__GNUC__) || defined(__GNUG__)
#undef A_UTIL_ENABLE_COMPILER_IS_GNU
#define A_UTIL_ENABLE_COMPILER_IS_GNU 1

#elif defined(_MSC_VER)
#undef A_UTIL_ENABLE_COMPILER_IS_MSVC
#define A_UTIL_ENABLE_COMPILER_IS_MSVC 1

#elif defined(__VISUALDSPVERSION__) || defined(__ADSPBLACKFIN__) || defined(__ADSPTS__) ||         \
    defined(__ADSP21000__)
#undef A_UTIL_ENABLE_COMPILER_IS_ADSP
#define A_UTIL_ENABLE_COMPILER_IS_ADSP 1

#elif defined(__IAR_SYSTEMS_ICC__) || defined(__IAR_SYSTEMS_ICC)
#undef A_UTIL_ENABLE_COMPILER_IS_IAR
#define A_UTIL_ENABLE_COMPILER_IS_IAR 1

#endif

#if A_UTIL_ENABLE_COMPILER_IS_GNU

#if !((__GNUC__ * 100 + __GNUC_MINOR__) >= 404)
#error Unsupported compiler version
#endif

#if defined(__GNUC__)
#define A_UTIL_ENABLE_COMPILER_VERSION_MAJOR (__GNUC__)
#else
#define A_UTIL_ENABLE_COMPILER_VERSION_MAJOR (__GNUG__)
#endif
#if defined(__GNUC_MINOR__)
#define A_UTIL_ENABLE_COMPILER_VERSION_MINOR (__GNUC_MINOR__)
#endif
#if defined(__GNUC_PATCHLEVEL__)
#define A_UTIL_ENABLE_COMPILER_VERSION_PATCH (__GNUC_PATCHLEVEL__)
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 &&                                                    \
    (__cplusplus >= 201103L ||                                                                     \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define A_UTIL_ENABLE_COMPILER_CXX_NULLPTR 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_NULLPTR 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 404 &&                                                    \
    (__cplusplus >= 201103L ||                                                                     \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 404 &&                                                    \
    (__cplusplus >= 201103L ||                                                                     \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 404 &&                                                    \
    (__cplusplus >= 201103L ||                                                                     \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT 0
#endif

#elif A_UTIL_ENABLE_COMPILER_IS_MSVC

#if !(_MSC_VER >= 1600)
#error Unsupported compiler version
#endif

/* _MSC_VER = VVRR */
#define A_UTIL_ENABLE_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
#define A_UTIL_ENABLE_COMPILER_VERSION_MINOR (_MSC_VER % 100)
#if defined(_MSC_FULL_VER)
#if _MSC_VER >= 1400
/* _MSC_FULL_VER = VVRRPPPPP */
#define A_UTIL_ENABLE_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
#else
/* _MSC_FULL_VER = VVRRPPPP */
#define A_UTIL_ENABLE_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 10000)
#endif
#endif
#if defined(_MSC_BUILD)
#define A_UTIL_ENABLE_COMPILER_VERSION_TWEAK (_MSC_BUILD)
#endif

#if _MSC_VER >= 1600
#define A_UTIL_ENABLE_COMPILER_CXX_NULLPTR 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_NULLPTR 0
#endif

#if _MSC_VER >= 1600
#define A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES 0
#endif

#if _MSC_VER >= 1800
#define A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES 0
#endif

#if _MSC_VER >= 1600
#define A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT 0
#endif

#elif A_UTIL_ENABLE_COMPILER_IS_Clang

#if !(((__clang_major__ * 100) + __clang_minor__) >= 301)
#error Unsupported compiler version
#endif

#define A_UTIL_ENABLE_COMPILER_VERSION_MAJOR (__clang_major__)
#define A_UTIL_ENABLE_COMPILER_VERSION_MINOR (__clang_minor__)
#define A_UTIL_ENABLE_COMPILER_VERSION_PATCH (__clang_patchlevel__)
#if defined(_MSC_VER)
/* _MSC_VER = VVRR */
#define A_UTIL_ENABLE_SIMULATE_VERSION_MAJOR (_MSC_VER / 100)
#define A_UTIL_ENABLE_SIMULATE_VERSION_MINOR (_MSC_VER % 100)
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_nullptr)
#define A_UTIL_ENABLE_COMPILER_CXX_NULLPTR 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_NULLPTR 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_rvalue_references)
#define A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_RVALUE_REFERENCES 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_variadic_templates)
#define A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_static_assert)
#define A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT 1
#else
#define A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT 0
#endif

#else
#error Unsupported compiler
#endif

#if defined(A_UTIL_ENABLE_COMPILER_CXX_NULLPTR) && A_UTIL_ENABLE_COMPILER_CXX_NULLPTR
#define A_UTIL_ENABLE_NULLPTR nullptr
#elif A_UTIL_ENABLE_COMPILER_IS_GNU
#define A_UTIL_ENABLE_NULLPTR __null
#else
#define A_UTIL_ENABLE_NULLPTR 0
#endif

#if defined(A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT) && A_UTIL_ENABLE_COMPILER_CXX_STATIC_ASSERT
#define A_UTIL_ENABLE_STATIC_ASSERT(X) static_assert(X, #X)
#define A_UTIL_ENABLE_STATIC_ASSERT_MSG(X, MSG) static_assert(X, MSG)
#else
#define A_UTIL_ENABLE_STATIC_ASSERT_JOIN(X, Y) A_UTIL_ENABLE_STATIC_ASSERT_JOIN_IMPL(X, Y)
#define A_UTIL_ENABLE_STATIC_ASSERT_JOIN_IMPL(X, Y) X##Y
template <bool>
struct A_UTIL_ENABLEStaticAssert;
template <>
struct A_UTIL_ENABLEStaticAssert<true> {
};
#define A_UTIL_ENABLE_STATIC_ASSERT(X)                                                             \
    enum {                                                                                         \
        A_UTIL_ENABLE_STATIC_ASSERT_JOIN(A_UTIL_ENABLEStaticAssertEnum, __LINE__) =                \
            sizeof(A_UTIL_ENABLEStaticAssert<X>)                                                   \
    }
#define A_UTIL_ENABLE_STATIC_ASSERT_MSG(X, MSG)                                                    \
    enum {                                                                                         \
        A_UTIL_ENABLE_STATIC_ASSERT_JOIN(A_UTIL_ENABLEStaticAssertEnum, __LINE__) =                \
            sizeof(A_UTIL_ENABLEStaticAssert<X>)                                                   \
    }
#endif

#endif

#endif
