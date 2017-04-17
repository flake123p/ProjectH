#!/bin/bash


echo ====== Start "lib_build.bat" ======
cd_temp=$PWD
cd ../_Library
lib_result1=0
lib_result2=0
#Source the 2nd script, i.e. . test2.sh and it will run in the same shell.
. ./lib_build.sh
echo cd_temp = $cd_temp
cd $cd_temp

result1=$lib_result1
result2=$lib_result2
errorlevel=$((result1 + result2))
if [ $errorlevel == 0 ]; then
	echo ====== Start "build.sh" ======

	make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=LINUX
	result1=$?

	make Build_All -f ./Makefiles/_2_Build.mak -I Makefiles MY_OS=LINUX
	result2=$?

	echo ====== Build Done!! Errorlevel = $result1, $result2 ====== 
else
	echo ====== Library Error, Abort Building!! ======
fi


