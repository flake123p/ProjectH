@echo off

echo ====== Clean Application ... ======
del .\Makefiles\*.d              /F /Q
del .\Makefiles\_00_Ext_Mods.mak /F /Q
del .\mod\*.txt                  /F /Q
del .\mod\*.mak                  /F /Q
del .\build\*.o                  /F /Q
del aout.exe                     /F /Q
del build_all_mod.bat            /F /Q
del build_all_mod.sh             /F /Q
del clean_all_mod.bat            /F /Q
del clean_all_mod.sh             /F /Q

if "%1" NEQ "--DisablePause" (
	pause
)
