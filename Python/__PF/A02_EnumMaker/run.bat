@echo off

if "%2" NEQ "DisablePathSetting" (
	CALL _env.bat
)

python EnumMaker.py

if "%1" NEQ "DisablePrint" (
	pause
)
