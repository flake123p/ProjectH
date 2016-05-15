@echo off

CALL DumpFilePaths.bat

if "%2" NEQ "DisablePathSetting" (
	CALL _env.bat
)

python test.py

if "%1" NEQ "DisablePrint" (
	pause
)
