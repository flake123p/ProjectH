@echo off

if "%2" NEQ "DisablePathSetting" (
	CALL _env.bat
)

python test.py 123 abc

if "%1" NEQ "DisablePrint" (
	pause
)
