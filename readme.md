
![unsplash](unsplash/manuel.jpg)

# DBJ COMPONENT SYSTEM&trade; (DBJCS)

Why are today's OS-es component systems so complex, and difficult to use? Behind this very pompous name is attempt to architect and implement the simplest possible component system.

Using only modern C and Windows DLL. 

If some "feature" is missing we can always pull the card of simplicity as an excuse. Let us see how far can we get.

## How is this working?

- one components is one DLL
- one component has one interface has one implementation
- each component (DLL) has the same def file 
  ```
EXPORTS
dbj_component_can_unload_now      PRIVATE
dbj_component_get_implementation  PRIVATE
  ```
- each component has to define two string literals
  1. The component dll file name
  2. The component factory function name
- that is enough information to runtime load the dll (aka component) and get to the factory function
- factory function returns a single pointer to the component interface  
  - actually an instance of a struct 
  - that interface aka struct is the only thing required to de declared in the component header


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