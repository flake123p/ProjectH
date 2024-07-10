@echo off

REM SET CD_TEMP=%CD%
REM CD ..\_Library
REM CALL lib_clean.bat --DisablePause
REM CD %CD_TEMP%

echo ====== Clean Application ... ======
del .\Makefiles\*.d /F /Q
del .\build\*.o     /F /Q
del aout.exe        /F /Q

if "%1" NEQ "--DisablePause" (
	pause
)
