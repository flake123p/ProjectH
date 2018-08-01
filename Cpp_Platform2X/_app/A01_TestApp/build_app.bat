@echo off

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)
SET LOCAL_PATH=%CD%

rem
rem Update mod_list.txt/mak
rem
CALL make --file Makefiles\ParsingModList.mak
rem
SET RET=%ERRORLEVEL%
IF %RET% NEQ 0 ( goto ERROR )

CALL make --file Makefiles\ParsingModList2.mak
rem
SET RET=%ERRORLEVEL%
IF %RET% NEQ 0 ( goto ERROR )

rem
rem Build Lib
rem
SET LIB_PATH=..\..\..\Cpp_Platform2\_Library
CD %LIB_PATH%
CALL lib_build.bat --DisablePathExport --DisablePause
CD %LOCAL_PATH%
rem
SET RET=%lib_result1%
IF %RET% NEQ 0 ( goto ERROR )
SET RET=%lib_result2%
IF %RET% NEQ 0 ( goto ERROR )

rem
rem Build all mods
rem
CALL build_all_mod.bat
rem
SET RET=%ERRORLEVEL%
IF %RET% NEQ 0 ( goto ERROR )

rem
rem Update mod archieve's full name
rem
CALL make -f ./Makefiles/_00_Ext_Mods_Gen.mak
rem
SET RET=%ERRORLEVEL%
IF %RET% NEQ 0 ( goto ERROR )

rem
rem Build App
rem
CALL make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles -I mod MY_OS=WIN
rem
SET RET=%ERRORLEVEL%
IF %RET% NEQ 0 ( goto ERROR )

CALL make Build_All -f ./Makefiles/_2_Build.mak -I Makefiles -I mod MY_OS=WIN
rem
SET RET=%ERRORLEVEL%
IF %RET% NEQ 0 ( goto ERROR )

aout.exe
GOTO END

:ERROR
ECHO ERRORLEVEL = %RET%

:END
if "%2" NEQ "--DisablePause" (
	pause
)
