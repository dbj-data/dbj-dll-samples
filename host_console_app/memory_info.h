
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

static inline void dbjcapi_print_mem_info(DWORD processID)
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS_EX pmc;

    DBG_PRINT("\nMemory info. Process ID: [%9lu]", processID);

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (NULL == hProcess)
    {
        DBG_PRINT("\nOpenProcess() failed?");
        return;
    }

    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc)))
    {
        DBG_PRINT("\nWorkingSetSize:\t\t\t %llu KB", (pmc.WorkingSetSize / 1024));
        DBG_PRINT("\nPrivate Bytes:\t\t\t %llu KB", (pmc.PrivateUsage / 1024));
        DBG_PRINT("\nPagefile (peak):\t\t %llu (%llu) KB\n", (pmc.PagefileUsage / 1024), (pmc.PeakPagefileUsage / 1024));
    }
    else
    {
        DBG_PRINT("\nGetProcessMemoryInfo() failed?");
    }
    CloseHandle(hProcess);
}

/*

NOTE: freeLibrary on dll  will not release memory immediately
one app starts most system dll's will simply stay in the memory

But user made dll's unloading will be very benefitial. Best example is resource only dll's
*/
static inline void dbjcapi_memory_info(void)
{
    dbjcapi_print_mem_info(GetCurrentProcessId());
}

DBJ_EXTERN_C_END

#endif // DBJ_MEMORY_INFO_INC
