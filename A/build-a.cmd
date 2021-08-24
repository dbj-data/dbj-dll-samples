@rem
@rem https://docs.microsoft.com/en-us/archive/msdn-magazine/2015/july/windows-with-c-windows-runtime-components
@rem
@echo off
@cls
setlocal
set "sourcebasename=component-a"
set "sourcefilename=%sourcebasename%"
set "build=..\Build"
@md %build% 2>nul

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
clang-cl %sourcefilename%.c /nologo /D_DEBUG /Zi /TC /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.dll /link /dll /def:%sourcefilename%.def

@echo.
@echo Done building: %build%\%sourcebasename%.dll
@echo.

endlocal