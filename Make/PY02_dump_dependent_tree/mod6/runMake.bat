@echo off

CALL _env.bat

CALL make --file MyMakefile.mak
CALL make --file MyMakefile2.mak

pause