#ifndef COMPONENT_B_INC_
#define COMPONENT_B_INC_

#include "../dbj-string.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef HANDLE
typedef void * HANDLE;
#endif

//  make sure build creates this dll for this component
#define COMPONENT_B_DLL_NAME "component-b.dll"

///////////////////////////////////////////////////////////////////////////////////
// proces wide SHMEM implemented in a DLL
struct component_shmem
{
    bool (*create)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */, unsigned /*size*/ );
    bool (*delete)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */ );

    bool (*set_value)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */, unsigned /* size */, void  * /* value */ );
    bool (*get_value)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */, unsigned /* size */, void ** /* value */ );
};

//
// FP of the dbj_component_factory
// for this component
typedef struct component_shmem *(*component_b_factory_fp)(void);

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus
#endif // COMPONENT_B_INC_