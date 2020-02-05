// Implement the shell memory as an array of struct, not as a linked list
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEMSIZE 1000

typedef struct MEM {
	char *var;
	char *value;
} shellMem;

shellMem memArray[MEMSIZE];
int nextEmptySlot = 0;

// to compare the current variable name in the memory to the variable name we are looking for 
// returns an int value indicating if this var already exists, not for the purposes of error code!
int match(char* arrayVar, char* varToFind) {
	int i;
	int matchedChars = 0;
	int varLen = strlen(varToFind);
	if(varLen != strlen(arrayVar)) return 0; // these 2 variable names are not a match
	for(i = 0; i<varLen; i++) {
		if(*(arrayVar+i) == *(varToFind + i)) matchedChars++;
	}
	if(matchedChars == varLen) return 1;
	return 0; // if the strings are not a perfect match
}

// to create a new variable and assign it the given name and value
int addVariable(char *varName, char* varValue) {
	if(nextEmptySlot == MEMSIZE) { return -2; } // this is meant to be error code; handle it depending on its specific negative value and either terminate the shell prog or display error msg and prompt agn.. come back to this
	else  {
		memArray[nextEmptySlot].var = strdup(varName);
		memArray[nextEmptySlot].value = strdup(varValue);
		nextEmptySlot++;
	}
	//TESTING CODE
//	int j;
//	for(j = 0; j < nextEmptySlot; j++) {
//		printf("slot %d var: %s value: %s ", j, memArray[j].var, memArray[j].value);
//	}
	return 0; // successfully added variable
}

int setVarValue(char* varName, char* varValue) {
	int i;
	int found = 0;
	for(i = 0; i < nextEmptySlot; i++) {
		found = match(memArray[i].var, varName);
		if(found) {
			memArray[i].var = strdup(varName);
			memArray[i].value = strdup(varValue);
	//TESTING CODE
//	int j;
//	for(j = 0; j < nextEmptySlot; j++) {
//		printf("in found slot %d var: %s value: %s ", j, memArray[j].var, memArray[j].value);
//	}

			return 0;
		}
	}
        return addVariable(varName, varValue);
}

int printVarValue(char* varName) {
	int i;
	int found = 0;
	for(i = 0; i < nextEmptySlot; i++) {
		found = match(memArray[i].var, varName);
		if(found) {
			printf("%s", memArray[i].value);
			return 0;
		}
	}
	return -5; // corresps to printing "Variable does not exist"
}
