
@ECHO off

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

rem CLS

echo ====== Library Start "build.bat" ======

CALL make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=WIN
set lib_result1=%ERRORLEVEL%

CALL make Build_All -f ./Makefiles/_2_BuildArchive.mak -I Makefiles MY_OS=WIN
set lib_result2=%ERRORLEVEL%

echo ====== Library Build Done!! Errorlevel = %lib_result1%, %lib_result2% ====== 

if "%2" NEQ "--DisablePause" (
	pause
)
