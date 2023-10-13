#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

/**
 * number_of_args - get number of argument
 * @str: input string (command + args)
 * Return: 0 if str NULL
 * or number of arguments
*/
int number_of_args(const char *str)
{
	int i = 0;
	char *token;
	char *str_copy = strdup(str);

	if (str_copy == NULL)
		return (0);

	token = strtok(str_copy, " ");
	while (token != NULL)
	{
		i++;
		token = strtok(NULL, " ");
	}

	free(str_copy);
	return (i);
}

/**
 * get_args - get arguments in array
 * @str: input string (command + args)
 * Return: array of srings (arguments) or NULL
*/
char **get_args(char *str)
{
	char *str_copy, *token, **args;
	int i = 0, numofargs, j;

	if (str == NULL)
		return (NULL);

	str_copy = strdup(str);
	if (str_copy == NULL)
		return (NULL);

	numofargs = number_of_args(str_copy);
	if (numofargs == 0)
	{
		free(str_copy);
		return (NULL);
	}
	args = malloc(sizeof(char *) * (numofargs + 1));
	if (args == NULL)
	{
		free(str_copy);
		return (NULL);
	}
	token = strtok(str_copy, " ");
	while (token != NULL && i < numofargs)
	{
		args[i] = strdup(token);
		if (args[i] == NULL)
		{
			for (j = 0; j < i; j++)
				free(args[j]);
			free(args);
			free(str_copy);
			return (NULL);
		}
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;
	free(str_copy);
	return (args);
}

/**
 * get_arr_paths - covert path from string to array of strings
 * @sizeofpaths: number of paths in string
 * @paths: string contains all paths
 * Return: array of srings (paths) or NULL
*/
char **get_arr_paths(int sizeofpaths, char *paths)
{
	char **arrpath, *str_copy1, *tkn;
	int i, j;

	arrpath = malloc(sizeof(char *) * (sizeofpaths + 1));
	if (arrpath == NULL)
	{
		return (NULL);
	}

	str_copy1 = strdup(paths);
	if (str_copy1 == NULL)
		free(arrpath);
	tkn = strtok(str_copy1, ":");
	i = 0;
	while (tkn != NULL && i < sizeofpaths)
	{
		arrpath[i] = strdup(tkn);
		if (arrpath[i] == NULL)
		{
			for (j = 0; j < i; j++)
			{
				free(arrpath[j]);
			}
			free(arrpath);
			free(str_copy1);
			return (NULL);
		}
		tkn = strtok(NULL, ":");
		i++;
	}
	arrpath[i] = NULL;
	free(str_copy1);
	return (arrpath);
}
/**
 * get_env_paths - get path in enviroment
 * @environ: contains all enviroment variables (key = val)
 * Return: array of srings (arguments) or NULL
*/
char **get_env_paths(char **environ)
{
	char *paths = NULL;
	int i = 0;
	char *tkn, *str_copy;
	char **arrpath;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			paths = environ[i] + 5; /* skip "PATH=" */
			break;
		}
		i++;
	}

	if (paths == NULL)
		return (NULL);
	str_copy = strdup(paths);
	if (str_copy == NULL)
		return (NULL);
	tkn = strtok(str_copy, ":");
	i = 0; /* Number of pathes */
	while (tkn != NULL)
	{
		i++;
		tkn = strtok(NULL, ":");
	}
	free(str_copy);
	arrpath = get_arr_paths(i, paths);
	return (arrpath);
}
/**
 * is_command_exist - check if command is exist in PATH of PATHS in enviroment
 * @filename: commandname or filename
 * @environ: contains all enviroment variables (key = val)
 * Return: 0 exist or 1 NOT
*/
int is_command_exist(char *filename, char **environ)
{
	const int MAX_PATH_LENGTH = 256;

	if (access(filename, F_OK) != -1)
	{
		return (0);
	}
	else
	{
		char *concatenated, **paths;
		int i = 0;

		paths = get_env_paths(environ);
		concatenated = malloc(sizeof(char) * MAX_PATH_LENGTH);
		while (paths[i] != NULL)
		{
			if (paths[i][strlen(paths[i]) - 1] != '/')
			{
				snprintf(concatenated, MAX_PATH_LENGTH, "%s/%s", paths[i], filename);
			} else
			{
				snprintf(concatenated, MAX_PATH_LENGTH, "%s%s", paths[i], filename);
			}
			if (access(concatenated, F_OK) != -1)
			{
				strcpy(filename, concatenated);
				for (; paths[i] != NULL; i++)/* complete free for paths allcotions*/
					free(paths[i]);
				free(paths);
				free(concatenated);
				return (0);
			}
			free(paths[i]);
			i++;
		}
		free(paths);
		free(concatenated);
	}
	return (-1);
}
