#!/bin/bash

cd_temp=$PWD
cd ../_Library
./lib_clean.sh
cd $cd_temp

echo ====== Clean Application ... ======
rm ./Makefiles/*.d --force --verbose
rm ./build/*.o     --force --verbose
rm ./aout.exe      --force --verbose

