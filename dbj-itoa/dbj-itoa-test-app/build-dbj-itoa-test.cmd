@echo off
@rem cls

setlocal

call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set "sourcebasename=dbj-itoa_test"
set "build=..\..\Build"
@md %build% 2>nul

@set "INCLUDE=F:\machine_wide;%INCLUDE%"

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
@rem /MTd -- use static debug runtime
@rem /MDd -- use debug dll runtime
@clang-cl %sourcebasename%.c decimal_from.c itoa_test.c /I "F:\machine_wide" /GR- /MTd /D_DEBUG /D_CRT_SECURE_NO_WARNINGS /D_HAS_EXCEPTIONS=0 /nologo /Zi /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.exe 

@echo.
@echo Done building: %build%\%sourcebasename%.exe
@echo.

endlocal