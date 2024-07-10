@echo off

rem Define paths
SET LIB_PATH=..\..\..\Cpp_Platform2\_Library
SET LOCAL_PATH=%CD%

rem Do clear
CD %LIB_PATH%
CALL lib_clean.bat --DisablePause
CD %LOCAL_PATH%

if "%1" NEQ "--DisablePause" (
	pause
)
