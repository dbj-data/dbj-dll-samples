#ifndef DBJ_SYSERRMSG_INC_
#define DBJ_SYSERRMSG_INC_

#include <dbj_capi/dbj-string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//  make sure build creates this dll for this component
#define DBJ_SYSERRMSG_DLL_NAME "dbj-syserrmsg.dll"

    // interface is a struct
    struct component_syserrmsg
    {
        // for next error_message() call
        // on true output to stderr too
        // stays untill called with false
        // default is false
        void (*also_to_stderr)(bool);
        dbj_string_512 (*error_message)(struct component_syserrmsg * /* self_ */, const unsigned /* result of GetError() */);
    };

    //
    // interface implementation for this component
    typedef struct component_syserrmsg *(*component_syserrmsg_factory_fp)(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // DBJ_SYSERRMSG_INC_