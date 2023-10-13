#include<string.h>
#include<unistd.h>
#include<stdio.h>

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
	if (chdir(arg) != -1)
	{
		pwd();
	}
	else
	{
		perror("cd: Error");
	}
}
