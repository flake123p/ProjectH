@ECHO OFF

SET ORI_CD=%CD%

rem
rem User config variable ( DO_FILTER = YES or NO )
rem
SET DO_FILTER=YES
SET EXTENTION_FILTER=.txt

rem
rem Delete old output file: result.txt
rem
IF EXIST result.txt (
	DEL /F /Q result.txt
)

CD ..
FOR /R %%A IN (*) DO (
	IF %DO_FILTER% EQU NO (
		ECHO %%A>>"%ORI_CD%/result.txt"
		rem ECHO %%A
	) ELSE (
		FOR %%B IN (%EXTENTION_FILTER%) DO (
			rem
			rem Compare to filter one by one
			rem
			IF %%~xA EQU %%B (
				ECHO %%A>>"%ORI_CD%/result.txt"
				rem ECHO %%A
			) ELSE (
				rem
				rem Not match, do nothing
				rem
			)
		)
	)
)

CD %ORI_CD%
