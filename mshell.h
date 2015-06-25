#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE 1000
#define MAXARG 50
#define bool int
#define true 1
#define false 0

#define DEBUG

char* buf;
char** params;
char cur_dir[1024];
struct passwd *pw;
struct parse_info;

void print_prompt();
int read_command(char **);
bool builtin_command(char **);
int parsing(char **params, int paraNum, struct parse_info *info);



#ifndef STRUCT_PARSE_INFO
#define STRUCT_PARSE_INFO

#define BACKGROUND          1
#define IN_REDIRECT         2
#define OUT_REDIRECT        4
#define OUT_REDIRECT_APPEND 8
#define IS_PIPED            16

struct parse_info{
    int flag;
    char* in_file;
    char* out_file;
    char** params2;
};
#endif
