#pragma once
#ifndef DBJCS_DLL_LOADER_INC
#define DBJCS_DLL_LOADER_INC
#pragma clang system_header
/* (c) 2019 - 2021 by dbj.org   -- https://dbj.org/license_dbj

DBJCS == DBJ Component System
		
Here is dynamic dll loading and fetching a function from the said dll.
This file is easy to understand if you start from the two macros at the bottom:

DBJCS_CALL(dll_name_, RFP, callback_)

DBJCS_ANY_CALL(dll_name_, function_name, RFP, callback_) 


#define DBJCS_DLL_CALLER_IMPLEMENTATION  in exactly one place 
*/
#include <dbj_capi/dbj_lock_unlock.h>

/// --------------------------------------------------------------
#ifdef DBJCS_DLL_CALLER_IMPLEMENTATION
/// --------------------------------------------------------------

#include <dbj_capi/cdebug.h>
#include <dbj_capi/default_log.h>

/// WARNING!
/// requires
/// #define STB_DS_IMPLEMENTATION
/// #include <stb/stb_ds.h> // hash table
/// in one compilation unit in this same project
#include <stb/stb_ds.h> // hash table

#include <stdarg.h>
#include <stdio.h>

DBJ_EXTERN_C_BEGIN
/* 
user can provide the actual log function for this loader, the required signature is
extern "C" void (*user_log_FP) (const char* file, long line, const char* , ...);
otherwise we will use dbj_capi default loader, based on a stderr redirection to file
*/
#ifndef DBJCS_LOADER_LOG
// #define DBJCS_LOADER_LOG(...) dbjcapi_default_log_function(__FILE__, __LINE__, __VA_ARGS__)
#define DBJCS_LOADER_LOG(...) DBG_PRINT(__VA_ARGS__)
#endif // ! DBJCS_LOADER_LOG

#pragma region hash table of states

typedef struct dbjcs_loader_state dbjcs_loader_state;

dbjcs_loader_state dbjloader_dll_unload(dbjcs_loader_state state);

/*
 one loader state per one dll
 */
typedef struct dbjcs_loader_state
{
	/* 
	dll_name_ is the HT key
	DLL name should be just a "base file name.dll"
	we do not want users to load the dll's from wherever on the machine
	that is not going to work properly and is very unsafe 
	*/
	dbj_string_128 key;
	HINSTANCE dll_handle_;
	// critical section might be in here
} dbjcs_loader_state;

dbjcs_loader_state dbjloader_new_state_struct_(void)
{
	dbjcs_loader_state instance = {{0}, 0};
	return instance;
}
/// --------------------------------------------------------------
/// we maintain a hash table of states
/* here is the hash table to hold state descriptors    */
static struct dbjcs_loader_state *state_descriptors_hash = 0;

/* 
unload all the dll's on app exit
*/
__attribute__((destructor)) void dbj_loader_descriptors_hash_destructor(void)
{
	for (int j = 0; j < stbds_hmlen(state_descriptors_hash); ++j)
		dbjloader_dll_unload(state_descriptors_hash[j]);

	stbds_hmfree(state_descriptors_hash);
}
/// --------------------------------------------------------------

void dbj_loader_add_to_hash(struct dbjcs_loader_state loader_descriptor_)
{
	stbds_hmputs(state_descriptors_hash, loader_descriptor_);
}

// return the found one or the default return
// use hmdefaults - Set the default struct used by hmget, but where ?
// what is the default return if we do not use that?
struct dbjcs_loader_state dbj_loader_find_in_hash(dbj_string_128 key_)
{
	return stbds_hmgets(state_descriptors_hash, key_);
}
#if 0
// true if to be deleted was found
// false otherwise
static bool dbj_loader_remove_from_hash(dbj_string_128 key_)
{
	// stbdes_hmdel returns 0 or 1
	return stbds_hmdel(state_descriptors_hash, key_);
}
#endif // 0

/* -------------------------------------------------------------- */
bool dbjcs_dll_loaded(dbjcs_loader_state *state_)
{
	DBJ_ASSERT(state_);
	// if this is NOT NULL we know DLL by this name is loaded
	// until it is not dbjcapi_dll_unloaded no other DLL can be used and its function called
	return state_->dll_handle_ != 0;
}

#pragma endregion hash table of states

int dbjcs_assign_dll_name(dbjcs_loader_state *state, dbj_string_128 name_)
{
	DBJ_ASSERT(state);

	// major logic check here
#ifdef _DEBUG
	if (dbjcs_dll_loaded(state))
	{
		if (state->key.data[0] != 0)
			DBG_PRINT("DLL %s, is already loaded", state->key);
		else
		{
			DBG_PRINT("DLL name must exist if DBJ DLL LOADER state is LOADED?");
			DBJ_FAST_FAIL;
		}
	}
#endif // _DEBUG

	state->key = name_;
	return true;
}

/*
We *no longer* manage just a single DLL load 
*/
dbjcs_loader_state dbjloader_load(
	/* 
	remember just a file name, not a path! 
	adversary might insert unwanted dll from an unwanted location
	*/
	const char dll_file_name_[static 1])
{
	dbjcs_loader_state state;
	dbj_string_128 key_arg_;
	DBJ_STRING_ASSIGN(key_arg_, dll_file_name_);
	// first try the HT of already loaded dll's
	state = dbj_loader_find_in_hash(key_arg_);

	if (state.dll_handle_ != NULL)
	{
		return state;
	}

	state = dbjloader_new_state_struct_();
	// ie if dll is already loaded
	dbjcs_assign_dll_name(&state, key_arg_);
	/*
this is precisely a reason we do want DLL's as full paths
we shall do the compliant solution by refusing to load a library unless it is located precisely 
where expected. Thus we reduce the chance of executing attackers planted DLL, 
when dynamically loading libraries.
*/
	state.dll_handle_ = LoadLibraryExA(
		state.key.data,
		NULL,
		LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);

	if (NULL == state.dll_handle_)
	{
		DBJCS_LOADER_LOG("DLL Loader error: [%d]. "
						 "By design looking only in system folders and application folder."
						 " Could not find the DLL by name: %s",
						 GetLastError(), state.key.data);
		// reset the file name
		DBJ_STRING_SET_EMPTY(state.key);
	}
	else
	{
		dbj_loader_add_to_hash(state);
	}
	return state;
}
/*
FreeLibrary() failure is very rare and might signal
some deep error with the machines or OS
thus we will not ignore it.
*/
dbjcs_loader_state dbjloader_dll_unload(dbjcs_loader_state state)
{
	if (dbjcs_dll_loaded(&state))
	{
		if (!FreeLibrary(state.dll_handle_))
		{
			DBJCS_LOADER_LOG(
				"\ndbjdll_load FreeLibrary failed. The DLL name is: %s\n", state.key);
			DBJ_FAST_FAIL;
		}
		// leave it in unloaded state
		state.dll_handle_ = 0;
		DBJ_STRING_SET_EMPTY(state.key);
	}

	return state;
}

/*
RFP = Required Function FP of the function fromm the DLL

bellow returns null or function pointer to the one requested
void * "saves the day here" thus the function is generic
the user knows the RFP and will cast to it
*/
void *dbjloader_get_function(dbjcs_loader_state *state, char const fun_name_[static 1])
{
	DBJ_ASSERT(state);
	if (!dbjcs_dll_loaded(state))
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
			"\nFailed to find the address for a function: %s\nThe DLL name is: %s", fun_name_, state->key.data);
		DBJ_FAST_FAIL;
	}
	return result;
}
/// ---------------------------------------------------------------------------------------------------------
#endif // DBJCS_DLL_CALLER_IMPLEMENTATION
/// ---------------------------------------------------------------------------------------------------------

/// each dbj components must have one factory method
// with always the same name
#ifndef DBJCS_FACTORYNAME
#define DBJCS_FACTORYNAME "interface_factory"
#endif // ! DBJCS_FACTORYNAME
/*
RFP = Required Function FP of the function fromm the DLL

The do it all function,
get the factory function and cast its result to RFP
RFP = Actual Factory Function Pointer for the actual components
Call Back Function signature is: void ( * callback ) ( RFP )

if dll load has failed the callback will not be called.

NOTE: LOCKING IS ALWAY ON IN BOTH
*/
#define DBJCS_CALL(dll_name_, RFP, callback_)                                    \
	do                                                                           \
	{                                                                            \
		dbjcapi_function_lock_unlock();                                          \
		dbjcs_loader_state state_ = dbjloader_load(dll_name_);                   \
		RFP function_ = (RFP)dbjloader_get_function(&state_, DBJCS_FACTORYNAME); \
		if (function_)                                                           \
			callback_(function_);                                                \
		dbjcapi_function_lock_unlock();                                          \
	} while (0)

/*
call any function from the DLL whose full signature you know
*/
#define DBJCS_ANY_CALL(dll_name_, function_name, RFP, callback_)             \
	do                                                                       \
	{                                                                        \
		dbjcapi_function_lock_unlock();                                      \
		dbjcs_loader_state state_ = dbjloader_load(dll_name_);               \
		RFP function_ = (RFP)dbjloader_get_function(&state_, function_name); \
		if (function_)                                                       \
			callback_(function_);                                            \
		dbjcapi_function_lock_unlock();                                      \
	} while (0)

// ----------------------------------------------------------------------------
DBJ_EXTERN_C_END

#endif // DBJCS_DLL_LOADER_INC
