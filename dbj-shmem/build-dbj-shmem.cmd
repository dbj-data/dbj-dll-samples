@rem
@rem https://docs.microsoft.com/en-us/archive/msdn-magazine/2015/july/windows-with-c-windows-runtime-components
@rem
@echo off
@rem cls
setlocal
set "sourcebasename=dbj-shmem"
set "sourcefilename=%sourcebasename%"
set "build=..\Build"
@md %build% 2>nul

@set "INCLUDE=F:\machine_wide;%INCLUDE%"

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
clang-cl %sourcefilename%.c /nologo /I "f:\machine_wide" /MDd /D_DEBUG /Zi /TC /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.dll /link /dll /def:%sourcefilename%.def

@echo.
@echo Done building: %build%\%sourcebasename%.dll
@echo.

endlocal