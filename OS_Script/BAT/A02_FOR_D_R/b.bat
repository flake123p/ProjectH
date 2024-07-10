@ECHO OFF
GOTO START
======================================================================
	FOR /D

	Conditionally perform a command on several Directories/Folders.

	Syntax
		  FOR /D [/r] %%parameter IN (folder_set) DO command

	Key
	   folder_set  : A set of one or more folders. Wildcards must be used.

	   command     : The command to carry out, including any
					 command-line parameters.

	   %%parameter : A replaceable parameter:
					 in a batch file use %%G (on the command line %G)

	   /r          : Recurse into subfolders (see notes below)
	   
	Example Directories:
		DirA/file1
		DirA/DirC/file3
		DirB/file2
======================================================================
:START
ECHO Print simple folder name without recurse (FOR /D):
FOR /D %%x IN (*) DO (
	ECHO %%x
)
ECHO.

ECHO Print complete folder name with recurse (FOR /D /R):
FOR /D /R %%x IN (*) DO (
	ECHO %%x
)
ECHO.

ECHO Print complete file name with recurse (FOR /R):
FOR /R %%x IN (*) DO (
	ECHO %%x
)
PAUSE