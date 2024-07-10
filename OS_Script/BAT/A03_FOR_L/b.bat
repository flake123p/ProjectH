@ECHO OFF
GOTO START

	FOR /L

	Conditionally perform a command for a range of numbers.

	Syntax   
		  FOR /L %%parameter IN (start,step,end) DO command 

	Key
	   start       : The first number  
	   step        : The amount by which to increment the sequence 
	   end         : The last number 

	   command     : The command to carry out, including any 
					 command-line parameters.

	   %%parameter : A replaceable parameter:
					 in a batch file use %%G (on the command line %G)
	So (20,-5,10) would generate the sequence (20 15 10) 

	(1,1,5) would generate the sequence 1 2 3 4 5

:START
ECHO Print a range of numbers. (FOR /L)
FOR /L %%x IN (3 2 8) DO (
	ECHO %%x
)

PAUSE