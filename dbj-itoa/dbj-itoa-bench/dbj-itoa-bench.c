
// part of dbj machine_wide repository of foundation libs
// used "everywhere" on this machine
// roadmap: mandate DBJ_MACHINE_WIDE env var that will point to the root of it
// for example on this machine:
// set "DBJ_MACHINE_WIDE=D:\MACHINE_WIDE"

#define UBENCH_IMPLEMENTATION
#include <ubut/ubench.h>

UBENCH_STATE; // note there is no ()!
int main(int argc, const char *const argv[])
{
    system(" "); // yes win cmd still needs this
    return ubench_main(argc, argv);
}