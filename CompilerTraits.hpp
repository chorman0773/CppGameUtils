#ifndef __CompilerTraits_hpp_2018_06_11_12_31
#define __CompilerTraits_hpp_2018_06_11_12_31

#if __cplusplus >= 201103
#define CPP_HAS_CONSTEXPR
#define CPP_HAS_VARADIAC_TEMPLATE
#define CPP_HAS_INITIALIZER_LIST
#define CPP_HAS_SMART_POINTER
#define CPP_HAS_RVALUE_REFERENCE
#define CPP_HAS_REFERENCE_QUALIFIERS
#define CPP_HAS_TEMPLATE_ALIAS
#define CPP_HAS_DEFAULT_CTOR
#define CPP_HAS_DELETE_CTOR
#define CPP_HAS_INLINE_NAMESPACE
#else
#error "Must use at least C++11 compiler"
#endif

#if __cplusplus >= 201402
#define CPP_HAS_INTEGER_SEQUENCE
#define CPP_HAS_TEMPLATE_VARIABLES
#define CPP_HAS_DECLTYPE_AUTO
#endif

#if __cplusplus >= 201703
#define CPP_HAS_VOID_T
#endif

#if __cplusplus > 201703
#endif

#if defined(__SNESOS_LIB)||defined(__ENHANCED_MACRO_SUPPORT)
#define CPP_HAS_ENHANCED_MACRO
#endif
#if defined(__SNESOS_LIB)||defined(__ENHANCED_NAMESPACE_SUPPORT)
#define CPP_HAS_ENHANCED_NAMESPACE
#endif

#if defined(__SNESOS_LIB)||defined(__UNIQUE__)
#define CPP_HAS_UNIQUE
#endif

#if defined(_WIN32)
#define CPP_HAS_GETCH
#define CPP_GETCH_HEADER <conio.h>
#define CPP_GETCH_DEPRECATED
#elif defined(__SNESOS_LIB)
#define CPP_HAS_GETCH
#define CPP_GETCH_HEADER <os/terminals.h>
#endif

#if defined(__COUNTER__)
#define CPP_HAS_COUNTER
#endif






#ifdef _MSC_VER
#if _MSC_VER < 1900
#undef CPP_HAS_INTEGER_SEQUENCE
#undef CPP_HAS_TEMPLATE_VARIABLES
#undef CPP_HAS_DECLTYPE_AUTO
#undef CPP_HAS_CONSTEXPR
#undef CPP_HAS_INLINE_NAMESPACE
#endif
#if _MSC_VER < 1800
#undef CPP_HAS_DEFAULT_CTOR
#undef CPP_HAS_DELETE_CTOR
#undef CPP_HAS_INITIALIZER_LIST
#undef CPP_HAS_REFERENCE_QUALIFIERS
#undef CPP_HAS_VARADIAC_TEMPLATE
#endif
#endif


#ifndef CPP_HAS_UNIQUE
#ifdef CPP_HAS_COUNTER
#define UNIQUE UNIQUENAME##__LINE__##__COUNTER__
#else
#define UNIQUE UNIQUENAME##__LINE__##AABBCCDD00112233
#endif
#else
#define UNIQUE __UNIQUE__
#endif

#ifndef CPP_HAS_ENHANCED_NAMESPACE
#define ENTER_NAMESPACE(name) namespace name{
#define EXIT_NAMESPACE(name) };
#ifdef CPP_HAS_INLINE_NAMESPACE
#define ENTER_UNQIUE(name) namespace{\
        inline namespace UNIQUE{\
            namespace name{
#define EXIT_UNIQUE(name) };};};
#else
#define ENTER_UNIQUE(name) namespace{\
        namespace name{
#define EXIT_UNIQUE(name) };};
#endif
#else
#define ENTER_NAMESPACE(name) #pragma namespaces enter(name)
#define EXIT_NAMESPACE(name) #pragma namespaces exit(name)
#define ENTER_UNIQUE(name) #pragma namespaces enter unqiue(name)
#define EXIT_UNIQUE(name) #pragma namespaces exit unique(name)
#endif


#endif