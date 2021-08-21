@rem
@rem https://docs.microsoft.com/en-us/archive/msdn-magazine/2015/july/windows-with-c-windows-runtime-components
@rem
@echo off
@cls
setlocal
set "sourcebasename=dbj-component"
@md Build 2>nul

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
@clang-cl %sourcebasename%.c /nologo /Zi /TC /W3 /FoBuild\ /FeBuild\%sourcebasename%.dll /link /dll /def:%sourcebasename%.def

endlocal