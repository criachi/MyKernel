#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

typedef struct {
    int IP; // instr ptr: like the PCB PC field, it pts to the next instr to execute in RAM 
    char IR[1000]; // instr reg: stores the currently executing instruction: stores the instr tht will be sent to the interpreter() for execution
    int quanta; 
    int offset;
} CPU;

int getIP();
void setIP(int instrIndex);
char* getIR();
void createCPU(int quanta);
int run(int quanta);
void setQuanta(int quanta);
bool isCPUBusy();
void setOffset(int offset);
int getOffset();

#endif