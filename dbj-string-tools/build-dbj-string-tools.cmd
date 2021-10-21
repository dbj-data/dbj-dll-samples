@rem
@rem (c) 2021 by dbj@dbj.org
@rem 
@rem dependancy: dbj_capi 
@rem member of machine_wide repo that contains all the "machine wide"
@rem common stuff used by all the projects; on this machine from f:/machine_wide
@rem
@REM F:\machine_wide, git remote -v yields:
@REM
@REM origin  https://github.com/dbj-data/machine_wide.git (fetch)
@REM origin  https://github.com/dbj-data/machine_wide.git (push)
@rem
@echo off
@rem cls
setlocal

@rem Setup VS2019
@rem change to your local path, if need be
call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set "sourcebasename=dbj-string-tools"
set "sourcefilename=%sourcebasename%"
set "build=..\Build"
@md %build% 2>nul

@set "INCLUDE=F:\machine_wide;%INCLUDE%"

@rem /Zi -- debug builds + pdb file -- remove this switch for release builds
@rem /TC -- threat all as C
clang-cl %sourcefilename%.c /nologo /I "f:\machine_wide" /MDd /D_DEBUG /Zi /TC /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.dll /link /dll /def:%sourcefilename%.def

@echo.
@echo Done building: %build%\%sourcebasename%.dll
@echo.

endlocal