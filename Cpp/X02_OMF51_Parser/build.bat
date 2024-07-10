
@ECHO off

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

rem CLS

echo ====== Start "build.bat" ======

CALL make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=WIN
set result1=%ERRORLEVEL%

CALL make Build_All -f ./Makefiles/_2_Build.mak -I Makefiles MY_OS=WIN
set result2=%ERRORLEVEL%

echo ====== Build Done!! Errorlevel = %result1%, %result2% ====== 

if "%2" NEQ "--DisablePause" (
	pause
)
