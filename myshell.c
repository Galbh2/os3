/*
 ============================================================================
 Name        : myshell.c
 Author      : Michal Momika (204200026) & Gel Bem Haim (301732798)
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>


// Array of pointers to the commands
static char* p_commands[16];

#define BUFFERSIZE 256

/**
 * Parse an array of words to an array of commands
 */
char** toArrayOfArrays(char* input, int size);

/**
 * Gets array of commands, and execute each command
 */
void executeEachCmd(char** commands, int size);

/**
 * Gets a command represented as a string, parses it and executes
 */
int parseAndExecute(char* command);

/**
 * Arguments:
 * 		name - the name of the command
 * 		doInBackground - boolean value which indicates if the cmd should be ran in a new process
 * 		options - two dimensional array of options and values
 * 		argv - a list of arguments
 *
 * this functions execute a given command
 */
int executeCmd(char* argv[], int numOfArgs, int doInBackground);

/**
 * Main loop
 * gets input from the user until he hit 'enter'
 * the loop will break when the user types 'exit'
 */

void runMainLoop();

/**
 * Releases the block of commands from the heap
 */
// void releaseMemory(char** commands);

/**
 * Checks if memory allocation succeed
 */
void checkAllocation(void* p);

/**
 * prints the current directory
 */

void printCurrentDir();

void printCurrentDir() {

	char* currentDir =  getcwd(NULL,0);
		printf("%s> ",currentDir);
		free(currentDir);
}

void checkAllocation(void* p) {
	if (!p) {
			fprintf(stderr, "Error in memory allocation");
			exit(1);
		}
}


// IMPLEMENTATION

void runMainLoop(){

	char c;
	int bufferSize = BUFFERSIZE;
	int positions = 0;
	char* buffer = malloc(sizeof(char) * bufferSize);

	checkAllocation(buffer);

	printf("/> ");

	while (1) {

		c = getchar();
		if (c =='\n') {

			// close the string
			buffer[positions] = '\0';

			// start the chain of execution

			toArrayOfArrays(buffer, positions);

			printCurrentDir();

			// cleanup
			free(buffer);
			positions = 0;
			bufferSize = BUFFERSIZE;
			buffer = malloc(sizeof(char) * bufferSize);
			checkAllocation(buffer);

		}
		else {
			buffer[positions] = c;
			positions++;
			// we are checking if the buffer is full, if so we increase its size
				if (positions >= bufferSize) {
					bufferSize += BUFFERSIZE;
					buffer = realloc(buffer, bufferSize);
				}

				checkAllocation(buffer);
		}
	}
}

char** toArrayOfArrays(char* input, int size) {

	int outerIndex = 0;
	char* p1;
	char* p2;

	p1 = input;
	p2 = input + 1;

	p_commands[outerIndex] = p1;

	while (*p2 != '\0') {

		if ( *p1 == '&' && *p2 == '&' ) {
			*p1 = '\0';
			p1 += 2;
			p2 += 2;
			outerIndex++;
			p_commands[outerIndex] = p1;

		} else {
			p1++;
			p2++;
		}
	}
	// add one to complete the number of words
	executeEachCmd(p_commands, outerIndex + 1);
	return p_commands;
}

void executeEachCmd(char** commands, int size) {

	int i;

	for (i = 0; i < size; i++) {
		parseAndExecute(commands[i]);
	}
}

int parseAndExecute(char* command) {

	char* p_position = command;
	int numOfWords = 1;
	int runInBack = 0;
	char** p_args_index;

	int len = strlen(command);

	// Checks if the program has to run in background or not (&)
	if (command[len - 1] == '&') {
		runInBack = 5;
		command[len - 1] = '\0';
	}

	// Counts the number of words (without &)
	while(*p_position != '\0') {

		if (*p_position == ' '){
			numOfWords++;
		}
		p_position++;
	}

	// Allocate memory for the arguments
	char** p_args = malloc(sizeof(char*) * numOfWords + 1);
	checkAllocation(p_args);

	p_args_index = p_args;

	// extract the name of the command
	*p_args_index = strtok (command," ");
	p_args_index++;

	// builds the argument array
	while ( (*p_args_index = strtok (NULL," ")) != NULL) {
		p_args_index++;
	}

	return executeCmd(p_args, numOfWords - 1, runInBack);

}

int executeCmd(char* argv[], int numOfArgs, int doInBackground) {

	// run chdir and exit
	char* name = argv[0];
	int returnCode;

	if (!strcmp(name, "cd") && (numOfArgs == 1)) {
		if ( (returnCode = chdir(argv[1])) != 0) {
			perror("Error while cd");
		}
		return returnCode;
	} else if (!strcmp(name, "exit")) {
		exit(0);
	}

	int status;
	pid_t childPid = fork();

	if (!childPid) {
		if (execvp(name, argv) == -1) {
			printf("Error: cannot start program %s\n", name);
		}
	} else {
		if (!doInBackground) {
			waitpid(childPid, &status, 0);
		}
	}

	return 1;
}

int main(int argc, char *argv[], char *envp[]){
	runMainLoop();
	return 0;
}
