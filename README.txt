This is the documentation for the Christina Riachi Shell Version 1.0 January 2020.

I used Windows Subsystem for Linux to code this assignment. 

Instructions to run this program: 

Run the mysh executable file. My testfile provided is called myTestFile.txt. The help command reads from a file which contains the command info as found in the assignment. This file is called command_info.txt so make sure it is on your machine and fopen() is able to find it.  

Some key things to look out for: (This is to be read if you have doubts as to whether my implementation is valid or not. Only read the help command instructions because they are important!)  
I implement error handling by having commands return errorCodes to the shell.c file into the main function so that the corresponding error message can be printed and displayed to the user.

help command: I made this command read the command names and descriptions from a text file called "command_info.txt" which I have supplied 
along with the other required assignment files. This is because I anticipated adding more commands and functionality to the shell in future assignments and thought
a file would be easier to modify than some code consisting of formatted printf statements. I also check that there are no arguments passed to this command, otherwise, it does not execute and it displays an error message explaining that this command requires 0 arguments. I did this because it does not explicitly go against any of the assignment requirements, and does not go against emulating bash either. If in bash, one types help and an unrecognized command after it, it prints an error message. Granted, I could have added functionality to allow the user to type "help valid command" and display only the information pertinent to that command, but the assignment only required that we provide the user with the ability to ask for help concerning all commands. 

quit command: The quit command, when called from inside a script which is executed with the "run SCRIPT.txt" command, exits only the script to emulate the behavior of popular shells such as bash. Otherwise, if quit is typed manually by the user or it is a command inside a piped file, then it exits the shell. I do not check for 0 arguments with this command since in bash, if one types "exit" followed by a random word, it does not print an error and performs the requested functionality. 

set VAR STRING: I check that the number of arguments is exactly 2. In addition, since I parse based on the " " delimiter, I do not consider a string with space in it as 1 argument.
So, in the specific test case: set bobbie Montreal Canadians, my program considers this an error relating to too many arguments (3) and displays an error message accordingly.  
I considered making my code more accommodating for this case, but that would involve enforcing the need for double quotes around the string containing a space in order to stay true to the BASH shell which I used as inspiration to handle my program's behavior. This would have added to the complexity of my parsing function while the main objective of the assignment was to familiarize ourselves with the basic flow and components of a shell.  

print VAR: I also check for the number of arguments (number of args must be exactly 1) and print an error message if needed.

run SCRIPT.TXT: I check for the number of arguments to ensure it is 1 and print an error message otherwise. Again, the shell does not currently support filenames with spaces in the name because the parser would consider each space-separated string as a separate argument and I currently do not have the quotation feature enabled that allows the shell to bypass this, like Bash does.
In addition, if the script contains a badly formed command (with too many or too few arguments), or an invalid (non-existing) command, my program prints an error message and terminates the script. Note that when piping the contents of a file to the program (ie taking in input from the file instead of STDIN), error messages are printed and the rest of the script continues to be read, because this emulates user input. 
Because of this, if a file has commands that will result in error handling followed by a run SCRIPT.TXT command which also contains erroneous commands, then the first erroneous command found in the SCRIPT.TXT file that was run inside the initial piped file will cause an error message to be printed on the terminal and finish executing SCRIPT.TXT. 
So recursion will only work (ie occur MORE than 1 time) if the file being recursively called has no erroneous commands before the recursive call. 

Error Codes and their Significance to Functions:
-- This documentation is more for my own personal convenience for future assignments where I assume we will be iterating upon our existing code.

VALUE                   ERROR MESSAGE
  3			This command requires 2 arguments
 -3                     Script not found
  2			This command requires 1 argument
  1			This command requires 0 arguments
 -1			Could not display commands (for the help command in case the text file could not be opened)
 -2			Memory is full! Cannot add variable
  5			Unknown command
 -5			Variable does not exist (in case of printing a non-existing variable)
