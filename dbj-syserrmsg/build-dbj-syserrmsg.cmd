@rem
@rem
@echo off
@rem cls
setlocal

@rem Setup VS2019
call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64


set "sourcebasename=dbj-syserrmsg"
set "sourcefilename=%sourcebasename%"
set "build=..\Build"
@md %build% 2>nul

@set "INCLUDE=d:\machine_wide;%INCLUDE%"

@echo Include: %INCLUDE%
@pause

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
clang-cl %sourcefilename%.c /nologo /I "d:\machine_wide" /MDd /D_DEBUG /Zi /TC /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.dll /link /dll /def:%sourcefilename%.def

@echo.
@echo Done building: %build%\%sourcebasename%.dll
@echo.

endlocal