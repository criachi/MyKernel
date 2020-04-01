#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "pcb.h"
#include "shell.h"
#include "ram.h"
#include "memorymanager.h"

#define QUANTA 2

// ready queue
PCB *head;
PCB *tail;

// running queue made of 1 PCB max
PCB *activePCB;

void addToReady(PCB *pcb_ptr)
{
    if (head == NULL)
    {
        head = pcb_ptr;
        tail = head;
    }
    else
    {
        tail->next = pcb_ptr;
        tail = pcb_ptr;
    }
}

PCB *getActivePCB()
{
    return activePCB;
}

PCB *getHeadOfReadyQueue()
{
    return head;
}

// called inside launcher; responsible only for creating PCB and adding it to ready queue
PCB *myInit(int pages_max)
{
    PCB *pcb_ptr = makePCB(pages_max); // makes a PCB using malloc
    addToReady(pcb_ptr);
    return pcb_ptr;
}

// finds the PCB whose frame was usurped by another PCB so that we can update its page table to indicate no mapping for its page
void updateVictimPCB(int victimFrame)
{
    PCB *temp = head;
    while (temp)
    {
        int pageTableIndex = findFrameInPageTable(temp, victimFrame);
        if (pageTableIndex != -1) // if a page from this PCB's page table was mapped to the victim frame that was usurped, then delete this mapping
        {
            temp->pageTable[pageTableIndex] = -1;
            break;
        }
        temp = temp->next;
    }
}

void scheduler()
{
    while (head)
    { // while there are PCBs in the ready queue
        while (isCPUBusy());
        // dequeueing active PCB
        activePCB = head;
        head = head->next;
        activePCB->next = NULL;
        if (activePCB->pageTable[activePCB->PC_page] == -1)
        { // if the frame this PCB previously held in RAM reserved for this next page has been usurped by another PCB during a page fault (as a victim frame)
        // reload this usurped frame by calling pageFaultHelper and add the PCB back to the tail of the ready queue 
            pageFaultHelper(activePCB->PC_page, activePCB->PID);
            addToReady(activePCB);
        }
        else
        { // upon context switch when PCB gains access to CPU, need to restore the PCB state where it left off 
        // by syncing up the CPU attributes with the PCB saved state
            setIP(activePCB->pageTable[activePCB->PC_page] * FRAME_SIZE);
            setOffset(activePCB->PC_offset);
            setQuanta(QUANTA);
            int returnCode = run(QUANTA);
            if (returnCode == 2)
            { // program terminated , need to delete pcb and free ram!
                for (int i = 0; i < PAGE_TABLE_SIZE; i++)
                {
                    int frameNumber = getFrameForPage(activePCB, i);
                    if (frameNumber != -1) // if this page number has a frame mapped to it in RAM, must free the frame in RAM to make way for other programs
                    {
                        freeRAMFrame(frameNumber);
                    }
                }
                //upon program termination, must delete the copy of the program file in the backing store/swap space 
                char PID[32];
                sprintf(PID, "%d", activePCB->PID);
                const char *filename = strcat(PID, ".txt");
                char filepath[100] = "BackingStore/";
                strcat(filepath, filename);
                char command[120] = "rm ";
                strcat(command, filepath);
                system(command);
                free(activePCB); 
            }
            else if (returnCode == 0)
            { // if quanta ended with no interrupt, need to do appropriate updates
                activePCB->PC = getIP();
                activePCB->PC_offset = getOffset();
            }
            if (returnCode == 0 || returnCode == 1)
            { // if page fault and no termination, or if quanta ended with no interrupt, need to move pcb to back of the list
                addToReady(activePCB);
            }
        }
    }
}

int handlePageFault()
{
    activePCB->PC_page++; // det the nxt pg of our currently active PCB (no loops in our programs)
    if (activePCB->PC_page >= activePCB->pages_max)
    {     
        return 2; // to indicate to run in cpu tht program terminated (at multiple of 4)
    }
    // else:
    int frameToBeLoaded = activePCB->pageTable[activePCB->PC_page];
    if (frameToBeLoaded == -1)
    { // frame is invalid - not in ram right now 
        pageFaultHelper(activePCB->PC_page, activePCB->PID);
    } // after loading the page in ram or realizing it is already loaded, must update these fields before putting PCB 
    // back at the tail of the ready queue
    activePCB->PC = activePCB->pageTable[activePCB->PC_page] * PAGE_SIZE;
    activePCB->PC_offset = 0;
    return 1;
}

// one-time fn call invoked by main() at the start to initialize and acquire the resources it needs to run
void boot()
{
    initializeRAM();
    system("rm -rf BackingStore && mkdir BackingStore"); 
}

int kernel()
{
    createCPU(QUANTA);
    head = NULL;
    tail = NULL;
    activePCB = NULL;
    return shellUI();
}
