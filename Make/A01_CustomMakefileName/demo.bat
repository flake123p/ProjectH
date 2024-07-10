@echo off

CALL _env.bat

CALL make -f MyMakefile.txt

CALL make --file MyMakefile.txt

pause