#!/bin/bash

echo ====== Clean Mod [${PWD##*/}] ======
rm ./Makefiles/*.d              --force --verbose
rm ./Makefiles/_00_Ext_mods.mak --force --verbose
rm ./build/*.o                  --force --verbose
rm ./archive/*.a                --force --verbose

