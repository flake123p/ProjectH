
@ECHO off

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

rem CLS

echo ====== Start "lib_build.bat" ======
SET CD_TEMP=%CD%
CD ..\_Library
CALL lib_build.bat --DisablePathExport --DisablePause
CD %CD_TEMP%
set result1=%lib_result1%
set result2=%lib_result2%
IF %lib_result1% NEQ 0 (
	echo ====== Library Error, Abort Building!! ======
	goto END_OF_BUILD
)
IF %lib_result2% NEQ 0 (
	echo ====== Library Error, Abort Building!! ======
	goto END_OF_BUILD
)

echo ====== Start "build.bat" ======
CALL make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=WIN
set result1=%ERRORLEVEL%

CALL make Build_All -f ./Makefiles/_2_Build.mak -I Makefiles MY_OS=WIN
set result2=%ERRORLEVEL%

echo ====== Build Done!! Errorlevel = %result1%, %result2% ====== 

:END_OF_BUILD

if "%2" NEQ "--DisablePause" (
	pause
)

