# MyOS
This program is meant to simulate an OS with a command-line interface that allows you to issue 6 basic commands. The simulation includes a kernel (and scheduler), loader, a single-core cpu, ram, and memory management using paging.

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
| exec p1 p2 p3  | Executes concurrent programs (1 to 3 programs, which can be the same file)  |
| quit  |  Exits / terminates the shell with "Bye!"  |

## Scheduler 
### The Ready Queue
Implemented as FIFO and RR. It is a linked list.
### The Running "Queue" 
Made up of only 1 active PCB pointer since we are simulating a single-core CPU. 
### The PCB 
Contains field such as PC, the program counter, keeping track of the instruction to be executed in the process, as well as PC_page and PC_offset to track the process's current page and the instruction offset within that page. 
### Quanta
For the purposes of this simulation, a quanta is defined to be 2 instructions. It can be interrupted when a process needs to switch to the next frame (whether this frame is loaded into RAM or not). At the end of a quanta or upon generation of an interrupt, the running process's PCB is placed back at the end of the ready queue. 

## Virtual Memory 
Virtual memory simulation is used for the exec command. A program is launched by copying the entire file into a BackingStore directory. By default, 2 pages of the program at most are initially loaded into RAM. For the purposes of this simulation, RAM is a array of 40 strings representing instructions, and a page (and equivalently a frame) is made up of 4 instructions, meaning RAM can hold up to 10 frames. 

To load the pages of a program into RAM whether at launch time or at a page fault, RAM is searched from beginning to end for the first free frame. If no free frames are found, a victim frame is chosen from the other processes on the ready queue to be replaced by the current process's frame. The victim frame is chosen by generating a random frame number initially. If that frame belongs to the currently active process, then the frame number is incremented successively until a suitable victim frame is found. Note that each process's PCB has a page table of size 10, meaning a program cannot have more than 10 pages. The appropriate page tables of all PCB's involved are updated. 

The CPU generates an interrupt when a frame is exhausted of its instructions and the process must access its next page of instructions. If the next page needed is already loaded into RAM, the relevant information about which page the process is currently on is saved in its PCB and the PCB is added back at the end of the ready queue. If the next page needed is not in RAM, it is fetched from the backing store and loaded into RAM. The PCB in this case also loses its quanta and is added back to the end of the ready queue with information about its state saved. 

Upon program termination, the PCB is deleted as well as the copy of the program file in the backing store. 



