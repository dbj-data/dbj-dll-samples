@rem
@rem https://docs.microsoft.com/en-us/archive/msdn-magazine/2015/july/windows-with-c-windows-runtime-components
@rem
@echo off
@cls
setlocal
set "sourcefoldername=B\"
set "sourcebasename=component-b"
set "sourcefilename=%sourcefoldername%%sourcebasename%"
@md Build 2>nul

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
clang-cl %sourcefilename%.c /nologo /Zi /TC /W3 /FoBuild\ /FeBuild\%sourcebasename%.dll /link /dll /def:%sourcefilename%.def

endlocal