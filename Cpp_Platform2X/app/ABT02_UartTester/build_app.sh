#!/bin/bash

local_app_path=$PWD

while : ; do
  #
  # [[[ Update mod_list.txt/mak ]]]
  #
  make --file Makefiles/ParsingModList.mak
  rc=$?
  if [ $rc != 0 ]; then
    echo ====== Build Error ======
    break
  fi
  make --file Makefiles/ParsingModList2.mak
  rc=$?
  if [ $rc != 0 ]; then
    echo ====== Build Error ======
    break
  fi
  #
  # [[[ LIBRARY ]]]
  #
  cd ../../../Cpp_Platform2/_Library
  lib_result1=0
  lib_result2=0
  #Source the 2nd script, i.e. . test2.sh and it will run in the same shell.
  . ./lib_build.sh
  cd $local_app_path
  errorlevel=$((lib_result1 + lib_result2))
  if [ $errorlevel != 0 ]; then
    echo ====== Library Error, Abort Building!! ======
    break
  fi
  #
  # [[[ Build all mods ]]]
  #
  ./build_all_mod.sh
  rc=$?
  if [ $rc != 0 ]; then
    echo ====== Build Error ======
    break
  fi
  #
  # [[[ Update mod archieve's full name ]]]
  #
  make -f ./Makefiles/_00_Ext_Mods_Gen.mak
  rc=$?
  if [ $rc != 0 ]; then
    echo ====== Build Error ======
    break
  fi

  #
  # [[[ Build App ]]]
  #
  make -f ./Makefiles/_1_UpdateMFiles.mak -I Makefiles -I mod MY_OS=LINUX
  result1=$?
  make Build_All -f ./Makefiles/_2_Build.mak -I Makefiles -I mod MY_OS=LINUX
  result2=$?
  echo ====== APP Build Done!! Errorlevel = $result1, $result2 ======
  break
done
