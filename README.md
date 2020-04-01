# MyKernel
This program is meant to simulate an OS with a command-line interface that allows you to issue 6 basic commands. The simulation includes a kernel (and scheduler), loader, cpu, ram, and memory management using paging.

## How to Run
From your command line: 

"make" to compile the mykernel program, followed by "make run" to start the program and receive a prompt to input a command. 

"make test" to run the mykernel program while piping in testfile.txt as input.

"make mytest" to run the mykernel program while piping in mytestfile.txt as input.

"make clean" to delete all object files and the compiled "mykernel" program created.

## Available Commands
| COMMAND  | DESCRIPTION |
| ------------- | ------------- |
| help  | Content Cell  |
| set VAR STRING   |  Creates a variable and assigns it a value in shell memory  |
| print VAR  | Displays the STRING assigned to VAR  |
| run SCRIPT.TXT  | Executes the file SCRIPT.TXT  |
| exec p1 p2 p3  | Executes concurrent programs (1 to 3 programs)  |
| quit  |  Exits / terminates the shell with "Bye!"  |

## Scheduler 

## Virtual Memory 


