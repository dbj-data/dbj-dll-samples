@rem
@rem
@echo off
@rem cls
setlocal
set "sourcebasename=dbj-syserrmsg"
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