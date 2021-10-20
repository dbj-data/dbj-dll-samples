#pragma once

// DBJ: nicked from MSSTL yvals_core.h
//---------------------------------------------------------------------------------
// warning: constexpr if is a C++17 extension [-Wc++17-extensions]
// warning: explicit(bool) is a C++20 extension [-Wc++20-extensions]
// warning: ignoring __declspec(allocator) because the function return type '%s' is not a pointer or reference type
//     [-Wignored-attributes]
// warning: user-defined literal suffixes not starting with '_' are reserved [-Wuser-defined-literals]
// warning: unknown pragma ignored [-Wunknown-pragmas]
#ifndef DBJ_DISABLE_CLANG_WARNINGS
#ifdef __clang__
// clang-format off
#define DBJ_DISABLE_CLANG_WARNINGS                                 \
	_Pragma("clang diagnostic push")                                \
	_Pragma("clang diagnostic ignored \"-Wc++17-extensions\"")      \
	_Pragma("clang diagnostic ignored \"-Wc++20-extensions\"")      \
	_Pragma("clang diagnostic ignored \"-Wignored-attributes\"")    \
	_Pragma("clang diagnostic ignored \"-Wuser-defined-literals\"") \
	_Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
// clang-format on
#else // __clang__
#define DBJ_DISABLE_CLANG_WARNINGS
#endif // __clang__
#endif // DBJ_DISABLE_CLANG_WARNINGS

#ifndef DBJ_RESTORE_CLANG_WARNINGS
#ifdef __clang__
#define DBJ_RESTORE_CLANG_WARNINGS _Pragma("clang diagnostic pop")
#else // __clang__
#define DBJ_RESTORE_CLANG_WARNINGS
#endif // __clang__
#endif // DBJ_RESTORE_CLANG_WARNINGS

// clang-format off
#ifndef DBJ_DISABLE_4996
#ifdef __clang__
#define DBJ_DISABLE_4996 \
	_Pragma("clang diagnostic push")    \
	_Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#else //!  __clang__
#define DBJ_DISABLE_4996 \
	__pragma(warning(push))             \
	__pragma(warning(disable : 4996)) // was declared deprecated
#endif // !  __clang__
#endif // DBJ_DISABLE_4996
// clang-format on

#ifndef DBJ_RESTORE_4996
#ifdef __clang__
#define DBJ_RESTORE_4996 _Pragma("clang diagnostic pop")
#else // __clang__
#define DBJ_RESTORE_4996 __pragma(warning(pop))
#endif // __clang__
#endif // DBJ_RESTORE_4996

/*
(c) 2021 by dbj@dbj.org

in case you had enough of this:

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

why not trying the MSVC warning(supress: XXXX) pragma?
but it has to be on a line adjacent before the line you want to silence the cl.exe about

using this macro oyou do not have to put it on separate snf previous line
sometimes one must put it immediately before . Example


if ( SUPR4996 sscanf(line, "%[^=] = \"%[^\"]\"", key, value) == 2
	|| SUPR4996 sscanf(line, "%[^=] = '%[^\']'", key, value) == 2)
{
}

only that will silence the cl.exe compiler

*/
#ifdef _MSC_VER
#define SUPR4996  __pragma( warning(suppress : 4996) )
#else
// this is perhaps bad idea as there will not be DBJ_RESTORE_4996 to close it
// but, does this matter?
#error Just CL has warning suppress, use DBJ_DISABLE_4996 instead, once per file
#endif