
#include "../dbj_capi/cdebug.h"
#include "../dbj-component.h"
#include "../dbj-component-string.h"

// each component has one struct that describes the component interface
#include "../A/component-a.h"
#include "../B/component-b.h"

#define COMPONENT_A_DLL_NAME "component-a.dll"
#define COMPONENT_B_DLL_NAME "component-b.dll"

#define DBJ_DLL_CALL_INCLUDES_WINDOWS
#include "../dll_loader/dbj_dll_call.hpp"

#include <assert.h>
#include <stdio.h>
/* ----------------------------------------------------------------------------------------------- */
static void show_component_info(const char *const component_dll_name)
{
  using ::dbj::win::dll_call;

  dll_call<DBJ_COMPONENT_SEMVER_FP>(component_dll_name,
                                    DBJ_COMPONENT_SEMVER_NAME,
                                    // return type can not be void
                                    [&](DBJ_COMPONENT_SEMVER_FP get_version)
                                    {
                                      dbj_component_version info_ = get_version();
                                      DBG_PRINT("\ncomponent dll: %s, version info", component_dll_name);
                                      DBG_PRINT("\nMajor: %d, minor: %d, patch: %d", info_.major, info_.minor, info_.patch);
                                      DBG_PRINT("\nDescription: %s", info_.description);
                                      return true;
                                    });
}
/* ----------------------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
  // BIG FAT WARNING: dll_call is very convenient, but after it has finished
  // the dll is unloaded!
  using ::dbj::win::dll_call;
  {
    // FP of the dbj_component_get_implementation
    // is different for each component
    typedef struct component_a *(*FP)(void);

     show_component_info(COMPONENT_A_DLL_NAME);

    // here we are just demoing dll_call() returning the type and value as expected from the
    // fumction signature and its function pointer used
    int fty2 =
        dll_call<FP>(COMPONENT_A_DLL_NAME,      // load this dll
                     DBJ_COMPONENT_FACTORYNAME, // get to this function
                                                // implementation_ is never null as lambda is not called
                                                // if something goes wrong
                                                // also there is a log file named <app base name>.exe.log
                                                // and the dll_call returns the default value of whatever
                                                // type this lambda might return
                     [](FP implementation_)
                     {
                       struct component_a *dbj_component = implementation_();
                       return dbj_component->function(dbj_component);
                     });

    DBG_PRINT("fty2 : %d", fty2);
  }
  {
      show_component_info(COMPONENT_B_DLL_NAME);
    // FP of the dbj_component_get_implementation
    // is different for each component
    typedef struct component_b *(*FP)(void);
    // again we could print the connstr_ from inside the lambda not outside
    dbj_component_string_1024 connstr_ =
        dll_call<FP>(COMPONENT_B_DLL_NAME,      // load this dll
                     DBJ_COMPONENT_FACTORYNAME, // get to this function
                                                // implementation_ is never null as lambda is not called
                                                // if something goes wrong
                                                // also there is a log file named <app base name>.exe.log
                                                // and the dll_call returns the default value of whatever
                                                // type this lambda might return
                     [&](FP implementation_)
                     {
                       struct component_b *the_component = implementation_();
                       return the_component->connection_string(the_component);
                     });

    DBG_PRINT("\nconnection string: %s\n", connstr_.data);
  }
  return 0;
}