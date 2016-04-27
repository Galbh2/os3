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

// static char *my_argv[16], *my_envp[16];
// static char *search_path[16];

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
int executeCmd(char* name, char* argv[], int numOfArgs, int doInBackground);

/**
 * Main loop
 * gets input from the user until he hit 'enter'
 * the loop will break when the user types 'exit'
 */

void runMainLoop();

/**
 * Releases the block of commands from the heap
 */
void releaseMemory(char** commands);

/**
 * Checks if memory allocation succeed
 */
void checkAllocation(void* p);


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

	printf("Enter Commands\n");

	while (1) {
		c = getchar();
		if (c =='\n') {

			// close the string

			buffer[positions] = '\0';

			toArrayOfArrays(buffer, positions);

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
		// printf("%s\n", 	printf("spaces = %d", numOfSpaces);commands[i]);
		parseAndExecute(commands[i]);
	}
}

int parseAndExecute(char* command) {

	char* token;
	char* p_position = command;
	int numOfWords = 1;
	int runInBack = 0;
	char* name;
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

	char** p_args = malloc(sizeof(char*) * numOfWords);
	checkAllocation(p_args);

	p_args_index = p_args;

	// extract the name of the command
	token = strtok (command," ");
	name = token;

	// builds the argument array
	while ( (token = strtok (NULL," ")) != NULL) {
		*p_args_index = token;
		p_args_index++;
	}

	return executeCmd(name, p_args, numOfWords - 1, runInBack);

}

int executeCmd(char* name, char* argv[], int numOfArgs, int doInBackground) {

	/*
	 printf("%s\n", name);
	 int i;
	 for (i = 0; i < numOfArgs; i++) {
	 printf("%s\n", argv[i]);
	 }
	 printf("%d in back\n", doInBackground);
	 */

	int status;
	pid_t childPid = fork();

	if (!childPid) {
		if (execvp(name, argv) == -1) {
			perror("Command doesn't exist");
		}
	} else {
		if (!doInBackground) {
			printf("block\n");
			waitpid(childPid, &status, 0);
			printf("release block\n");
		}

		return 1;
	}
}

int main(int argc, char *argv[], char *envp[]){
	runMainLoop();
	printf("hello world");
	return 0;
}
