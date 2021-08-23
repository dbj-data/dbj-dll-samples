@rem
@rem
@echo off
@cls
setlocal
set "sourcebasename=dbj-host"
@md Build 2>nul

@rem /Zi -- debug builds + pdb file
@rem /TC -- threat all as C
@clang-cl %sourcebasename%.cpp /GR- /D_DEBUG /D_HAS_EXCEPTIONS=0 /nologo /Zi /W3 /FoBuild\ /FeBuild\%sourcebasename%.exe 

endlocal