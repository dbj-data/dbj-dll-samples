#define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
#define DBJCS_DLL_CALLER_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION

#include "memory_info.h"

#include <dbj_capi/cdebug.h>
#include "../dbj-component.h"
#include <dbj_capi/dbj-string.h>
#include "../dbj-component-loader.h"

// components are partitioned in their own projects
#include "../A/component-a.h"
#include "../dbj-shmem/dbj-shmem.h"
#include "../dbj-vector/dbj-vector.h"
#include "../dbj-syserrmsg/dbj-syserrmsg.h"

#include <assert.h>
#include <stdio.h>
/* ----------------------------------------------------------------------------------------------- */
// FP of the interface_factory
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
static inline void shmem_component_user(component_shmem_factory_fp factory)
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
  // #pragma clang diagnostic push
  // #pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
  DBJ_VERIFY(*ptr == 42);
  // #pragma clang diagnostic pop
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
// vector of equaly sized items
// this is a callback, after its done DLL is unloaded
static inline void dbj_vector_component_user(dbj_vector_component_fp factory)
{
  struct dbj_vector_component *imp = factory();

  // item_size, not vector size is the first argument
  dbj_vector_t *vec = imp->create(_countof("A"), 2);
  DBJ_VERIFY(imp->size(vec) == 0); // nothing has been pushed yet
  DBJ_VERIFY(imp->capacity(vec) == 2);
  // nothing is stopping you to push whatever you want, there are no checks on its size
  DBJ_VERIFY(DBJ_VCTR_IS_SUCCESS(imp->push(vec, "A")));
  DBJ_VERIFY(DBJ_VCTR_IS_SUCCESS(imp->push(vec, "B")));
  DBJ_VERIFY(DBJ_VCTR_IS_SUCCESS(imp->push(vec, "C")));
  DBJ_VERIFY(imp->size(vec) == 3);

  DBJ_VERIFY(!strcmp("A", imp->at(vec, 0)));
  DBJ_VERIFY(!strcmp("B", imp->at(vec, 1)));
  DBJ_VERIFY(!strcmp("C", imp->at(vec, 2)));

  imp->clear(vec);
}

static inline void syserrmsg_component_user(component_syserrmsg_factory_fp factory)
{
  struct component_syserrmsg *imp = factory();
  imp->also_to_stderr(true);
  dbj_string_512 msg_;
  // probe the existence of messages [0 .. 65535)
  for (int k = 2900; k < 3100; ++k)
  {
    msg_ = imp->error_message(imp, k);
    if (DBJ_STRING_IS_EMPTY(msg_))
      continue;
    DBG_PRINT("Error[%6d] : %s", k, msg_.data);
  }
}
/* ----------------------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
  DBJCS_LOADER_LOG("Starting: %s", argv[0]);
  dbjcapi_memory_info(stderr);

  show_component_info(DBJ_SYSERRMSG_DLL_NAME);
  DBJCS_CALL(DBJ_SYSERRMSG_DLL_NAME, component_syserrmsg_factory_fp, syserrmsg_component_user);

  show_component_info(COMPONENT_FILENAME_DBJ_VECTOR);
  DBJCS_CALL(COMPONENT_FILENAME_DBJ_VECTOR, dbj_vector_component_fp, dbj_vector_component_user);

  show_component_info(COMPONENT_A_DLL_NAME);
  DBJCS_CALL(COMPONENT_A_DLL_NAME, component_a_factory_fp, component_a_user);

  show_component_info(DBJ_SHMEM_DLL_NAME);
  DBJCS_CALL(DBJ_SHMEM_DLL_NAME, component_shmem_factory_fp, shmem_component_user);
  // hit the component cache
  DBJCS_CALL(DBJ_SHMEM_DLL_NAME, component_shmem_factory_fp, shmem_component_user);
  DBJCS_CALL(DBJ_SHMEM_DLL_NAME, component_shmem_factory_fp, shmem_component_user);

  DBJCS_LOADER_LOG("Ending: %s", argv[0]);
  dbjcapi_memory_info(stderr);

  return 0;
}