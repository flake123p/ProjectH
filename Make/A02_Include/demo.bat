@echo off

CALL _env.bat

CALL make xxx -f MyMakefile.txt -I INC

CALL make xxx --file MyMakefile.txt --include-dir INC

pause