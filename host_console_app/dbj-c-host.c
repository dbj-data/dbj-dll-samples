#define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
#define DBJCS_DLL_CALLER_IMPLEMENTATION

#include "memory_info.h"

#include <dbj_capi/cdebug.h>
#include "../dbj-component.h"
#include "../dbj-string.h"
#include "../dbj-component-loader.h"

// components are partitioned in their own projects
#include "../A/component-a.h"
#include "../dbj-shmem/dbj-shmem.h"

#include <assert.h>
#include <stdio.h>
/* ----------------------------------------------------------------------------------------------- */
// FP of the dbj_component_factory
// is different for each component
static inline void get_version_cb(DBJ_COMPONENT_SEMVER_FP get_version)
{
  struct dbj_component_version_ info_ = get_version();
  DBG_PRINT("\ncomponent dll version info");
  DBG_PRINT("\nMajor: %d, minor: %d, patch: %d", info_.major, info_.minor, info_.patch);
  DBG_PRINT("\nDescription: %s", info_.description);
}
/* ----------------------------------------------------------------------------------------------- */
static void show_component_info(const char component_dll_name[static 1])
{
  DBJCS_ANY_CALL(component_dll_name, DBJCS_SEMVER_NAME, DBJ_COMPONENT_SEMVER_FP, get_version_cb);
}
/* ----------------------------------------------------------------------------------------------- */
// this is a callback, after its done DLL is unloaded
static inline void component_b_user(component_b_factory_fp factory)
{
  struct component_shmem *implementation = factory();
  dbj_shmem_key_type key;
  DBJ_STRING_ASSIGN(key, "key_one");

  DBJ_VERIFY(implementation->create(implementation, key, sizeof(int)));
  int fty2 = 42;
  DBJ_VERIFY(implementation->set_value(implementation, key, sizeof(int), &fty2));
  int retrieved = 0;
  int *ptr = &retrieved;
  DBJ_VERIFY(implementation->get_value(implementation, key, sizeof(int), (void **)&ptr));
  // code is ok clang is wrong ;) ... so
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
  DBJ_VERIFY(*ptr == 42);
#pragma clang diagnostic pop
  DBJ_VERIFY(implementation->delete (implementation, key));
}
/* ----------------------------------------------------------------------------------------------- */
// this is a callback, after its done DLL is unloaded
static inline void component_a_user(component_a_factory_fp factory)
{
  struct component_a *implementation = factory();
  // intrface struct has a method called "get42"
  // with the following signature
  // int get42(component_a *) ;
  int fty2 = implementation->get42(implementation);
  DBG_PRINT("\nComponent A\nfty2 : %d\n", fty2);
  dbj_string_1024 connstr_ =
      implementation->connection_string(implementation);
  DBG_PRINT("\nconnection string: %s\n", connstr_.data);
}
/* ----------------------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
  DBJCS_LOADER_LOG("Starting: %s", argv[0]);
  dbjcapi_memory_info(stderr);

  show_component_info(COMPONENT_A_DLL_NAME);
  DBJCS_FACTORY_CALL(COMPONENT_A_DLL_NAME, component_a_factory_fp, component_a_user);

  show_component_info(COMPONENT_B_DLL_NAME);
  DBJCS_FACTORY_CALL(COMPONENT_B_DLL_NAME, component_b_factory_fp, component_b_user);

  DBJCS_LOADER_LOG("Ending: %s", argv[0]);
  dbjcapi_memory_info(stderr);

  return 0;
}