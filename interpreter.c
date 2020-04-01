#include "interpreter.h"
#include "shellmemory.h"
#include "kernel.h"
#include "memorymanager.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

char **tokenize(char *str)
{
    size_t num_tokens = 1;
    int flag = 0;
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (flag == 0 && str[i] == ' ')
        {
            num_tokens = num_tokens + 1;
            flag = 1;
        }
        if (str[i] != ' ')
        {
            flag = 0;
        }
    }
    char **ret_arr =
        (char **)calloc((num_tokens + 1), sizeof(char *)* (num_tokens + 1)); 

    if (ret_arr == NULL)
    {
        perror("calloc");
        return NULL;
    }
    flag = 0;
    int ignore_flag = 0;
    char *modified_str = (char *)str;
    size_t counter = 0;
    const size_t length_str = strlen(str);
    for (size_t i = 0; i < length_str; i++)
    {
        if (modified_str[i] == '\n' || modified_str[i] == '\r')
            modified_str[i] = ' ';
        if (modified_str[i] == '"')
        {
            ignore_flag = ignore_flag ^ 0x1;
        }
        if (flag == 0 && modified_str[i] != ' ')
        {
            ret_arr[counter] = &(modified_str[i]);
            counter = counter + 1;
            flag = 1;
        }
        if (modified_str[i] == '\\' && modified_str[i + 1] == ' ')
        {
            i++;
            continue;
        }
        if (flag == 1 && modified_str[i] == ' ' && ignore_flag == 0)
        {
            modified_str[i] = '\0';
            flag = 0;
            continue;
        }
    }
    ret_arr[counter] = NULL;

    for (size_t i = 0; i < counter; ++i)
    {
        if (ret_arr[i][0] == '\"' &&
            ret_arr[i][strlen(ret_arr[i] - 1)] == '\"')
        {
            ret_arr[i][strlen(ret_arr[i]) - 1] = '\0';
            ret_arr[i] = ret_arr[i] + 1;
        }
    }

    return ret_arr;
}

int in_file_flag = 0;
int in_exec_flag = 0;
int interpreter(char *raw_input);

int help()
{
    printf(""
           "COMMAND         DESCRIPTION\n"
           "help            Displays all the commands\n"
           "quit            Exits / terminates the shell with \"Bye!\"\n"
           "set VAR STRING  Assigns a value to shell memory\n"
           "print VAR       Displays the STRING assigned to VAR\n"
           "run SCRIPT.TXT  Executes the file SCRIPT.TXT\n"
           "exec p1 p2 p3   Executes concurrent programs\n");
    return 0;
}

int quit()
{
    printf("Bye!\n");
    if (in_file_flag == 0 && in_exec_flag == 0)
    {
        shell_memory_destroy();
        exit(0);
    } 
    return 0;
}

static int run(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Script not found.\n");
        return 1;
    }
    int enter_flag_status = in_file_flag;
    in_file_flag = 1;
    while (!feof(file))
    {
        char *line = NULL;
        size_t linecap = 0;
        getline(&line, &linecap, file);

        int status = interpreter(line);
        free(line);
        if (status != 0) 
        {
            break;
            return status;
        }
    }
    fclose(file);
    in_file_flag = enter_flag_status;
    return 0;
}

int set(const char *key, const char *value)
{
    int status = shell_memory_set(key, value);
    if (status != 0)
        printf("set: Unable to set shell memory.\n");
    return status;
}

int print(const char *key)
{
    const char *value = shell_memory_get(key);
    if (value == NULL)
    {
        printf("print: Undefined value.\n");
        return 1;
    }
    printf("%s\n", value);
    return 0;
}

int exec(char **tokens)
{ 
    in_exec_flag = 1;
    int numScripts = 1;
    if (tokens[2] != NULL)
        numScripts++;
    if (tokens[3] != NULL)
        numScripts++;
        
    for (int i = 1; i <= numScripts; i++)
    {
        FILE *file = fopen(tokens[i], "r");
        if (file == NULL)
        {
            printf("Error: %s could not be found!\n", tokens[i]);
            return 1;
        }
        int returnToken = launcher(file);
        if (returnToken == 1) 
        {
            return 1;
        };
    }

    scheduler();
    in_exec_flag = 0;
    return 0;
}

int interpreter(char *raw_input)
{
    char **tokens = tokenize(raw_input);
    if (tokens[0] == NULL)
        return 0; // empty command

    if (strcmp(tokens[0], "help") == 0)
    {
        if (tokens[1] != NULL)
        {
            printf("help: Malformed command\n");
            free(tokens);
            return 1;
        }
        free(tokens);
        return help();
    }
    if (strcmp(tokens[0], "quit") == 0)
    {
        if (tokens[1] != NULL)
        {
            printf("quit: Malformed command\n");
            free(tokens);
            return 1;
        }
        if (in_file_flag == 0 && in_exec_flag == 0)
        {
            free(raw_input);
        }

        //free(raw_input); 
        free(tokens);
        return quit();
    };

    if (strcmp(tokens[0], "set") == 0)
    {
        if (!(tokens[1] != NULL && tokens[2] != NULL && tokens[3] == NULL))
        {
            printf("set: Malformed command\n");
            free(tokens);
            return 1;
        }
        int status = set(tokens[1], tokens[2]);
        free(tokens);
        return status;
    }

    if (strcmp(tokens[0], "print") == 0)
    {
        if (!(tokens[1] != NULL && tokens[2] == NULL))
        {
            printf("print: Malformed command\n");
            free(tokens);
            return 1;
        }
        int status = print(tokens[1]);
        free(tokens);
        return status;
    }

    if (strcmp(tokens[0], "run") == 0)
    {
        if (!(tokens[1] != NULL && tokens[2] == NULL))
        {
            printf("run: Malformed command\n");
            free(tokens);
            return 1;
        }
        int result = run(tokens[1]);
        free(tokens);
        return result;
    }

    if (strcmp(tokens[0], "exec") == 0)
    {
        if (tokens[1] == NULL || (tokens[2] != NULL && tokens[3] != NULL && tokens[4] != NULL)) // if it has no arguments or more than 3 arguments
        {
            printf("exec: Malformed command\n");
            free(tokens);
            return 1;
        }
        int result = exec(tokens);
        free(tokens);
        return result;
    }
    printf("Unrecognized command \"%s\"\n", tokens[0]);
    free(tokens);
    return 1;
}
