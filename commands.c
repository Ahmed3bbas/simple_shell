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

	if (strlen(arg) == 0)
		chdir("~");
	else if (chdir(arg) == -1)
		fprintf(stderr, "cd: %s: No such file or directory\n", arg);

	if (setenv("OLDPWD", getenv("PWD"), 1) != 0)
	{
		perror("Failed to set OLDPWD");
	}

	getcwd(buf, sizeof(buf));
	if (setenv("PWD", buf, 1) != 0)
	{
		perror("Failed to set OLDPWD");
	}
}
