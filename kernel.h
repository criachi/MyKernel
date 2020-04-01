#ifndef KERNEL_H
#define KERNEL_H
#include "pcb.h"

PCB* myInit(int pages_max);
void scheduler();
void updateVictimPCB(int victimFrame);
PCB* getActivePCB();
int handlePageFault();
PCB* getHeadOfReadyQueue();
void boot();
int kernel();

#endif