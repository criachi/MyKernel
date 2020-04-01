#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdio.h>
extern double PAGE_SIZE;
void pageFaultHelper(int pageNumber, int activePID);
int launcher(FILE *p);

#endif