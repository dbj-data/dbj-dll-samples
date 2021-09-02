
![](unsplash/work_in_progres_raw.jpg)
# TODO
## Is Component loader too restrictive?
- At the moment it is one dll one user at one time, usage pattern
  - But is that too restrictive? 
  - restrictive is good when is stops the explosion of wrong usage patterns
  - What about multi proc desktop solutions?
  - is dbj-shmem good enough?
    - tested enough?
  - What about component using another component pattern?
- Why not keeping it restrictive and see what happens?
- One perhaps neutral addendum might be to organize a simple cache of dll's inside a loader
```cpp
#define DBJCS_FUTURE_ANY_CALL(dll_name_, function_name,RFP, callback_)           
	do {
        // dll name is the dll cache key
        // if in cache will not be loaded
		dbjcs_dll_load(dll_name_);
        // dll is found by the key  
        // aka dll name                            
		RFP function_ = (RFP)dbjcs_dll_get_function(dll_name_, function_name); 
		if (function_)                                          
			callback_(function_)        
        // can unload on demand
        // or from a destructor at app exit
        // but OS does that anyway                           
		dbjcs_dll_unload(dll_name_)                                     
	} while (0)  
```
 
## Locking implementations ideas
- All seems fine right now
- academic interest: https://preshing.com/20120226/roll-your-own-lightweight-mutex/

## Brave new components
1. Open source C library for drawing vector graphics? In a DBJCS Component? Why not?
  - https://preshing.com/20170529/heres-a-standalone-cairo-dll-for-windows/
1.  Key Value Storage using SQLite

## DBJ SHMEM

- DBJ SHMEM is transient key value storage
  - there is was a system wide COM+ component doing exactly this?! ([or even more](https://docs.microsoft.com/en-us/office/vba/language/reference/user-interface-help/dictionary-object)?)
- Architecturally might be critical if we keep the restrictive component loader. 
  - In that scenario there are many components single using other components, single using other components, ad infinitum.
  - They all talking to each other using SHMEM
  - Risky or Simple?
- That one SHMEM might be a point of contention like Registry is today
  - Not that much because it is transient, not  persistent
- Implementation to be tested. Properly.
