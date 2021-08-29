
#ifndef DBJ_MEMORY_INFO_INC
#define DBJ_MEMORY_INFO_INC

#include <dbj_capi/ccommon.h>
#include <dbj_capi/cdebug.h>
#include <dbj_capi/dbj_windows_include.h>
// #include <windows.h> 
// #include <stdio.h>
#include <psapi.h>
// #include <assert.h>

DBJ_EXTERN_C_BEGIN

static inline void dbjcapi_print_mem_info(FILE* fp_ , DWORD processID)
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS_EX pmc;

    fprintf( fp_, "\nMemory info. Process ID: [%9lu]", processID);

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (NULL == hProcess)
    {
        fprintf(fp_, "\nOpenProcess() failed?");
        return;
    }

    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
    {
        fprintf( fp_, "\nWorkingSetSize:\t\t\t %llu KB", (pmc.WorkingSetSize / 1024));
        fprintf( fp_, "\nPrivate Bytes:\t\t\t %llu KB", (pmc.PrivateUsage / 1024));
        fprintf( fp_, "\nPagefile (peak):\t\t %llu (%llu) KB\n", (pmc.PagefileUsage / 1024), (pmc.PeakPagefileUsage / 1024));
    }
    else {
        fprintf(fp_, "\nGetProcessMemoryInfo() failed?");
    }
        CloseHandle(hProcess);
}

/*
will use stderr
assumption is stderr will be redirected before 

NOTE: freeLibrary on dll  will not release memory immediately
one app starts most system dll's will simply stay in the memory

But user made dll's unloading will be very benefitial. Best example is resource only dll's
*/
static inline void  dbjcapi_memory_info( FILE * fp_ )
{
    DBJ_ASSERT(fp_);
    dbjcapi_print_mem_info(fp_, GetCurrentProcessId());
}

DBJ_EXTERN_C_END

#endif // DBJ_MEMORY_INFO_INC
