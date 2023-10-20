#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include "main.h"

/**
 * pwd - print current working directory
 * Return: void
*/
void pwd(void)
{
	char buf[1024];

	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		printf("%s\n", buf);
	}
	else
	{
		perror("pwd: Error");
	}
}

/**
 * cd - change directory
 * @arg: the path needed to change to
 * Return: void
*/

void cd(char *arg)
{
	char buf[1024];

	if (arg == NULL)
		arg = getenv("HOME");
	else if (strcmp(arg, "-") == 0)
	{
		arg = getenv("OLDPWD");
		if (arg == NULL)
			arg = ".";
	}
	if (chdir(arg) == -1)
	{
		fprintf(stderr, "cd: %s: No such file or directory\n", arg);
		return;
	}

	getcwd(buf, sizeof(buf));
	if (strcmp(getenv("PWD"), buf) != 0)
	{
		if (setenv("OLDPWD", getenv("PWD"), 1) != 0)
		{
			perror("Failed to set OLDPWD");
		}
	}
	if (setenv("PWD", buf, 1) != 0)
	{
		perror("Failed to set OLDPWD");
	}
}

/**
 * _setenv - set variable or change it value in enviornment
 * Return: exit state if sucess return 0 else 1
*/
int  _setenv(void)
{
	char *key, *value;

	key = strtok(NULL, " ");
	if (key == NULL)
	{
		perror("You have to pass key and value");
		return (EXIT_FAILURE);
	}

	value = strtok(NULL, " ");
	if (value == NULL)
	{
		perror("You have to pass value");
		return (EXIT_FAILURE);
	}

	if (setenv(key, value, 1) != 0)
	{
		fprintf(stderr, "Failed to set %s", key);
		return (EXIT_FAILURE);
	}
	return (0);
}

/**
 * _unsetenv - unset variable from enviornment
 * Return: exit state if sucess return 0 else 1
*/
int  _unsetenv(void)
{
	char *key;

	key = strtok(NULL, " ");
	if (key == NULL)
	{
		perror("You have to pass key and value");
		return (EXIT_FAILURE);
	}

	if (unsetenv(key) != 0)
	{
		fprintf(stderr, "Failed to unset %s", key);
		return (EXIT_FAILURE);
	}
	return (0);
}
