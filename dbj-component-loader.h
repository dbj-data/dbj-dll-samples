#ifndef DBJCS_DLL_LOADER_INC
#define DBJCS_DLL_LOADER_INC
/* (c) 2019 - 2021 by dbj.org   -- https://dbj.org/license_dbj

DBJCS == DBJ Component System
		
Here is dynamic dll loading and fetching a function from the said dll

#define DBJCS_DLL_CALLER_IMPLEMENTATION  in exactly one place 

**Caveat emptor**

This loader is single DLL loader. 
It works in 4 sequential states.
Single DLL can be used by a single user at one time.

state name  |  loader step
------------+--------------
LOADED 		1. load the dll
FUNCTIONS	2. get the function(s) from it
USERS		3. use the function(s) from the callback
UNLOADED 	4. unload the dll

Please be sure not to abuse these steps. For dynamic usage use only this loader.
Of course you are free to link in the components at build time and use them.
Although be aware that is a monolithic application, 
without better partitioning dynamic components are giving.

*/

#include <dbj_capi/ccommon.h>

// ----------------------------------------------------------------
DBJ_EXTERN_C_BEGIN

typedef enum dbjcs_dll_call_semver
{
	major = 1,
	minor = 5,
	patch = 0
} dbjcs_dll_call_semver;

DBJ_EXTERN_C_END

/// --------------------------------------------------------------
#ifdef DBJCS_DLL_CALLER_IMPLEMENTATION
/// --------------------------------------------------------------

#include <dbj_capi/cdebug.h>
#include <dbj_capi/default_log.h>

#include <stdarg.h>
#include <stdio.h>

DBJ_EXTERN_C_BEGIN
/// --------------------------------------------------------------
/// user can provide the actual log function
/// the required signature is
///
/// extern "C" void (*user_log_FP) (const char* file, long line, const char* , ...);
#ifndef DBJCS_LOADER_LOG
#define DBJCS_LOADER_LOG(...) dbjcapi_default_log_function(__FILE__, __LINE__, __VA_ARGS__)
#endif // DBJCS_LOADER_LOG

#undef DBJCS_LOADER_BUF_LEN
#define DBJCS_LOADER_BUF_LEN 1024

typedef struct dbjcs_loader_state
{
	// critical section will be in here
	HINSTANCE dll_handle_;
	char dll_name_[DBJCS_LOADER_BUF_LEN];
} dbjcs_loader_state;

static inline dbjcs_loader_state *dbjcs_loader_state_(void)
{
	static dbjcs_loader_state instance = {0, {0}};
	return &instance;
}

static inline bool dbjcs_dll_loaded(void)
{
	dbjcs_loader_state *state = dbjcs_loader_state_();
	// if this is NOT NULL we know DLL by this name is loaded
	// until it is not dbjcapi_dll_unloaded no other DLL can be used and its function called
	return state->dll_handle_ != 0;
}

static inline int dbjcs_assign_dll_name(char const name_[static 1])
{
	dbjcs_loader_state *state = dbjcs_loader_state_();

	// major logic check here
#ifdef _DEBUG
	if (dbjcs_dll_loaded())
	{
		if (state->dll_name_[0] != 0)
			DBG_PRINT("DLL %s, is already loaded", state->dll_name_);
		else
		{
			DBG_PRINT("DLL name must exist if DBJCAPI DLL LOADER state is LOADED?");
			DBJ_FAST_FAIL;
		}
	}
#endif // _DEBUG

	return strncpy_s(state->dll_name_, DBJCS_LOADER_BUF_LEN, name_, strlen(name_));
}

/*
We manage just a single DLL load 

DLL load --> call -->dbjcs_dll_unload
*/
static inline void dbjcs_dll_load(
	const char dll_file_name_[static 1])
{
	// will do fast fail on logic error
	// ie if dll is already loaded
	dbjcs_assign_dll_name(dll_file_name_);

	dbjcs_loader_state *state = dbjcs_loader_state_();

	state->dll_handle_ = LoadLibraryA(state->dll_name_);

	if (NULL == state->dll_handle_)
	{
		DBJCS_LOADER_LOG(" Could not find the DLL by name: %s", state->dll_name_);
		// reset the file name
		state->dll_name_[0] = '\0';
	}
}
/*
FreeLibrary() failure is very rare and might signal
some deep error with the machines or OS
thus we will not ignore it.
*/
static inline void dbjcs_dll_unload()
{
	if (dbjcs_dll_loaded())
	{
		dbjcs_loader_state *state = dbjcs_loader_state_();
		if (!FreeLibrary(state->dll_handle_))
		{
			DBJCS_LOADER_LOG(
				"\ndbjdll_loadFreeLibrary failed. The DLL name is: %s\n", state->dll_name_);
			DBJ_FAST_FAIL;
		}
		// leave it in unloaded state
		state->dll_handle_ = 0;
		state->dll_name_[0] = '\0';
	}
}

// we will use the destructor to free the dll if any is left in memory
__attribute__((destructor)) static inline void dbjcs_dll_loader_destructor(void) { dbjcs_dll_unload(); }

/*
RFP = Required Function FP of the function fromm the DLL

bellow returns null or function pointer to the one requested
void * "saves the day here" thus the function is generic
the user knows the RFP and will cast to it
*/
static inline void *dbjcs_dll_get_function(char const fun_name_[static 1])
{
	dbjcs_loader_state *state = dbjcs_loader_state_();
	if (!dbjcs_dll_loaded())
	{
		DBJ_FAST_FAIL;
		return 0;
	}

	// Funny fact: GetProcAddress has no unicode equivalent
	FARPROC result =
		GetProcAddress(
			// handle to DLL module
			(HMODULE)state->dll_handle_,
			// name of a function
			fun_name_);
	if (result == 0)
	{
		DBJCS_LOADER_LOG(
			"\nFailed to find the address for a function: %s\nThe DLL name is: %s", fun_name_, state->dll_name_);
		DBJ_FAST_FAIL;
	}
	return result;
}

#endif // DBJCS_DLL_CALLER_IMPLEMENTATION

#ifndef DBJCS_FACTORYNAME
#define DBJCS_FACTORYNAME "dbj_component_factory"
#endif // ! DBJCS_FACTORYNAME
/*
RFP = Required Function FP of the function fromm the DLL

The do it all function,
get the factory function and cast its result to RFP
RFP = Actual Factory Function Pointer for the actual components
Call Back Function signature is: void ( * callback ) ( RFP )

if dll load has failed the callback 
will not be called.
*/
#define DBJCS_FACTORY_CALL(dll_name_, RFP, callback_)                   \
	do                                                                  \
	{                                                                   \
		dbjcs_dll_load(dll_name_);                                      \
		RFP function_ = (RFP)dbjcs_dll_get_function(DBJCS_FACTORYNAME); \
		if (function_)                                                  \
			callback_(function_);                                       \
		dbjcs_dll_unload();                                             \
	} while (0)

/*
call any function from the DLL whose full signature you know
*/
#define DBJCS_ANY_CALL(dll_name_, function_name, RFP, callback_)    \
	do                                                              \
	{                                                               \
		dbjcs_dll_load(dll_name_);                                  \
		RFP function_ = (RFP)dbjcs_dll_get_function(function_name); \
		if (function_)                                              \
			callback_(function_);                                   \
		dbjcs_dll_unload();                                         \
	} while (0)

// ----------------------------------------------------------------------------
DBJ_EXTERN_C_END

#endif // DBJCS_DLL_LOADER_INC
