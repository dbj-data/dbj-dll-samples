# Shared Memory in a DBJ Component

Basically wit this tool you can create inter process, named memory blocks. Inter process means, block are shared between processes if all use this same DLL and are holding it in their memory in the same time.

When last process release this component (dll) the shared memory is lost. Thus, this is not a persistent storage.

Shared Memory term is often abbreviated into: ***shmem***.

In WIN32, shmem is actually implemented as a (kind-of-a) file which is shared. It is much easier to understand if you think of it in that way. A shared file, that disappears after last user has released it.

## The API

DBJ Component is used through one struct.
```cpp
struct component_shmem
{
```
Create a named shmem block of required size under a given name.
```cpp
    bool (*create)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */, unsigned /*size*/ );
```
Delete a named shmem block.
```cpp    
    bool (*delete)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */ );
```
Set value into the named shmem block.
```cpp
    bool (*set_value)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */, unsigned /* size */, void  * /* value */ );
```
Get the value from the named shmem block.
```cpp    
    bool (*get_value)(struct component_shmem * /* self_ */, dbj_string_64 /* key_ */, unsigned /* size */, void ** /* value */ );
```
End of `struct component_shmem`
```cpp    
};
```
It is obvious all methods return false on any error. The error is in the good old `errno`.
## Usage
Here is the typical callback using this dbj component. This is a callback, after its done component is unloaded.

```cpp
static inline void component_b_user(component_shmem_factory_fp factory)
{
    // single pointer to the struct component interface
  struct component_shmem *implementation = factory();

// name of the shmem block
  dbj_string_64 key;
  DBJ_STRING_ASSIGN(key, "key_one");

// create shmem to store a single int
  DBJ_VERIFY(implementation->create(implementation, key, sizeof(int)));

// store the int value in a shmem
  int fty2 = 42;
  DBJ_VERIFY(implementation->set_value(implementation, key, sizeof(int), &fty2));

// retrieve the single named int from the shmem  
  int retrieved = 0;
  int * ptr = & retrieved ;
  DBJ_VERIFY(implementation->get_value(implementation, key, sizeof(int), (void **)&ptr));
  DBJ_VERIFY(*ptr == 42 );

  // manually remove the shmem
  DBJ_VERIFY(implementation->delete (implementation, key));
}
```
Using the DBJ Component loader that callback is used typically like so:
```cpp
int main(int argc, char **argv)
{
  DBJCS_CALL(
      /* dll file name */
      "dbj-shmem.dll", 
      /* 
      method name, always: 'interface_factory' 
      defined in dbj-component.h
      */
      DBJCS_FACTORYNAME, 
      /* defined in the header of this dll */
      component_factory_fp, 
      component_user /* callback above */
    );

  return 0;
}
```