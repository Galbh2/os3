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
		runInBack = 1;
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


	printf("%d\n", runInBack);

	return executeCmd(name, p_args, numOfWords - 1, runInBack);

}

int executeCmd(char* name, char* argv[], int numOfArgs, int doInBackground) {

	printf("%s\n", name);

	int i;

	for (i = 0; i < numOfArgs; i++) {
		printf("%s\n", argv[i]);
	}


	printf("%d in back\n", doInBackground);

	return 0;
}

/*
char *lsh_read_line(void) {
  int buffSize = BUFFSIZE;
  int position = 0;
  int commandIndex = 0;
  char *arrayOfArraysOfChars[16];
  *arrayOfArraysOfChars = malloc(sizeof(char) * buffSize);
  int c;

  if (!arrayOfArraysOfChars) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
    	arrayOfArraysOfChars[position] = '\0';
      return arrayOfArraysOfChars;
    } else if (c == '&' && getChar() == '&') {

    	arrayOfArraysOfChars[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= buffSize) {
      buffSize += BUFFSIZE;
      arrayOfArraysOfChars = realloc(arrayOfArraysOfChars, buffSize);
      if (!arrayOfArraysOfChars) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}
*/

int main(int argc, char *argv[], char *envp[])
{

	runMainLoop();


	/*
	char c;
	int i, fd;
	char *tmp = (char *)malloc(sizeof(char) * 100);
	char *path_str = (char *)malloc(sizeof(char) * 256);
	char *cmd = (char *)malloc(sizeof(char) * 100);

	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);

	copy_envp(envp);
	get_path_string(my_envp, path_str);
	insert_path_str_to_search(path_str);

	if(fork() == 0) {
		execve("/usr/bin/clear", argv, my_envp);
		exit(1);
	} else {
		wait(NULL);
	}
	printf("\ shell> ");
	fflush(stdout);
	while(c != EOF) {
		c = getchar();
		switch(c) {
			case '\n': if(tmp[0] == '\0') {
					   printf("[MY_SHELL ] ");
				   } else {
					   fill_argv(tmp);
					   strncpy(cmd, my_argv[0], strlen(my_argv[0]));
					   strncat(cmd, "\0", 1);
					   if(index(cmd, '/') == NULL) {
						   if(attach_path(cmd) == 0) {
							   call_execve(cmd);
						   } else {
							   printf("%s: command not found\n", cmd);
						   }
					   } else {
						   if((fd = open(cmd, O_RDONLY)) > 0) {
							   close(fd);
							   call_execve(cmd);
						   } else {
							   printf("%s: command not found\n", cmd);
						   }
					   }
					   free_argv();
					   printf("[MY_SHELL ] ");
					   bzero(cmd, 100);
				   }
				   bzero(tmp, 100);
				   break;
			default: strncat(tmp, &c, 1);
				 break;
		}
	}
	free(tmp);
	free(path_str);
	for(i=0;my_envp[i]!=NULL;i++)
		free(my_envp[i]);
	for(i=0;i<10;i++)
		free(search_path[i]);
	printf("\n");
	*/
	printf("hello world");
	return 0;
}
