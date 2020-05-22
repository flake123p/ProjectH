#!/bin/bash

echo ====== Clean Application ... ======
rm ./Makefiles/*.d              --force --verbose
rm ./Makefiles/_00_Ext_Mods.mak --force --verbose
rm ./mod/*.txt                  --force --verbose
rm ./mod/*.mak                  --force --verbose
rm ./build/*.o                  --force --verbose
rm ./aout.exe                   --force --verbose
rm ./build_all_mod.bat          --force --verbose
rm ./build_all_mod.sh           --force --verbose
rm ./clean_all_mod.bat          --force --verbose
rm ./clean_all_mod.sh           --force --verbose