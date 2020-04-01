#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ram.h"

char *ram[RAM_SIZE];

// initializes ram[] w/ NULL ptrs to indicate no code there
void initializeRAM()
{
    for (int i = 0; i < RAM_SIZE; i++)
    {
        ram[i] = NULL;
    }
}

// frees specified frame of RAM; a frame is made up of 4 cells in RAM (4 instructions)
void freeRAMFrame(int frameNumber) {
    for (int i = frameNumber * FRAME_SIZE; i < (frameNumber * FRAME_SIZE) + FRAME_SIZE; i++)
    {
        ram[i] = NULL;
    }
}

// stores passed instruction at specified memory address in ram (ie in specified cell index)
void addToRAM(char* line, int memAddress) {
    ram[memAddress] = strdup(line);
}

// returns instruction found in RAM at specified location
char *read(int memAddress)
{
    return ram[memAddress];
}
