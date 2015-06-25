#include "mshell.h"
#define MAXPIDTABLE 1024

pid_t BPTable[MAXPIDTABLE]; 

void sig_handle(int sig){
    pid_t pid;
    int i;
    for(i=0;i<MAXPIDTABLE;i++){
        if(BPTable[i] != 0){
            pid = waitpid(BPTable[i],NULL, WNOHANG);
            if(pid > 0){
                printf("process %d exited.\n",pid);
                BPTable[i] = 0;
            } else if(pid < 0){
                if(errno != ECHILD){
                    perror("waitpid error\n");
                }
            }
        }
    }
    return;
}



int main(void){
    pid_t    chdPid1,chdPid2;
    int      status;
    int      paraNum;
    struct parse_info   info;




    buf = malloc(sizeof(char) * (MAXLINE+1));
    params = malloc(sizeof(char*) * (MAXARG+2));

    if(buf==NULL || params==NULL){
        printf("Wshell error:malloc failed.\n");
        return;
    }

    if (signal(SIGINT, sig_handle) == SIG_ERR){
        perror("signal error");
    }

    while (1) {
        int pipe_fd[2], in_fd, out_fd;
        print_prompt();
        
        paraNum = read_command(params);
        if(-1==paraNum){
            continue;
        }
        
        parsing(params, paraNum, &info);

        if(builtin_command(params)){
            continue;
        }
        if (info.flag & IS_PIPED){ /* params2 is not null */
            if (pipe(pipe_fd)<0){
                printf("Mshell erroe:pipe failed.\n");
                exit(0);
            }
        }
        if ((chdPid1 = fork())!=0){ /* parent code here */
            if(info.flag & IS_PIPED){
                if((chdPid2 = fork())==0){ /* child 2 code here */
                    close(pipe_fd[1]);
                    close(fileno(stdin));
                    dup2(pipe_fd[0],fileno(stdin));
                    close(pipe_fd[0]);
                    execvp(info.params2[0],info.params2);
                } else { /* parent code after fork() for child 2 here*/
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    waitpid(chdPid2, &status, 0); /* wait for child 2 */
                }
            }
            if(info.flag & BACKGROUND){
                printf("Child pid:%u\n",chdPid1);
                //  register a background process
                int i;
                for(i = 0;i < MAXPIDTABLE;i++){
                    if(BPTable[i] == 0){
                        BPTable[i] = chdPid1;
                    }
                }
                if(i==MAXPIDTABLE){
                    perror("Too much background processes.\n");
                }
            } else {
                waitpid(chdPid1, &status, 0); /* wait for child 1 */
            }
        } else { /* child 1 code here */
            if(info.flag & IS_PIPED){ /* PIPED */
                if(!(info.flag & OUT_REDIRECT) && !(info.flag & OUT_REDIRECT_APPEND)){
                    /* PIPED BUT NOT OUT_DIRECTED */
                    close(pipe_fd[0]);
                    close(fileno(stdout));
                    dup2(pipe_fd[1], fileno(stdout));
                    close(pipe_fd[1]);
                } else { /* PIPED AND OUT_DIRECTED */
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);  /* send a EOF to command2 */
                    if (info.flag & OUT_REDIRECT){
                        out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    } else {
                        out_fd = open(info.out_file, O_WRONLY|O_APPEND|O_TRUNC, 0666);
                    }
                    close(fileno(stdout));
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
         
            } else { /* NOT PIPED */
                if(info.flag & OUT_REDIRECT){
                    out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    close(fileno(stdout));
                    dup2(out_fd,fileno(stdout));
                    close(out_fd);
                }
                
                if(info.flag & OUT_REDIRECT_APPEND){ 
                    out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666); 
                    dup2(out_fd,fileno(stdout));
                    out_fd = open(info.out_file, O_WRONLY|O_APPEND|O_TRUNC, 0666); 
                    close(fileno(stdout));
                }
            }
            if(info.flag & IN_REDIRECT){
                in_fd = open(info.in_file, O_CREAT|O_RDONLY, 0666);  
                close(fileno(stdin));
                dup2(in_fd, fileno(stdin));
                close(in_fd);
            }
            execvp(params[0],params);
        }
    }
    free(buf);
    free(params); 
    exit(0);     
}
