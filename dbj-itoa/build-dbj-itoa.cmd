@rem
@rem (c) 2021 by dbj@dbj.org
@rem 
@rem
@echo off
@cls
setlocal

@rem Setup VS2019
@rem change to your local path, if need be
call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set "sourcebasename=dbj-itoa"
set "sourcefilename=%sourcebasename%"
set "build=..\Build"
@md %build% 2>nul

@set "INCLUDE=D:\machine_wide;%INCLUDE%"

@rem /Zi -- debug builds + pdb file -- remove this switch for release builds
@rem /TC -- threat all as C
clang-cl %sourcefilename%.c src\dbj_itoa_inner.c /nologo /MDd /D_DEBUG /Zi /TC /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.dll /link /dll /def:%sourcefilename%.def

@echo.
@echo Done building: %build%\%sourcebasename%.dll
@echo.

endlocal