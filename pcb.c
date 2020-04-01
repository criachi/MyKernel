#include <stdlib.h>
#include "pcb.h"

int PID = 0; 

int getCurrentPID() {
    return PID;
}

void setPC(PCB* pcb, int PC) {
    pcb->PC = PC;
}

PCB* makePCB(int pages_max) {
    PCB* pcb_ptr = (PCB*) malloc(sizeof(PCB)*1);
    (*pcb_ptr).PID = PID;
    (*pcb_ptr).PC = 0;
    (*pcb_ptr).PC_page = 0;
    (*pcb_ptr).PC_offset = 0;
    (*pcb_ptr).next = NULL;
    for(int i = 0; i < PAGE_TABLE_SIZE; i++) {
        (*pcb_ptr).pageTable[i] = -1;
    }
    (*pcb_ptr).pages_max = pages_max;
    PID++;
    return pcb_ptr;
}

int getFrameForPage(PCB* p, int pageNumber) {
    return p->pageTable[pageNumber];
}

// searches the page table of the specified pcb for a mapping to the specified frame number
// returns the page mapped to the frame if such a mapping is found, else returns -1
int findFrameInPageTable(PCB *p, int frameNumber) {
    for(int i = 0; i < PAGE_TABLE_SIZE; i++) {
        if(p->pageTable[i] == frameNumber) {
            return i;
        }
    }
    return -1;
}
