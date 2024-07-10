@ECHO OFF

ECHO Simple "for" example:

rem
rem variable: x  (MUST USE "SINGLE" CHARACTER)
rem
FOR %%x IN (2 3 go) DO (
	ECHO %%x
)

PAUSE