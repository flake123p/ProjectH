@echo off

if "%2" NEQ "DisablePathSetting" (
	CALL _env.bat
)

python main.py

if "%1" NEQ "DisablePrint" (
	pause
)
