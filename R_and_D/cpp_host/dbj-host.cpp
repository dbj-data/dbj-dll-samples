
#include "../dbj_capi/cdebug.h"
#include "../dbj-component.h"
#include <dbj_capi/dbj-string.h>

// each component has one struct that describes the component interface
#include "../A/component-a.h"
#include "../B/component-b.h"

#define DBJ_DLL_CALL_INCLUDES_WINDOWS
#include "../dll_loader/dbj_dll_call.hpp"

#include <assert.h>
#include <stdio.h>
/* ----------------------------------------------------------------------------------------------- */
static void show_component_info(const char *const component_dll_name)
{
  using ::dbj::win::dll_call;

  dll_call<DBJ_COMPONENT_SEMVER_FP>(component_dll_name,
                                    DBJCS_SEMVER_NAME,
                                    // return type can not be void
                                    [&](DBJ_COMPONENT_SEMVER_FP get_version)
                                    {
                                      struct dbj_component_version_ info_ = get_version();
                                      DBG_PRINT("\ncomponent dll: %s, version info", component_dll_name);
                                      DBG_PRINT("\nMajor: %d, minor: %d, patch: %d", info_.major, info_.minor, info_.patch);
                                      DBG_PRINT("\nDescription: %s", info_.description);
                                    });
}
/* ----------------------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
  // BIG FAT WARNING: dll_call is very convenient, but after it has finished
  // the dll is unloaded!
  using ::dbj::win::dll_call;
  {
    show_component_info(COMPONENT_A_DLL_NAME);

    // here we are just demoing dll_call() returning the type and value as expected from the
    // fumction signature and its function pointer used
    dll_call<component_a_factory_fp>(COMPONENT_A_DLL_NAME, // load this dll
                                     DBJCS_FACTORYNAME,    // get to this function
                                                           // implementation_ is never null as lambda is not called
                                                           // if something goes wrong
                                                           // also there is a log file named <app base name>.exe.log
                                                           // and the dll_call returns the default value of whatever
                                                           // type this lambda might return
                                     [](component_a_factory_fp factory)
                                     {
                                       struct component_a *dbj_component = factory();
                                       DBG_PRINT("fty2 : %d", dbj_component->get42(dbj_component));
                                     });
  }
  {
    show_component_info(DBJ_SHMEM_DLL_NAME);

    dll_call<component_b_factory_fp>(DBJ_SHMEM_DLL_NAME, // load this dll
                                     DBJCS_FACTORYNAME,  // get to this function
                                                         // implementation_ is never null as lambda is not called
                                                         // if something goes wrong
                                                         // also there is a log file named <app base name>.exe.log
                                                         // and the dll_call returns the default value of whatever
                                                         // type this lambda might return
                                     [&](component_b_factory_fp factory)
                                     {
                                       struct component_shmem *the_component = factory();
                                       dbj_string_1024 connstr_ = the_component->connection_string(the_component);
                                       DBG_PRINT("\nconnection string: %s\n", connstr_.data);
                                     });
  }
  return 0;
}