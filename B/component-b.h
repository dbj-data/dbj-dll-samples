#ifndef COMPONENT_B_INC_
#define COMPONENT_B_INC_

#include "../dbj-component-string.h"

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
struct component_b
{
    HANDLE (*create)(struct component_b * /* self_ */, dbj_component_string_64 /* key_ */, unsigned /*size*/ );
    HANDLE (*delete)(struct component_b * /* self_ */, HANDLE /* handle_ */ );

    bool (*set_value)(struct component_b * /* self_ */, HANDLE /* handle_ */, void  * /* value */ );
    bool (*get_value)(struct component_b * /* self_ */, HANDLE /* handle_ */, void ** /* value */ );

    // dbj_component_string_1024 (*create)(struct component_b *);
};

// we do not actually need this declaration in this header
//
// struct component_b *dbj_component_factory(void);
//
// dbj_component_factory is the only of the three functions
// from the def file whose function pointer declaration is required
// because it is unique per each component
//
// FP of the dbj_component_factory
// for this component
typedef struct component_b *(*component_b_factory_fp)(void);

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus
#endif // COMPONENT_B_INC_