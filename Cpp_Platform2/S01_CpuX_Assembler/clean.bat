@echo off

SET CD_TEMP=%CD%
CD ..\_Library
CALL lib_clean.bat --DisablePause
CD %CD_TEMP%

echo ====== Clean Application ... ======
del .\Makefiles\*.d /F /Q
del .\build\*.o     /F /Q
del aout.exe        /F /Q

if "%1" NEQ "--DisablePause" (
	pause
)
