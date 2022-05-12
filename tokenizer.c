//
// Created by wave_ceo on 11/05/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include "Parser.c"

#define MAXLIST 100


void tokenizer(char *str,char **parsed,int count){

    int i;
    char *NmStr= malloc(strlen(str)+1);
    strcpy(NmStr,str);
    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
    printf("%s\n",NmStr);
    parsed[count+1]=NULL;
    CheckParsed(NmStr,parsed,count);

}
