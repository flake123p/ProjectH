#!/bin/bash

echo === Start of Make ===

make -f ./Makefiles/_1_UpdateMFiles.make -I Makefiles

make Build_All -f ./Makefiles/_2_Build.make -I Makefiles

makke_result=$?

echo === End of Make === result is ${makke_result}

if test ${makke_result} = 0
then
  echo === Start of a.out ===
  ./a.out
  echo === End of a.out ===
fi