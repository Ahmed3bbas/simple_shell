#ifndef SHELLHEADER
#define SHELLHEADER
extern char **environ;
void pwd(void);
void cd(char *arg);
char **get_args(char *str);
int number_of_args(char *str);
char **get_env_paths();
int is_command_exist(char *filename, char **environ);
int _getline(char *lineptr, size_t n, int stream);
int  _setenv(char **env);
int  _unsetenv(char **env);
#endif
