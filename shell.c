#include <stdio.h>
#include <string.h>
#include "interpreter.h"
#include <stdlib.h>

int parse(char userInput[]) { // did not specify the size of the user input array so that an array of any size can be accepted 
// but we know we will only look thru the given array up till a size of 1000 characters
	char* token;
	char* words[100]; // array of character pointers (ie each array slot contains a string which is a sep word!)
	const char * delim = " "; // needs to be of type const char * or like a char array to be input into the strtok function
	token = strtok(userInput, delim);
	int wordsIndex = 0;
	// walk thru the other tokens
	// fn returns a pointer to the 1st token found in the string; a null ptr is returned if there are no tokens left to retrieve
	while(token != NULL && wordsIndex < 100) {
		token[strcspn(token, "\r\n")] = '\0'; // replacing \r and \n if they are at end of string with the null terminating character
		words[wordsIndex] = strdup(token);
		token = strtok(NULL, delim);
		wordsIndex++;
	}
//	for(int i = 0; i <= wordsIndex; i++) {
//		printf("%s", words[i]);
//	}
//	printf("numwords is %d ", wordsIndex);

/*	char tmp[200];
	char* words[100];
	int a;
	int b;
	int wordsIndex =0;
	for(a = 0; userInput[a] == ' ' && a < 1000; a++);

	while(userInput[a] != '\0' && a < 1000) {
		for(b = 0; userInput[a] != '\0' && userInput[a] != '\n' && userInput[a] == '\r' && userInput[a] != ' ' && a < 1000; a++, b++) {
			tmp[b] = userInput[a];
		}
		tmp[b] = '\0';

		words[wordsIndex] = strdup(tmp);
		a++; wordsIndex++;
	} // w is number of words in the command
	printf("num words is %d", wordsIndex);
	for(int i = 0; i <= wordsIndex; i++) {
		printf("%s", words[i]);
	}
*/
	return interpreter(words, wordsIndex);
}

int main() {

	char prompt[100]= {'$', '\0'}; // modifiable prompt
	char userInput[1000];
	int errorCode = 0;

	char const* welcome_msg = "Welcome to the Christina Riachi shell!";
	char const* record_data = "Version 1.0 Created January 2020";

	printf("%s\n%s", welcome_msg, record_data);

	while(1) {
		printf("\n%s", prompt);
		if(fgets(userInput, 1000, stdin) == NULL) {; // limit input to array size taking into accnt the null terminating character 
			return 0;
		}
		errorCode = parse(userInput);
		if(errorCode == 3) printf("This command requires 2 arguments");
		else if(errorCode == -3) printf("Script not found");
		else if(errorCode == 2) printf("This command requires 1 argument");
		else if(errorCode == 1) printf("This command requires 0 arguments");
		else if(errorCode == -1) printf("Could not display commands.");
		else if(errorCode == -2) printf("Memory is full! Cannot add variable.");
		else if(errorCode == 5) printf("Unknown command.");
		else if(errorCode == -5) printf("Variable does not exist.");
		//if(errorCode != 0) exit(99);
	}










}
