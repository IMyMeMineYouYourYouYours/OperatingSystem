#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#define BUF 1024
int main(int argc, char* argv[]){
    char line[BUF];//입력 받은 저장소
    char *temp[100];
    int pid,num;
    FILE *fp;
    if(argc>1)//batch mode
        fp=fopen(argv[1],"r");
    else//interactive mode
        fp=stdin;
    while(1){
        if(argc==1)
            printf("prompt> ");
        if(!fgets(line,BUF,fp)){
            break;
        }
        if(argc>1)
            printf("%s",line);
        size_t length=strlen(line);
        if(line[length-1]=='\n')
            line[length-1]='\0';
        if(strcmp(line,"quit")==0)
            break;
        char *token;
        int i=0;
        token=strtok(line,";");
       
        while(token!=NULL){//;기준으로 파싱을 함
            temp[i]=token;
            token=strtok(NULL,";");
            i++;
        }
        num=i;
            char *token1;
            char *progpath[20][20];//실행 시킬 프로그램 저장소
        for(i=0;i<num;i++){
            int j=0;
            token1=strtok(temp[i]," ");
            while(token1!=NULL){//" " 기준으로 파싱을 함
                progpath[i][j]=token1;
                token1=strtok(NULL," ");
                j++;
            }
            progpath[i][j]=NULL;
        }
        int check=0;
        for(i=0;i<num;i++){//fork
            if(!strcmp("quit",progpath[i][0]))// 명령어 실행중 quit이 있는지 확인
                check=1;
            if(check)
                break;
            pid=fork();
            if(pid==0){
                execvp(progpath[i][0],progpath[i]);
                exit(0);
            }else if(pid>0){
                wait(NULL);
            }else{
                perror("fork is not executed!!\n");
            }
        }
        if(check)
            break;
    }
}
