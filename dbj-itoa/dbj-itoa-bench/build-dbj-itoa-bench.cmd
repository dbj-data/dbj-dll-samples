@echo off
@rem cls

setlocal

call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set "sourcebasename=dbj-itoa-bench"
set "build=..\..\Build"
@md %build% 2>nul

@set "INCLUDE=D:\machine_wide;%INCLUDE%"

@rem /Zi -- debug builds + pdb file
@rem /D_DEBUG
@rem /TC -- threat all as C
@rem /MTd -- use static debug runtime
@rem /MDd -- use debug dll runtime
@rem this is release build
@clang-cl %sourcebasename%.c benchmarks.c /O2 /I "D:\machine_wide" /GR- /MT /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /D_HAS_EXCEPTIONS=0 /nologo /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.exe 

@echo.
@echo Done building: %build%\%sourcebasename%.exe RELEASE build
@echo.

endlocal