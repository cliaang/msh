#include "mshell.h"

int parse_info_init(struct parse_info *info){
    info->flag = 0;
    info->in_file = NULL;
    info->out_file = NULL;
    info->params2 = NULL;
    return 0;
}

int parsing(char **params, int paraNum, struct parse_info *info){
    int i;
    parse_info_init(info);
    if(strcmp(params[paraNum-1],"&")==0){
        info->flag |= BACKGROUND;
        params[paraNum-1] = NULL;
        paraNum--;
    }
    for(i = 0; i<paraNum;){
        if(strcmp(params[i],"<<") == 0 || strcmp(params[i],"<")==0){
            info->flag |= IN_REDIRECT;
            info->in_file = params[i+1];
            params[i] = NULL;
            i+=2;
        }else if (strcmp(params[i], ">") ==0){
            info->flag |= OUT_REDIRECT;
            info->out_file = params[i+1];
            params[i] = NULL;
            i+=2;
        } else if (strcmp(params[i],">>")==0){
            info->flag |= OUT_REDIRECT_APPEND;
            info->out_file = params[i+1];
            params[i] = NULL;
            i+=2;
        } else if (strcmp(params[i] ,"|")==0){
            info->flag |= IS_PIPED;
            params[i] = NULL;
            info->params2 = &params[i+1];
            break;
        } else {
            i++;
        }
    }

#ifdef DEBUG
        printf("DEBUG INFO\nbackground:%s\n",info->flag&BACKGROUND?"yes":"no");
        printf("in redirect:");
        info->flag&IN_REDIRECT?printf("yes,file:%s\n",info->in_file):printf("no\n");
        printf("out redirect:");
        info->flag&OUT_REDIRECT?printf("yes,file:%s\n",info->out_file):printf("no\n");
        printf("pipe:");
        info->flag&IS_PIPED?printf("yes,command:%s\n",info->params2[0]):printf("no\n");
#endif
        return 1;
}
