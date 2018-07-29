@echo off

rem Define paths
SET LIB_PATH=..\..\..\Cpp_Platform2\_Library
SET LOCAL_PATH=%CD%

rem Do clear
CD %LIB_PATH%
CALL lib_clean.bat --DisablePause
CD %LOCAL_PATH%

echo ====== Clean Application ... ======
del .\Makefiles\*.d              /F /Q
del .\Makefiles\_00_Ext_Mods.mak /F /Q
del .\mod\*.txt                  /F /Q
del .\mod\*.mak                  /F /Q
del .\build\*.o                  /F /Q
del aout.exe                     /F /Q

if "%1" NEQ "--DisablePause" (
	pause
)
