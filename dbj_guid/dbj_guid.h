//-------------------------------------------------------------------------------------------------------
// DBJ_GUID parsing
// Written by Alexander Bessonov
// Written by Tobias Loew
// Written by DBJ -- note: this is not DBJ_GUID generator.
//                    Also vs WIN32 this is largely compile time excersize
//                    2021 AUG 22 -- made into modern C
//                    #define DBJ_GUID_IMPLEMENTATION in exactly one place before including this
//
// Licensed under the MIT license.
//-------------------------------------------------------------------------------------------------------
#ifndef DJB_GUID_INC
#define DJB_GUID_INC

#include <stdbool.h>
#include <stdint.h>

/*
	basicaly if you need UUID + on win you include rpc.h
	in that scenario you do not need any kind of "portable" solution
	like this bellow is
	*/
typedef struct DBJ_GUID_
{
	uint32_t Data1{};
	uint16_t Data2{};
	uint16_t Data3{};
	uint8_t Data4[8]{};
} DBJ_GUID;

// we do not use exceptions, on error we return null guid
extern DBJ_GUID null_guid;
#ifdef DBJ_GUID_IMPLEMENTATION
extern DBJ_GUID null_guid = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
#endif

static inline bool is_null(const DBJ_GUID &guid_)
{
	return (&guid_ == &null_guid);
}

// equality operators can be used at compile time too
static inline bool equal(const DBJ_GUID &left_, const DBJ_GUID &right_)
{
	return (left_.Data1 == right_.Data1) &&
		   (left_.Data2 == right_.Data2) &&
		   (left_.Data3 == right_.Data3) &&
		   (left_.Data4[0] == right_.Data4[0]) &&
		   (left_.Data4[1] == right_.Data4[1]) &&
		   (left_.Data4[2] == right_.Data4[2]) &&
		   (left_.Data4[3] == right_.Data4[3]) &&
		   (left_.Data4[4] == right_.Data4[4]) &&
		   (left_.Data4[5] == right_.Data4[5]) &&
		   (left_.Data4[6] == right_.Data4[6]) &&
		   (left_.Data4[7] == right_.Data4[7]);
}

// static inline bool operator == (const DBJ_GUID& left_, const DBJ_GUID& right_) { return equal(left_, right_); }
// static inline bool operator != (const DBJ_GUID& left_, const DBJ_GUID& right_) { return !equal(left_, right_); }

// parse char array into DBJ_GUID
DBJ_GUID make_guid(const unsigned N, const char str[static N]);

#ifdef DBJ_GUID_IMPLEMENTATION

const size_t short_guid_form_length = 36; // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
const size_t long_guid_form_length = 38;  // {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}

//
uint8_t parse_hex_digit(const char c)
{
	return ('0' <= c && c <= '9')
			   ? c - '0'
		   : ('a' <= c && c <= 'f')
			   ? 10 + c - 'a'
		   : ('A' <= c && c <= 'F')
			   ? 10 + c - 'A'
			   : (perror(__FILE__ " -- invalid character in DBJ_GUID"), exit(0), '?');
}

uint8_t parse_hex_uint8_t(const char *ptr)
{
	return (parse_hex_digit(ptr[0]) << 4) + parse_hex_digit(ptr[1]);
}

uint16_t parse_hex_uint16_t(const char *ptr)
{
	return (parse_hex_uint8_t(ptr) << 8) + parse_hex_uint8_t(ptr + 2);
}

uint32_t parse_hex_uint32_t(const char *ptr)
{
	return (parse_hex_uint16_t(ptr) << 16) + parse_hex_uint16_t(ptr + 4);
}

DBJ_GUID parse_guid(const char *begin)
{
	return DBJ_GUID{
		parse_hex_uint32_t(begin),
		parse_hex_uint16_t(begin + 8 + 1),
		parse_hex_uint16_t(begin + 8 + 1 + 4 + 1),
		{parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1),
		 parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2),
		 parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1),
		 parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2),
		 parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2),
		 parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2 + 2),
		 parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2 + 2 + 2),
		 parse_hex_uint8_t(begin + 8 + 1 + 4 + 1 + 4 + 1 + 2 + 2 + 1 + 2 + 2 + 2 + 2 + 2)}

	};
}

// on wrong input we do not throw exception
// we set errno to EINVAL and return dbj::null_guid
DBJ_GUID make_guid_helper(const char *str, size_t N)
{
	using namespace details;

	if (!(N == long_guid_form_length || N == short_guid_form_length))
	{
		perror(__FILE__ "\n\nString DBJ_GUID of the form {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} or XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX is expected");
		errno = EINVAL;
		return null_guid;
	}

	if (N == long_guid_form_length && (str[0] != '{' || str[long_guid_form_length - 1] != '}'))
	{
		perror(__FILE__ "\n\nMissing opening or closing brace");
		errno = EINVAL;
		return null_guid;
	}

	return parse_guid(str + (N == long_guid_form_length ? 1 : 0));
}

DBJ_GUID make_guid(const unsigned N, const char str[static N])
{
	return make_guid_helper(str, N - 1);
}

#ifdef DBJ_GUID_IMPLEMENTATION

// mandatory portable helpers

// we have here portable UUID generator but I very much doubt
// "enterprise" developers will be allowed to use it
#include "uuid4.h"

	/*
	little non win portable uuid generator
	note: works for windows too
	*/
	static inline DBJ_GUID uuid4_guid() noexcept
	{
		char string_uuid_[UUID4_LEN]{};
		uuid4_generate(string_uuid_);

		DBJ_GUID win_rpc_guid_ =
			make_guid_helper(
				(const char *)string_uuid_,
				strlen((const char *)string_uuid_));

		return win_rpc_guid_;
	}


// note: rpc.h also includes windows.h so we shall not #include rpc.h
// Note: if this is used in the code which does include rpc.h
// simply comment it out

#if defined(_WIN32)

#ifndef __RPC_H__

extern "C"
{

#define RPC_S_OK 0

	typedef struct _GUID
	{
		unsigned long Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char Data4[8];
	} DBJ_GUID;

	typedef _GUID UUID;
	typedef long RPC_STATUS;
	typedef unsigned const char *RPC_CSTR;

	__declspec(dllimport) long __stdcall UuidCreate(UUID *);

	__declspec(dllimport) long __stdcall UuidToStringA(
		const UUID *, unsigned const char **);

	__declspec(dllimport) long __stdcall RpcStringFreeA(unsigned const char **);

} // "C"

#endif // ! __RPC_H__

// for RPC usage in WIN32 this lib manual inclusion is mandatory
#pragma comment(lib, "Rpcrt4.lib")
#include <crtdbg.h>
#endif // _WIN32

// note: you know that anonymouys namespace leaves it to the linker what to do with
// inlines inside it, which almost always in that scenario are turned into statics
namespace
{
#ifdef _WIN32
	/*
	windows uses DCE RPC
	*/
	inline dbj::DBJ_GUID win_rpc_guid() noexcept
	{
		UUID legacy_uuid_;
		RPC_STATUS rpc_rezult_ = UuidCreate(&legacy_uuid_);
		_ASSERTE(rpc_rezult_ == RPC_S_OK);

		// Note: DCE strings are made of unsigned chars
		RPC_CSTR string_uuid_;

		rpc_rezult_ = UuidToStringA(
			&legacy_uuid_,
			&string_uuid_);
		_ASSERTE(rpc_rezult_ == RPC_S_OK);

		dbj::DBJ_GUID win_rpc_guid_ = dbj::details::make_guid_helper((const char *)string_uuid_, strlen((const char *)string_uuid_));

		rpc_rezult_ = RpcStringFreeA(
			&string_uuid_);
		_ASSERTE(rpc_rezult_ == RPC_S_OK);

		return win_rpc_guid_;
	}

#undef RPC_S_OK
#endif // _WIN32

	inline void test_dbj_guid() noexcept
	{

		using namespace dbj::literals;

		// compile time
		dbj::DBJ_GUID guid_1 = "{FE297330-BAA5-407F-BB47-F78752D2C209}"_guid;
		dbj::DBJ_GUID guid_2 = "{FE297330-BAA5-407F-BB47-F78752D2C209}"_guid;

		static_assert(guid_1 == guid_2);

		static_assert(!is_null(guid_1));

		// runtime DBJ_GUID makers
		// this is WIN32 function
#ifdef _WIN32
		dbj::DBJ_GUID guid_3 = win_rpc_guid();
		assert(guid_1 != guid_3);
#endif // _WIN32

		dbj::DBJ_GUID guid_4 = uuid4_guid(); // internaly OS agnostic
		assert(guid_1 != guid_4);
	}

} // nspace

#endif // !DJB_GUID_INC
