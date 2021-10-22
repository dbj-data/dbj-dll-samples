#include "dbj-syserrmsg.h"
#include "../dbj-component.h"

DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 1, 0, "dbj syserrmsg component");
/* --------------------------------------------------------------------------------- */
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION
/* --------------------------------------------------------------------------------- */
#include <windows.h>

#ifndef DBJ_ASSERT
#include <crtdbg.h>
#define DBJ_ASSERT _ASSERTE
#endif // ! DBJ_ASSERT

#include <stdio.h>

#include <lmerr.h>

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

// https://docs.microsoft.com/en-us/windows/win32/netmgmt/looking-up-text-for-error-code-numbers

static HMODULE netmsg_handle_ = NULL;
static bool also_dump_to_stderr_ = false;

static dbj_string_512
MakeErrorText(
    DWORD dwLastError)
{
    // dbj_string_512 msg_ = {{""}};
    HMODULE hModule = NULL; // default to system source
    dbj_string_512 MessageBuffer = {{0}};
    DWORD dwBufferLength = sizeof MessageBuffer.data;

    DWORD dwFormatFlags = /* FORMAT_MESSAGE_ALLOCATE_BUFFER | */ FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM;

    //
    // If dwLastError is in the network range,
    //  load the message source dll.
    // if not already loaded
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
        else //    netmsg_handle_ != NULL
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
             (LPSTR)&MessageBuffer.data,
             sizeof(MessageBuffer.data) - 1,
             NULL)))
    {
        if (also_dump_to_stderr_)
        {
            static dbj_string_512 final_buffer_ = {{0}};

            DBJ_STRING_SET_EMPTY(final_buffer_);

#ifdef _DEBUG
            static_assert(_countof(final_buffer_.data) == sizeof(final_buffer_.data), "count == sizeof");
#endif

            // how much has been written into the final_buffer
            int snprintf_rez = _snprintf_s((char *restrict)final_buffer_.data,
                                           (rsize_t)sizeof(final_buffer_.data),
                                           _TRUNCATE,
                                           (const char *restrict)"[%6d]: %s", dwLastError, MessageBuffer);

            if (snprintf_rez < 0)
                goto clean_exit;

            // message to stderr.
            DWORD dwBytesWritten = 0;
            // so simple it is difficult to spot: WriteFile does not know about zero delimited strings
            /*BOOL write_file_rez =*/(void)WriteFile(
                GetStdHandle(STD_ERROR_HANDLE),
                final_buffer_.data,
                snprintf_rez, /* write only this much */
                &dwBytesWritten,
                NULL);

            // DBJ_ASSERT(write_file_rez);
        } // also_dump_to_stderr_
    }

    //
    // DBJ: leave the dll in, OS will take care of it on exit
    // if (hModule != NULL)
    //    FreeLibrary(hModule);

clean_exit:
    return MessageBuffer;
}
static dbj_string_512 error_message_implementation_(struct component_syserrmsg *self_, const unsigned error_code)
{
    DBJ_ASSERT(self_);
    return MakeErrorText(error_code);
}
/* --------------------------------------------------------------------------------- */
static void also_to_stderr_implementation(bool arg_)
{
    also_dump_to_stderr_ = arg_;
}
/* --------------------------------------------------------------------------------- */
/* interface implementation */
static struct component_syserrmsg interface_implementation_ = {

    .also_to_stderr = also_to_stderr_implementation,
    .error_message = error_message_implementation_};

struct component_syserrmsg *interface_factory(void)
{
    return &interface_implementation_;
}