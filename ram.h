#ifndef RAM_H
#define RAM_H

#define RAM_SIZE 40
#define FRAME_SIZE 4

char* read(int memAddress);
void initializeRAM();
void freeRAMFrame(int frameNumber);
void addToRAM(char* line, int memAddress);

#endif