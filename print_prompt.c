#include "mshell.h" 
void print_prompt(){
  
    uid_t uid;
    
    char host_name[HOST_NAME_MAX];
    gethostname(host_name, HOST_NAME_MAX);

    uid = geteuid(); 
    pw = getpwuid(uid);    
    if (pw != NULL){
        printf("%s@%s:",pw->pw_name,host_name);
    }
    if (getcwd(cur_dir, sizeof(cur_dir))!=NULL){
        char *home_dir = pw->pw_dir;
        if(strstr(cur_dir, home_dir) != NULL){
            char sub[1024];
            strncpy(sub, cur_dir+strlen(home_dir),strlen(cur_dir)-strlen(home_dir)+1);
            printf("~%s",sub);
        } else {
            printf("%s",cur_dir);
        }    
        if ( geteuid() == 0){
            printf("# ");
        } else {
            printf("& ");
        }
    } else {
        printf("error with getcwd().\n");
        exit(2);
    }
    
}
