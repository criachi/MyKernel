#ifndef PCB_H
#define PCB_H

#define PAGE_TABLE_SIZE 10

typedef struct
{
    // NOTE: PC field is only updated after a task switch
    int PC; // refers to the cell number of ram[] containing the instr to execute
    // these 3 attributes track which pg and offset the prog is currently at and the total nb of pgs in the program
    int PC_page;
    int PC_offset;
    int pages_max;
    int PID;
    struct PCB *next;
    int pageTable[PAGE_TABLE_SIZE]; // index of the array is the pg number 
} PCB;

int findFrameInPageTable(PCB *p, int frameNumber);
PCB *makePCB(int pages_max);
int getCurrentPID();
int getFrameForPage(PCB* p, int pageNumber);
void setPC(PCB* pcb, int PC);

#endif