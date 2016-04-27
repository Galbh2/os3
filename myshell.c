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

static char *my_argv[16], *my_envp[16];
static char *search_path[16];

#define BUFFSIZE 4096

/**
 * Parse an array of words to an array of commands
 */
char** toArrayOfArrays(char* argv[]);

/**
 * Gets array of commands, and execute each command
 */
void executeEachCmd(char** commands);

/**
 * Gets a command represented as a string, parses it and executes
 */
int parseAndExecute(char* command);

/**
 * Arguments:
 * 		name - the name of the command
 * 		doInBackground - boolean value which indicates if the cmd should be ran in a new process
 * 		options - two dimensional array of optiotns and values
 * 		argv - a list of arguments
 *
 * this functions execute a given command
 */
int executeCmd(char* name, int doInBackground, char* options[][], char* argv[]);




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
