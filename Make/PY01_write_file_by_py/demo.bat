@echo off

CALL _env.bat

CALL make xtarget -f MyMakefile.mak

pause