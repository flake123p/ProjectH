#!/bin/bash

echo ====== Build Mod [${PWD##*/}]  ======
make -f ./Makefiles/_00_Ext_Mods_Gen.mak
result1=$?

OFS_for_make="OFS=\"$@\""
make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=LINUX "$OFS_for_make"
result2=$?

make Build_All -f ./Makefiles/_2_BuildMod.mak -I Makefiles MY_OS=LINUX
result3=$?

echo ====== Build Done!! Errorlevel = $result1, $result2, $result3 ======

if [ $result1 != 0 ]; then
	exit $result1
fi

if [ $result2 != 0 ]; then
	exit $result2
fi

if [ $result3 != 0 ]; then
	exit $result3
fi
