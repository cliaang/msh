#include "mshell.h"

int main(void){

    buf = malloc(sizeof(char) * (MAXLINE+1));
    params = malloc(sizeof(char*) * (MAXARG+2));

    int paraNum = read_command(params);
    struct parse_info info;
    
    parsing(params, paraNum,&info);
    return 0;
}

 
