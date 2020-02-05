#include <string.h>
#include <stdio.h>
#include <stdlib.h> // for exit()
#include "shellmemory.h"
#include "shell.h"
// assumes format of command line is gonna be command arguments
// assumes words[0] is command
int errorCode = 0;
int wordNumber = 0;
int runningScript = 0;
int quit() {
	if(runningScript == 1) {	// so that we only exit the script if it is called by "run script"
		printf("%s\n", "Bye!");
		return 0;
	}
	printf("%s\n", "Bye!");
	exit(0);
}

int help() {
	if(wordNumber != 1) {
		return 1;
	}
	int newlineCounter = 0;
	FILE *fptr;
	const char filename[] = "command_info.txt";
	char c;
	fptr = fopen(filename, "r");
	if (fptr == NULL) {
		return -1;	// triggers error msg cld not display commands
	}

	c = fgetc(fptr);
	while(c != EOF) { // c!=EOF
		printf("%c", c);
		c = fgetc(fptr);
	}

	fclose(fptr);
	return 0;
}

int setVar(char* words[]) {
	if(wordNumber != 3) {
		return 3;
	}
//	printf("setting");
	char* var = words[1];
	char* value = words[2];

	return setVarValue(var, value);
}

int printVar(char* words[]) {
	if(wordNumber != 2) {
		return 2;
	}
//	printf("printing");
	char* var = words[1];
	return printVarValue(var);
}

int runScript(char* words[]) {
	if(wordNumber != 2) {
		return 2;
	}
	//char* line = NULL; // was char line[1000];
	char line[1000];
	FILE *p = fopen(words[1], "r"); // the program is in a file
	if(p == NULL) {
		return -3; // triggers error message: script not found
	}
	runningScript = 1;
	fgets(line, 1000, p);
	while(!feof(p)) {
//		printf("still reading file");
		errorCode = parse(line); // calls interpreter()
		if(errorCode != 0) {
			fclose(p);
			runningScript = 0;
			return errorCode;
		}
		fgets(line, 1000, p);
	}
	fclose(p);
	runningScript = 0;
	return errorCode;
}
int interpreter(char* words[], int numWords) { // again not specifying the size of the words array so tht the fn can accept several sizes
	wordNumber = numWords;
	if (!strcmp(words[0], "quit")) errorCode = quit();
	else if (!strcmp(words[0], "help")) errorCode = help();
	else if (!strcmp(words[0], "set")) errorCode = setVar(words);
	else if (!strcmp(words[0], "print")) errorCode = printVar(words);
	else if (!strcmp(words[0], "run")) errorCode = runScript(words);
	else if (!strcmp(words[0], "\0")) errorCode = 0; // case of no command passed, my parsing code replaces the line feed (LF, newline) character by '\0' so do nothing; simply reprint prompt
	else errorCode = 5; // this shld trigger a display of unknown command
	return errorCode;
}
