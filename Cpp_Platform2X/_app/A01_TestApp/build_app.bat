@echo off

CALL _env.bat
SET LOCAL_PATH=%CD%

rem
rem Update mod_list.txt/mak
rem
CALL make --file Makefiles\ParsingModList.mak
CALL make --file Makefiles\ParsingModList2.mak

rem
rem Build Lib
rem
SET LIB_PATH=..\..\..\Cpp_Platform2\_Library
CD %LIB_PATH%
CALL lib_build.bat --DisablePathExport --DisablePause
CD %LOCAL_PATH%

rem
rem Build mods
rem
CALL build_all_mod.bat

rem
rem Update mod archieve's full name
rem
CALL make -f ./Makefiles/_00_Ext_Mods_Gen.mak
set RC___00_Ext_Mods_Gen=%ERRORLEVEL%

rem
rem Build App
rem
CALL make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles -I mod MY_OS=WIN
set RC___1_UpdateMFiles=%ERRORLEVEL%
CALL make Build_All -f ./Makefiles/_2_Build.mak -I Makefiles -I mod MY_OS=WIN
set RC___2_Build=%ERRORLEVEL%

pause