#!/bin/bash

echo ====== Library Start "lib_build.sh" ======

make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles MY_OS=LINUX
lib_result1=$?

make Build_All -f ./Makefiles/_2_BuildArchive.mak -I Makefiles MY_OS=LINUX
lib_result2=$?

echo ====== Library Build Done!! Errorlevel = $lib_result1, $lib_result2 ======
