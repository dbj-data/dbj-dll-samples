#include "dbj-syserrmsg.h"
#include "../dbj-component.h"

DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 1, 0, "dbj syserrmsg component");
/* --------------------------------------------------------------------------------- */
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION;
/* --------------------------------------------------------------------------------- */
#include <windows.h>
#include <stdio.h>

#include <lmerr.h>

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

// https://docs.microsoft.com/en-us/windows/win32/netmgmt/looking-up-text-for-error-code-numbers

static HMODULE netmsg_handle_ = NULL;

static dbj_string_512
DisplayErrorText(
    DWORD dwLastError)
{
    dbj_string_512 msg_ = {{""}};
    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_IGNORE_INSERTS |
                          FORMAT_MESSAGE_FROM_SYSTEM;

    //
    // If dwLastError is in the network range,
    //  load the message source.
    //
    if (dwLastError >= NERR_BASE && dwLastError <= MAX_NERR)
    {
        // DBJ: added another handle and made it static
        // so this happens only once
        if (netmsg_handle_ == NULL)
        {
            netmsg_handle_ = LoadLibraryEx(
                TEXT("netmsg.dll"),
                NULL,
                LOAD_LIBRARY_AS_DATAFILE);
        }

        if (netmsg_handle_ != NULL)
        {
            dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
            hModule = netmsg_handle_;
        }
    }

    //
    // Call FormatMessage() to allow for message
    //  text to be acquired from the system
    //  or from the supplied module handle.
    //

    if ((dwBufferLength = FormatMessageA(
             dwFormatFlags,
             hModule, // module to get message from (NULL == system)
             dwLastError,
             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
             (LPSTR)&MessageBuffer,
             0,
             NULL)))
    {
        // DWORD dwBytesWritten;
        //
        // Output message string on stderr.
        //
        // WriteFile(
        //     GetStdHandle(STD_ERROR_HANDLE),
        //     MessageBuffer,
        //     dwBufferLength,
        //     &dwBytesWritten,
        //     NULL);

        // basically nobody knows what is the win err max message length
        // maybe it is 512 since that is a BUFSIZ
        DBJ_ASSERT(dwBufferLength < 512);

        DBJ_STRING_ASSIGN(msg_, MessageBuffer);

        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }

    //
    // If we loaded a message source, unload it.
    // DBJ: no leave it in, OS will take care of it on exit
    // if (hModule != NULL)
    //    FreeLibrary(hModule);

    return msg_;
}
static dbj_string_512 error_message_implementation_(struct component_syserrmsg *self_, const unsigned error_code)
{
    DBJ_ASSERT(self_);
    return DisplayErrorText(error_code);
}
/* --------------------------------------------------------------------------------- */
/* interface implementation */
static struct component_syserrmsg interface_implementation_ = {.error_message = error_message_implementation_};

struct component_syserrmsg *dbj_component_factory(void)
{
    return &interface_implementation_;
}