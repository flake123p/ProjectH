
@ECHO off

echo ====== Start: Create Mod Batchs ======
CALL make -f ./Makefiles/_3_CreateModBatchs.mak -I Makefiles MY_OS=WIN
set build_2_ext_mods_rc=%ERRORLEVEL%

echo ====== End: Create Mod Batchs!! Errorlevel = %build_2_ext_mods_rc% ====== 
