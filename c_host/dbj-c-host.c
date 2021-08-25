#define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
#define DBJCAPI_DLL_CALLER_IMPLEMENTATION

#include "memory_info.h"

#include "../dbj_capi/cdebug.h"
#include "../dbj-component.h"
#include "../dbj-component-string.h"

// each component has one struct that describes the component interface
#include "../A/component-a.h"
#include "../B/component-b.h"

#define COMPONENT_A_DLL_NAME "component-a.dll"
#define COMPONENT_B_DLL_NAME "component-b.dll"

#include "../dll_loader/dbj_dll_call.h"

#include <assert.h>
#include <stdio.h>
/* ----------------------------------------------------------------------------------------------- */
// FP of the dbj_component_factory
// is different for each component
static inline void get_version_cb(DBJ_COMPONENT_SEMVER_FP get_version)
{
  dbj_component_version info_ = get_version();
  DBG_PRINT("\ncomponent dll version info");
  DBG_PRINT("\nMajor: %d, minor: %d, patch: %d", info_.major, info_.minor, info_.patch);
  DBG_PRINT("\nDescription: %s", info_.description);
}
/* ----------------------------------------------------------------------------------------------- */
static void show_component_info(const char component_dll_name[static 1])
{
  DBJCAPI_DLL_CALL(component_dll_name, DBJ_COMPONENT_SEMVER_NAME, DBJ_COMPONENT_SEMVER_FP, get_version_cb);
}
/* ----------------------------------------------------------------------------------------------- */
// FP of the dbj_component_factory
// is different for each component
typedef struct component_b *(*BFP)(void);
static inline void component_b_factory(BFP factory)
{
  struct component_b *implementation = factory();
  dbj_component_string_1024 connstr_ =
      implementation->connection_string(implementation);
  DBG_PRINT("\nconnection string: %s\n", connstr_.data);
}
/* ----------------------------------------------------------------------------------------------- */
// FP of the interface implementation factory
// dbj_component_factory()
// is different for each component
// because it returns pointer to the particular interface
typedef struct component_a *(*AFP)(void);
static inline void component_a_factory(AFP factory)
{
  struct component_a *implementation = factory();
  // intrface struct has a method called "function"
  // with the following signature
  // int function(component_a *) ;
  int fty2 = implementation->function(implementation);
  DBG_PRINT("fty2 : %d", fty2);
}
/* ----------------------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
  DBJ_DLL_CALL_LOG("Starting: %s", argv[0]);
  dbjcapi_memory_info(stderr);

  show_component_info(COMPONENT_A_DLL_NAME);
  DBJCAPI_DLL_CALL(COMPONENT_A_DLL_NAME, DBJ_COMPONENT_FACTORYNAME, AFP, component_a_factory);
  dbjcapi_memory_info(stderr);
  show_component_info(COMPONENT_B_DLL_NAME);
  DBJCAPI_DLL_CALL(COMPONENT_B_DLL_NAME, DBJ_COMPONENT_FACTORYNAME, BFP, component_b_factory);
  dbjcapi_memory_info(stderr);
  DBJ_DLL_CALL_LOG("Ending: %s", argv[0]);

  return 0;
}