#include "mshell.h"

int read_command(char **params){
    memset(buf,'\0',MAXLINE+1);    
    int count = 0;
    if (fgets(buf, MAXLINE, stdin) == NULL){
        printf("unknown input, print \"exit\" to exit.\n");
        return -1;
    } else {
        if (buf[strlen(buf)-1] == '\n'){
            buf[strlen(buf)-1] = 0; /* replace newline with null */
        }
        
        const char delim = ' ';
        char* tmp       = buf;

        bool start = true;
        for (tmp=buf;*tmp;tmp++){
            if(start){
                if(*tmp==' '){
                    continue;
                } else {
                    params[count] = tmp;
                    count++;
                    start = false;
                }
            } else {
                if(*tmp == ' '){
                    *tmp = '\0';
                    start = true;
                } else {
                    continue;
                }
            }
        }         
    }
    params[count] = NULL;
    return count;   
}
