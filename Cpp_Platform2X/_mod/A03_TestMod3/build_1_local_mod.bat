
for %%I in (.) do set CurrDirName=%%~nxI
echo ====== Start: Build Local Module [%CurrDirName%] ======
CALL make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=WIN
set build_1_local_mod_rc1=%ERRORLEVEL%

CALL make Build_All -f ./Makefiles/_2_BuildMod.mak -I Makefiles MY_OS=WIN
set build_1_local_mod_rc2=%ERRORLEVEL%

echo ====== End: Build Local Module. Errorlevel = %build_1_local_mod_rc1%, %build_1_local_mod_rc2% ====== 

IF %build_1_local_mod_rc1% NEQ 0 (
	echo ====== Error: Build Local Library ======
	set build_1_local_mod_rc=1
	goto END_OF_FILE
)
IF %build_1_local_mod_rc2% NEQ 0 (
	echo ====== Error: Build Local Library ======
	set build_1_local_mod_rc=1
	goto END_OF_FILE
)
set build_1_local_mod_rc=0
:END_OF_FILE
