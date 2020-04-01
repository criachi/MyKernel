#include "interpreter.h"
#include "shellmemory.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int shellUI()
{
    printf("Kernel 2.0 loaded!\nWelcome to the Christina Riachi shell!\nShell version 2.0 Updated February 2020\n");

    shell_memory_initialize();
    while (1) //!feof(stdin)
    {
        printf("$ ");
        fflush(stdout);
        char *line = NULL;
        size_t linecap = 0;
        if (getline(&line, &linecap, stdin) == -1) {
            freopen("/dev/tty", "r", stdin);
            //break;
        } else {
            (void)interpreter(line);
            free(line);
        }
    }

    shell_memory_destroy();

    return 0;
}
