// (c) 2021 by dbj@dbj.org https://dbj.org/license_dbj
//
// #define DBJ_RPC_UUID_IMP before to have an implementation
//
#ifndef __clang__
#error __FILE__ " No time for MSVC shenanigans ..."
#endif // __clang__

#ifndef _WIN32
#error This is WIN32 code only
#endif


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef __RPC_H__
// for RPC usage in WIN32 this lib manual inclusion is mandatory
#pragma comment(lib, "Rpcrt4.lib")

    // note: rpc.h also includes windows.h so we shall not #include rpc.h
    // we will forward declare just what we need
    // Note: if this is used in the code which does include rpc.h
    // simply comment this out? TBC

#define RPC_S_OK 0

    typedef struct _GUID
    {
        unsigned long Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char Data4[8];
    } UUID;

    // typedef _GUID UUID;
    typedef long RPC_STATUS;
    typedef unsigned const char *RPC_CSTR;
#endif // ! __RPC_H__

 UUID dbj_rpc_guid(void);
 void dbj_rpc_guid_to_string(RPC_CSTR , const UUID *);
 void dbj_rpc_free_string(RPC_CSTR );

#ifdef DBJ_RPC_UUID_IMP
#ifndef __RPC_H__
    __declspec(dllimport) long __stdcall UuidCreate(UUID *);

    __declspec(dllimport) long __stdcall UuidToStringA(
        const UUID *, unsigned const char **);

    __declspec(dllimport) long __stdcall RpcStringFreeA(unsigned const char **);

#endif // ! __RPC_H__

    /*
	windows uses DCE RPC
	*/
    UUID dbj_rpc_guid(void)
    {
        UUID legacy_uuid_;
        RPC_STATUS rpc_rezult_ = UuidCreate(&legacy_uuid_);
        _ASSERTE(rpc_rezult_ == RPC_S_OK);

        return legacy_uuid_;
    }
    // Note: DCE strings are made of unsigned chars
     void dbj_rpc_guid_to_string(RPC_CSTR string_uuid_, const UUID *legacy_uuid_)
    {
        RPC_CSTR string_uuid_;

        RPC_STATUS rpc_rezult_ = UuidToStringA(
            legacy_uuid_,
            &string_uuid_);
        _ASSERTE(rpc_rezult_ == RPC_S_OK);
    }

    void dbj_rpc_free_string(RPC_CSTR string_uuid_)
    {
        RPC_STATUS rpc_rezult_ = RpcStringFreeA(&string_uuid_);
        _ASSERTE(rpc_rezult_ == RPC_S_OK);
        string_uuid_ = 0;
    }
#endif // DBJ_RPC_UUID_IMP

#ifdef DBJ_RPC_TESTING
    static inline void dbj_rpc_uuid_test(void)
    {
        UUID guid_3 = dbj_rpc_guid();

        RPC_CSTR string_uuid_ = 0;
        dbj_rpc_guid_to_string(string_uuid_,&guid_3) ;

        dbj_rpc_free_string(string_uuid_);
    }
#endif // DBJ_RPC_TESTING

#undef RPC_S_OK

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
