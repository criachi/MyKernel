# MyShell

My shell program which supports 5 basic commands and seeks to emulate bash behavior. 

| COMMAND | DESCRIPTION|
| ------------- | ------------- |
| help  |  Displays all the commands  |
| quit  | Exits / terminates the shell with “Bye!”  |
| set VAR STRING | Assigns a value to shell memory |
| print VAR |		Displays the STRING assigned to VAR |
| run SCRIPT.TXT 	|	Executes the file SCRIPT.TXT |

To compile the application using gcc:

gcc -c shell.c interpreter.c shellmemory.c

gcc -o mysh shell.o interpreter.o shellmemory.o 

To run the shell program: ./mysh (from the command line) 

You can also pipe a text file with commands to the program by doing: ./mysh < scriptname.txt which will have an output that emulates reading from user input. It will read each line and execute the command on each line. Once it is done reading commands, it will terminate running the script. 

An example testfile is provided in this repository as "myTestFile.txt". 
