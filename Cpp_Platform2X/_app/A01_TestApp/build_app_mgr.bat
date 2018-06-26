
@ECHO off

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

CALL build_1_ext_lib.bat
IF %build_1_ext_lib_rc% EQU 0 (
	echo ====== Start [1] Build Ext Lib ... continue ======
) ELSE (
	echo ====== Start [1] Build Ext Lib ... failed ======
	goto END_OF_BUILD
)

:END_OF_BUILD

if "%2" NEQ "--DisablePause" (
	pause
)

