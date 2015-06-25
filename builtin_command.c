#include "mshell.h"

bool builtin_command(char** params){
    if(!strcmp(params[0],"exit")){
        exit(0);
        return true;
    }
    if(!strcmp(params[0],"about")){
        printf("mshell, clone of bash.\n");
        return true;
    }
    if(!strcmp(params[0],"cd")){
        char* dir = params[1];
        if(dir==NULL){
            chdir(pw->pw_dir);
            return true;
        }

        pw = getpwuid(geteuid());
        if(getcwd(cur_dir, sizeof(cur_dir))==NULL){
            printf("error with getcwd().\n");
            exit(2);
        }
        char dest[1024];
        if(strncmp(dir,"..",2)==0){
            int len = strlen(cur_dir);
            while(cur_dir[len] != '/'){
                len--;
            }
            strncpy(dest,cur_dir,len);
            strcpy(dest+len,dir+2);
        }else if(strncmp(dir,".",1)==0){
            strcpy(dest,cur_dir);
            strcpy(dest+strlen(cur_dir),dir+1);
        }else if(strncmp(dir,"~",1)==0){
            strcpy(dest,pw->pw_dir);
            strcpy(dest+strlen(pw->pw_dir),dir+1);
        }
        chdir(dest);
        return true;
    }


    return false;
}
