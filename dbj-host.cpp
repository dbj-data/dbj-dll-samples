// each component has one struct the describe the component interface
#include "A\component-a.h"
#include "B\component-b.h"

#define COMPONENT_A_DLL_NAME "component-a.dll"
#define COMPONENT_B_DLL_NAME "component-b.dll"

// this is exported in a def file for each component
// and is always the same
// regardless of the return type or arguments
#define DBJ_COMPONENT_FACTORYNAME "dbj_component_get_implementation"

#define DBJ_DLL_CALL_INCLUDES_WINDOWS
#include "dll_loader/dbj_dll_call.h"

#include <assert.h>

int main(int argc, char **argv)
{
    using ::dbj::win::dll_call;
{
  // FP of the dbj_component_get_implementation
  // is different for each component 
  typedef struct component_a *(*FP)(void);

  int fty2 =
      dll_call<FP>(COMPONENT_A_DLL_NAME,    // load this dll
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

  assert(fty2 == 42);
}
{
  // FP of the dbj_component_get_implementation
  // is different for each component 
  typedef struct component_b *(*FP)(void);

  char buffy[1024] = { 0 };

  const char * connstr_ =
      dll_call<FP>(COMPONENT_B_DLL_NAME,    // load this dll
                   DBJ_COMPONENT_FACTORYNAME, // get to this function
                                              // implementation_ is never null as lambda is not called
                                              // if something goes wrong
                                              // also there is a log file named <app base name>.exe.log
                                              // and the dll_call returns the default value of whatever
                                              // type this lambda might return
                   [&](FP implementation_)
                   {
                     struct component_b *dbj_component = implementation_();

                     dbj_component->connection_string(dbj_component, 102, buffy);

                     return buffy ;
                   });

  assert(connstr_);
}
  return 0;
}