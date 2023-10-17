#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"main.h"
#include<sys/wait.h>

void run(char *command, char *str, char *environ[]);

/**
 * main - simple shell
 * Return: 0 for succes state;
*/

int main(void)
{
	char *str = NULL, *command, *arg, *str_copy;
	size_t buffer_size = 0;
	ssize_t bytes_read;
	int i;

	while (1)
	{
		/*
		* isatty()
		* returns 1 if fd is an open file descriptor referring to a terminal;
		* otherwise 0 is returned
		*/
		if (isatty(STDIN_FILENO))
			printf("($) ");

		bytes_read = getline(&str, &buffer_size, stdin);

		if (bytes_read == -1)
		{
			/*printf("%u\n", getpid());*/
			/*perror("Error reading input");*/
			break;
		}

		if (str[bytes_read - 1] == '\n')
			str[bytes_read - 1] = '\0';

		if (strcmp(str, "exit") == 0)
			break;

		/* process input*/
		str_copy = strdup(str);

		command = strtok(str_copy, " ");
		if (strcmp(command, "pwd") == 0)
		{
			pwd();
		}
		else if (strcmp(command, "cd") == 0)
		{
			arg = strtok(NULL, " ");
			cd(arg);
		}
		else if (strcmp(command, "env") == 0)
		{
			i = 0;
			while (environ[i])
			{
				printf("%s\n", environ[i]);
				i++;
			}
		}
		else
		{

			run(command, str, environ);

		}
		free(str_copy);
	}
	free(str);
	return (0);
}

/**
 * run - run command using execve
 * @command: file name will run by include path from PATH enviroment
 * or if file exist in same directory or the path of file
 * @str: input string command + arguments
 * @environ: Enviroment variable
 * Return: void
*/
void run(char *command, char *str, char *environ[])
{
	pid_t pid;
	int i;
	char **newargv;

	newargv = get_args(str);

	if (is_command_exist(command, environ) == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{

			if (newargv != NULL)
			{
				/*printf("%s\n", command);*/
				if (execve(command, newargv, environ) == -1)
				{
					perror("Error");
					free(newargv);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			int status;

			waitpid(pid, &status, 0);

			if (newargv != NULL)
			{

				i = 0;
				while (newargv[i] != NULL)
				{
					free(newargv[i]);
					i++;
				}
				free(newargv);
			}
		}
	}
	else
	{
		fprintf(stderr, "Command '%s' not found\n", command);
	}
}
