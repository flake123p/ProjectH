#!/bin/bash

echo ====== Build Mod [${PWD##*/}]  ======
make -f ./Makefiles/_00_Ext_Mods_Gen.mak
result1=$?

make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=LINUX
result2=$?

make Build_All -f ./Makefiles/_2_BuildMod.mak -I Makefiles MY_OS=LINUX
result3=$?

echo ====== Build Done!! Errorlevel = $result1, $result2, $result3 ====== 
