#include "dbj-component.h"
#define DBJ_DLL_CALL_INCLUDES_WINDOWS
#include "dll_loader/dbj_dll_call.h"

int main(int argc, char **argv)
{
  // FP of the dbj_component_get_implementation
  typedef struct specimen_ *(*FP)(void);

  using ::dbj::win::dll_call;
  // implementation_ is never null as lambda is not called
  // if something goes wrong there is a log file
  // <app base name>.exe.log
  // and the dll_call returns the default value of whatever
  // type this lambda might return
  int fty2 =
      dll_call<FP>("dbj-component.dll",                // load this dll
                   "dbj_component_get_implementation", // get to this function
                   [](FP implementation_)              // use it
                   {
                     struct specimen_ *dbj_component = implementation_();
                     return dbj_component->function(dbj_component);
                   });

  assert(fty2 == 42);

  return 0;
}