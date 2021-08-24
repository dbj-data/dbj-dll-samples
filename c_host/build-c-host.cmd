@rem
@rem
@echo off
@cls
setlocal
set "sourcebasename=dbj-c-host"
set "build=..\Build"
@md %build% 2>nul

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
@clang-cl %sourcebasename%.cpp /GR- /D_DEBUG /D_HAS_EXCEPTIONS=0 /nologo /Zi /W3 /Fo%build%\ /Fe%build%\%sourcebasename%.exe 

@echo.
@echo Done building: %build%\%sourcebasename%.exe
@echo.

endlocal