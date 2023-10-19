#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/**
 * _getline - read line char by char in \n exist exit
 * @lineptr: buffer used to input
 * @n: size of buffer
 * @stream: int file stream already opened
 * Return: Number of char read or -1 if error
*/
int _getline(char *lineptr, ssize_t n, int stream)
{
	ssize_t readbytes;
	int i = 0;

	while (i < n)
	{
		readbytes = read(stream, lineptr + i, 1);
		if (readbytes < 0)
		{
			perror("Error reading");
			return (-1);
		}
		else if (readbytes == 0)
		{
			break;
		}
		else if (lineptr[i] == '\n' ||  lineptr[i] == EOF)
		{
			i++; /* checked in main.c if the line have only \n */
			break;
		}
		i++;
	}
	return (i);
}
