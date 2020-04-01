#include "kernel.h"
// simulates an OS loader
int main()
{
    int error = 0;
    boot();
    error = kernel();
    return error;
}