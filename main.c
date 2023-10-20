#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"main.h"
#include<sys/wait.h>

int run(char *command, char *str, char *environ[]);

/**
 * _freeenv - free enviroment variable
 * @env: array of string contains environment vairables
 * Return: void
*/
void _freeenv(char **env)
{
	int i = 0;

	while (env[i])
	{
		free(env[i]);
		i++;
	}
	if (env != NULL)
		free(env);
}
/**
 * main - simple shell
 * Return: 0 for succes state;
*/

int main(void)
{
	char str[7168], *command, *arg, *str_copy, **new_env;
	/*size_t buffer_size = 0;*/
	ssize_t bytes_read;
	int i, j, state = EXIT_SUCCESS;

	i = 0;
	while (environ[i])
		i++;
	new_env  = malloc((i + 10) * sizeof(char *));
	if (new_env == NULL)
	{
		perror("malloc");
		return (1);
	}
	i = 0;
	while (environ[i])
	{
		new_env[i] = strdup(environ[i]);
		i++;
	}
	new_env[i] = NULL;

	while (1)
	{
		/*
		* isatty()
		* returns 1 if fd is an open file descriptor referring to a terminal;
		* otherwise 0 is returned
		*/
		if (isatty(STDIN_FILENO))
		{
			printf("($) ");
		}
		fflush(stdout);

		bytes_read = _getline(str, sizeof(str), STDIN_FILENO);
		/*getline(&str, &buffer_size, stdin);*/

		if (bytes_read == -1)
		{
			perror("Error reading input");
			/*printf("%u\n", getpid());*/
			_freeenv(new_env);
			exit(1);
		}
		else if (bytes_read == 0)
		{
			_freeenv(new_env);
			exit(0);
		}

		if (str[bytes_read - 1] == '\n' && bytes_read == 1)
			continue;
		if (str[bytes_read - 1] == '\n')
			str[bytes_read - 1] = '\0';

		/*Romove spaces*/
		i = 0;
		while (str[i] == ' ')
			i += 1;

		if (str[i] == '\0')
		{
			continue;
		}
		else if (str[0] == ' ')
		{
			/*Move string to the begining of spaces*/
			/*printf("%ld\n", strlen(str));*/
			j = 0;
			while (str[i])
			{
				if (str[i - 1] == str[i] && str[i] == ' ')
				{
					i++;
					continue;
				}
				str[j] = str[i];
				i++;
				j++;
			}
			if (str[j - 1] == ' ')
				str[j - 1] = '\0';
			else
				str[j] = '\0';
			/*printf("%ld\n", strlen(str));*/
		}

		/* process input*/
		str_copy = strdup(str);
		command = strtok(str_copy, " ");


		if (strcmp(command, "exit") == 0)
		{
			/*free(str);*/
			_freeenv(new_env);
			arg = strtok(NULL, " ");
			if (arg == NULL)
			{
				free(str_copy);
				exit(state); /* Exit state 0 for success state*/
			}
			else
			{
				/*// printf("%s\n", arg);*/
				state = atoi(arg);
				/*// printf("%d\n", state);*/
				if (*arg == '0' && state == 0)
				{
					free(str_copy);
					exit(state);
				}
				else if (state < 0)
				{
					fprintf(stderr, "exit: Illegal number:%d\n", state);
					free(str_copy);
					state = 2;
					exit(state);
				}
				else if (state == 0)
				{
					fprintf(stderr, "exit: Illegal number:%s\n", arg);
					free(str_copy);
					state = 2;
					exit(state);
				}
				else
				{
					free(str_copy);
					exit(state);

				}
			}
		}
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
			while (new_env[i])
			{
				printf("%s\n", new_env[i]);
				i++;
			}
		}
		else if (strcmp(command, "setenv") == 0)
		{
			state = _setenv(new_env);
		}
		else if (strcmp(command, "unsetenv") == 0)
		{
			state = _unsetenv(new_env);
		}
		else
		{
			state = run(command, str, new_env);
		}
		free(str_copy);
	}
	printf("asdasdas");
	_freeenv(new_env);
	/*free(str);*/
	return (0);
}

/**
 * run - run command using execve
 * @command: file name will run by include path from PATH enviroment
 * or if file exist in same directory or the path of file
 * @str: input string command + arguments
 * @environ: Enviroment variable
 * Return: exit status
*/
int run(char *command, char *str, char *environ[])
{
	pid_t pid;
	int i;
	char **newargv;
	char *command_path;
	const int MAX_PATH_LENGTH = 256;
	int exit_status = 0;

	newargv = get_args(str);
	command_path = malloc(sizeof(char) * MAX_PATH_LENGTH);
	strcpy(command_path, command);

	if (is_command_exist(command_path, environ) == 0)
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
				/*printf("%s\n", command_path);*/
				if (execve(command_path, newargv, environ) == -1)
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
			/*printf("%d", status);*/

			if (WIFEXITED(status))
			{
				exit_status = WEXITSTATUS(status);
			}

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
			free(command_path);
			return (exit_status);
		}
	}
	else
	{
		fprintf(stderr, "Command '%s' not found\n", command_path);
		free(command_path);
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
		return (127); /* Exit status for not found command*/
	}
	return (exit_status);

}
