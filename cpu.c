#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "interpreter.h"
#include "ram.h"
#include "cpu.h"
#include "memorymanager.h"
#include "kernel.h"

CPU cpu;
bool isBusy = false;

void createCPU(int quanta)
{
    cpu.quanta = quanta;
    cpu.IP = -1;
    cpu.offset = 0;
}

void setQuanta(int quanta)
{
    cpu.quanta = quanta;
}

int getIP()
{
    return cpu.IP;
}

void setIP(int instrIndex)
{
    cpu.IP = instrIndex;
}

char *getIR()
{
    return cpu.IR;
}

void setIR(char *instruction)
{
    strcpy(cpu.IR, instruction);
}

bool isCPUBusy()
{
    return isBusy;
}

void setOffset(int offset)
{
    cpu.offset = offset;
}

int getOffset()
{
    return cpu.offset;
}

int run(int quanta)
{
    isBusy = true;
    while (quanta > 0)
    {
        if (read(cpu.IP + cpu.offset) == NULL)
        {
            isBusy = false;
            return 2;
        }
        setIR(strdup(read(cpu.IP + cpu.offset)));
        cpu.offset++;
        interpreter(cpu.IR);
        quanta--;
    }
    if (cpu.offset == PAGE_SIZE)
    { // if offset is 4 (reached end of frame), we generate a pseudo-interrupt
        int programEnded = handlePageFault(); 
        isBusy = false;
        return programEnded;
    }
    isBusy = false;
    return 0;
}