@rem
@rem
@echo off
@rem cls
setlocal

@rem Setup VS2019
call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set "sourcebasename=dbj-c-host"
set "build=..\Build"
@md %build% 2>nul

@set "INCLUDE=D:\machine_wide;%INCLUDE%"

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
@rem /MTd -- use static debug runtime
@rem /MDd -- use debug dll runtime
@clang-cl %sourcebasename%.c /I "D:\machine_wide" /GR- /MTd /D_DEBUG /D_HAS_EXCEPTIONS=0 /nologo /Zi /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.exe 

@echo.
@echo Done building: %build%\%sourcebasename%.exe
@echo.

endlocal