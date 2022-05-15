//
// Created by wave_ceo on 11/04/2022.
//

#ifndef SP_ASSIGNMENT_TSHELL_FUNC_H
#define SP_ASSIGNMENT_TSHELL_FUNC_H

typedef  struct {
    int execNuM;
    char** command;

}ReturnType;


void fork_exec(char **cmp);
void print_tokens(char ** t,int tc);
int tokc(const char *buff);
char **tokenize(char *buff,char *sep,int tokc);
void fork_exec_pipe (
        char ** args ,int n,int *f,int* fo, bool a) ;
void oInfo();
int parseP(char* str, int tCount);
void parseS(char* str, char** parsed);
int BuiltInCMD(char** parsed);
int processString(char* str, char** parsed, char** parsedpipe,char** parsedRedirect,int tC);
void PipedTwo(char** parsed, char** parsedpipe);
int execArgsRedirect(char** parsed, char** parsedRedirect,int count);

#endif //SP_ASSIGNMENT_TSHELL_FUNC_H
