#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ram.h"
#include "pcb.h"
#include "kernel.h"
#include "memorymanager.h"

#define FRAME_COUNT (int) (RAM_SIZE / PAGE_SIZE) // 40/4 = 10 FRAMES 
double PAGE_SIZE = 4.0;

// counts total nb of lines in a program 
double countLines(FILE *f) {
    char codeLine[1000];
    double lineCount = 0;
    while (fgets(codeLine, 1000, f) != NULL) {
            lineCount++; 
    }
    return lineCount;
}

// returns total nb of pages needed to load file completely into RAM
// have to initialize the PCB field max pages after this is called 
int countTotalPages(FILE *f) {
    double lineCount = countLines(f);
    if(lineCount == 0) {
        return 0;
    }

    if(ceil(lineCount / PAGE_SIZE) <= 1) {
        return 1;
    } 
    return ceil(lineCount / PAGE_SIZE);
}

// loads specified page into RAM 
void loadPage(int pageNumber, FILE *f, int frameNumber) { 
    rewind(f);
    char *line = NULL; 
    size_t linecap = 0;
    int lineNb = 0;
    for(int i = 0; i < (pageNumber * PAGE_SIZE); i++) {
        getline(&line, &linecap, f);
    }
    
    while(getline(&line, &linecap, f) != -1 && lineNb < 4) {
        addToRAM(line, lineNb + (frameNumber * PAGE_SIZE));
        lineNb++;
    }
    free(line);
}

// searches ram for first null frame it can find 
int findFrame() {
    int i = 0;
    while(read(i) != NULL && i < RAM_SIZE) {
        i+=4;
    }
    
    if(i == RAM_SIZE) return -1;
    return (int) (i/PAGE_SIZE); // i is the index of the first null Frame found in RAM  

}

// only invoked when findFrame returns -1
int findVictim(PCB *p) { // p is currently active pcb
    int frameNumber = rand() % FRAME_COUNT; 
    while (findFrameInPageTable(p, frameNumber) != -1) { // if from the get-go, the frame number is not found in the active pcb's pg table, then skip this while loop 
        frameNumber++;
        frameNumber = frameNumber % FRAME_COUNT;
    }
    return frameNumber;
}

//if a victim was selected then the PCB table of the victim frame must be changed; if not, we wld pass -1 instead of victimFrame
//we do this once for PCB asking for page fault, and we might do it agn for the victim PCB(if there was one)
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    // this means the p that was passed is that of the pcb asking for page fault so tht its program can be loaded into 
    // RAM
    (*p).pageTable[pageNumber] = frameNumber;
    if(victimFrame != -1) { // it means we had to choose a victim frame so need to update the pg table of the PCB which possessed this victim frame as well
        updateVictimPCB(victimFrame);
    } 
}

// responsible for loading initial <=2 pages of a program (default behavior)
int launcher(FILE *p) {
// copy the entire file into the backing store 
    char PID[32]; 
    sprintf(PID, "%d", getCurrentPID());
    const char *filename = strcat(PID,".txt");
    char filepath[1000] = "./BackingStore/";
    strcat(filepath, filename);
    FILE *output = fopen(filepath, "w+");
    if (output == NULL) 
    { 
        printf("Error: Cannot create file %s \n", filepath); 
        return 1;  
    }  
    char *line = NULL;
    size_t linecap = 0;
    while(getline(&line, &linecap, p) != -1) {
         if(line[0] != '\n' && line[0] != '\r') {
            fputs(line, output);
        }
    }
    free(line);
    fclose(output); 
    fclose(p);

    FILE* file = fopen(filepath, "r");
    int totalPages = countTotalPages(file);
    rewind(file);
    if(totalPages > 10) {
        printf("Error: Cannot launch a program with more than 40 instructions\n"); // TODO: recheck if need to print error msg and stop all execution if more than 1 script or not
        return 1;
    }
    PCB* pcbBeingCreated = myInit(totalPages); 
    int counter = 0;
    if(totalPages == 1) {
        counter = 1;
    } else if(totalPages > 1) {
        counter = 2;
    }
    for(int i = 0; i < counter; i++) {
        int victimFrame = -1;
        int freeFrameNumber = findFrame();
        if(freeFrameNumber == -1) { // if no free frames are found, must choose a victim frame to replace 
            int victimFrame = findVictim(getHeadOfReadyQueue()); 
            loadPage(i, file, victimFrame);
            if(i == 0) {
                setPC(getHeadOfReadyQueue(), victimFrame * FRAME_SIZE); // setting PC to be the starting address of first page of program in memory
            }
            updatePageTable(pcbBeingCreated, i, victimFrame, victimFrame); 
        } else { // else simply use this free frame to load the needed page(s)
            loadPage(i, file, freeFrameNumber);
            if(i == 0) {
                setPC(getHeadOfReadyQueue(), freeFrameNumber * FRAME_SIZE); // setting PC to be the starting address of first page of program in memory
            }
            updatePageTable(pcbBeingCreated, i, freeFrameNumber, victimFrame); // in this case: victimFrame = -1 and free frame nb is valid nb 
        }
    }
    fclose(file);
}

// same as launcher but loads only the specified page by opening the file in the backing store and finding an appropriate frame to load the page in 
void pageFaultHelper(int pageNumber, int activePID) {
    char PID[32]; 
    sprintf(PID, "%d", activePID);
    const char *filename = strcat(PID,".txt");
    char filepath[1000] = "./BackingStore/";
    strcat(filepath, filename);
    FILE *program = fopen(filepath, "r");
    if (program == NULL) { 
        printf("Cannot access file %s \n", filepath); 
        exit(1);  
    }  
    int victimFrame = -1;
    int freeFrameNumber = findFrame();
    if(freeFrameNumber == -1) {
        int victimFrame = findVictim(getActivePCB());
        loadPage(pageNumber, program, victimFrame);
        setPC(getActivePCB(), victimFrame * FRAME_SIZE);
        updatePageTable(getActivePCB(), pageNumber, victimFrame, victimFrame); 
    } else {
        loadPage(pageNumber, program, freeFrameNumber);
        setPC(getActivePCB(), freeFrameNumber * FRAME_SIZE);
        updatePageTable(getActivePCB(), pageNumber, freeFrameNumber, victimFrame); // in this case: victimFrame = -1 and free frame nb is valid nb 
    }
    fclose(program);
}
