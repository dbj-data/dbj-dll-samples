#ifndef COMPONENT_B_INC_
#define COMPONENT_B_INC_

#include <dbj_capi/dbj-string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//  make sure build creates this dll for this component
#define DBJ_SHMEM_DLL_NAME "dbj-shmem.dll"

    typedef dbj_string_32 dbj_shmem_key_type;

    // proces wide SHMEM implemented in a dbj component
    struct component_shmem
    {
        bool (*create)(struct component_shmem * /* self_ */, dbj_shmem_key_type /* key_ */, unsigned /*size*/);
        bool (*delete)(struct component_shmem * /* self_ */, dbj_shmem_key_type /* key_ */);

        bool (*set_value)(struct component_shmem * /* self_ */, dbj_shmem_key_type /* key_ */, unsigned /* size */, void * /* value */);
        bool (*get_value)(struct component_shmem * /* self_ */, dbj_shmem_key_type /* key_ */, unsigned /* size */, void ** /* value */);
    };

    //
    // FP of the dbj_component_factory
    // for this component
    typedef struct component_shmem *(*component_b_factory_fp)(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif // COMPONENT_B_INC_