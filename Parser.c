//
// Created by wave_ceo on 11/05/2022.
//
#include <stdio.h>
#include "tshell_func.c"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>



void pipeline_tokenizer(char *tbParsed,int c){
    int pCount=1;
        //printf("%s\n",tbParsed);

   char *piped[100];
    piped[0] = strtok(tbParsed, "|");
    for(int token =1; (piped[token]=strtok(NULL,"|"))!=NULL;token++){
    //for (int i = 0; i < c+1; i++) {
      //  if(piped[i]==NULL){
        //    i--;
        //}

        pCount++;
    }
   // printf("%d\n",pCount);
   //for (int i=0;i<pCount;i++){
     //   printf("%s\n",piped[i]);
    //}
   int *FIn=NULL;
   int *Fout=NULL;
   bool block=false;
    fork_exec_pipe(piped,pCount,FIn,Fout,block);
    wait(NULL);
  free(tbParsed);
    //return *piped;
    //if (piped[1] == NULL) {
      //  return NULL; // returns zero if no pipe is found.
    //}else{
      //  return *piped;
   // }

}
void CheckParsed(char *s,char** parsed,int Tcount){
    for (int i=0;i<Tcount;i++){
       printf("%s\n",parsed[i]);
    }
    if (strcmp(parsed[0],">")==0 || strcmp(parsed[0],"<")==0 || strcmp(parsed[0],"|")==0 || strcmp(parsed[0],">>")==0){
        printf("error:command not of an executable type");

    }else{
        if(Tcount==1 ||Tcount==2){
            if(BuiltInCMD(parsed)==0){
                fork_exec(parsed);
                wait(NULL);
            }
        }else{
            pipeline_tokenizer(s,Tcount);
        }

    }
}
