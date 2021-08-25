
![unsplash](unsplash/manuel.jpg)

# DBJ COMPONENT SYSTEM&trade; (DBJCS)

Why are today's OS-es component systems so complex, and difficult to use? Behind this very pompous name is attempt to architect and implement the simplest possible component system.

Using only modern C and Windows DLL. 

If some "feature" is missing we can always pull the card of simplicity as an excuse. Let us see how far can we get.

## How is this working?

- one components is one DLL
- one component has one interface has one implementation
- All dbj-components (DLLs) have the same def file. 
  - This is it:
```
EXPORTS
dbj_component_can_unload_now      PRIVATE
dbj_component_factory             PRIVATE
dbj_component_version             PRIVATE
```
- each component has to have two string literals
  1. The component dll file name
```cpp
// component_a.h
#define COMPONENT_A_DLL_NAME "component_a.dll"
```
  2. The component factory function name
     1. that  is always that same: `"dbj_component_factory"`
- that is enough information to runtime load the dll (aka component) and get to the factory function
  - not execute it yet
- factory function returns a single pointer to the component interface 
```cpp
// from component_a.h
// factory function declaration is not required
// struct component_a * dbj_component_factory(void);
``` 
  - that interface is a struct and is the only thing required to de declared in the component header for users to use.
```cpp
// component_a.h
struct component_a {
  // data 
    int data_;
  // function pointers
    int (*get42)(struct component_a *);
};
```
For the factory function from a DLL aka "component" to be executed we need a function pointer that matches the  footprint of the factory function. 

Factory function does not have to be declared. It is defined by name in the `.def` file of the component and its full foot print is only implied.

But we need it function pointer so that we cna use it when we load the component dll.
```cpp
// component_a.h
// factory function pointer declaration is required
typedef struct component_a * (component_a_factory_fp)(void);
```
### Let us clarify. 

`dbj-component-loader.h` contains a macro that does it  all. 
```cpp
#define DBJCAPI_DLL_CALL(
dll_name_, 
fun_name_, 
RFP,          // the factory function pointer 
callback_)    // the callback where it is used   
```
Synopsis of its core explains succinctly the workings of DBJCS:

```cpp
// try and load the dll
dbjcapi_dll_load(dll_name_);
// get to the function first
// cast the result to required function pointer
RFP function_ = (RFP)dbjcapi_dll_get_function(fun_name_); 
// if function is found call the callback
// with it as a argument
if (function_)            
      callback_(function_);
// immediately "unload" the dll      
// Windows keeps it in a memory for a while
dbjcapi_dll_unload();
```
Example: On the struct interface of the `component_a.dll` declared is a function pointer `get42` . The struct interface is declared like this:
```cpp
struct component_a
{
    int data_;
    int (*get42)(struct component_a *);
};
```
Let us call that `get42`. We need:

- dll name
  - `"component_a.dll"`
- factory function name; always the same
  - `"dbj_component_factory"`
- factory function pointer of the exact component
  - different for each component
  - `component_a_factory_fp` declared in `"component_a.h"`
```cpp
// the callback where we will do the job
void use_component_a ( component_a_factory_fp factory_) ;

 // execute the load and call
DBJCAPI_DLL_CALL( 
  "component_a.dll",
 "dbj_component_factory", 
 component_a_factory_fp,
  use_component_a
 ) ;

 void use_component_a ( component_a_factory_fp factory_) 
{
  // get to the implementation instance
  struct component_a * implementation = factory_() ;
  
  // call function declared on it
  int  rezult = implementation->get42(implementation*);
}
// if any, dll loader failures are already logged
```


## The Roadmap

- Currently no UUID is really necessary
- no central "entity" to (for example) deliver implementations etc. 
- versioning use is a moot point
    - instead of delivering 1..N versions from one component have 1..N components, one per version 
    - that would mean a very many dll's
      - instead we might mandate the version function that would return the semver strings
- If using anything from Windows for do not go above `rpc.h` 
  - MSRPC is the very foundation of Windows
  - COM is 1993 OOA/OOD/OOP IPC 
  - COM is based on MSRPC
  - it is legacy and it is a problem
    - COM works just because of millions (or more?) man hours invested in it 

---

&copy; 2021 by dbj@dbj.org

https://dbj.org/license_dbj 