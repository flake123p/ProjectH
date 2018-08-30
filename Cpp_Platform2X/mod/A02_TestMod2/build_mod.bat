
for %%I in (.) do set CurrDirName=%%~nxI
echo ====== Start: Build Local Module [%CurrDirName%] ======
CALL make -f ./Makefiles/_00_Ext_Mods_Gen.mak
set build_mod_rc1=%ERRORLEVEL%

CALL make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=WIN %*
set build_mod_rc2=%ERRORLEVEL%

CALL make Build_All -f ./Makefiles/_2_BuildMod.mak -I Makefiles MY_OS=WIN
set build_mod_rc3=%ERRORLEVEL%

echo ====== End: Build Local Module. Errorlevel = %build_mod_rc1%, %build_mod_rc2% , %build_mod_rc3% ====== 

IF %build_mod_rc1% NEQ 0 (
	echo ====== Error: Build Local Module ======
	exit /b %build_mod_rc1%
)
IF %build_mod_rc2% NEQ 0 (
	echo ====== Error: Build Local Module ======
	exit /b %build_mod_rc2%
)
IF %build_mod_rc3% NEQ 0 (
	echo ====== Error: Build Local Module ======
	exit /b %build_mod_rc3%
)
