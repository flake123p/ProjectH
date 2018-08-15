
@ECHO off

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

CALL build_mod.bat

if "%2" NEQ "--DisablePause" (
	pause
)

