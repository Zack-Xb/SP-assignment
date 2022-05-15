//
// Created by wave_ceo on 11/04/2022.
//
#include <stdio.h>
#include "tshell_func.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

void CWD()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

void fork_exec(char **cmp) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (execvp(cmp[0], cmp)) {
            perror("execvp() failed");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
        return;
    }
}

//void print_tokens(char ** t,int tc){
  //  printf("\ntokens:[");
   // for(int i=0;i<=tc;i++){
     //   printf("[%s]",t[i]);
    //}
//}

int tokc(const char *buff){
    int count =0, inword=0;
    do {
        switch (*buff) {
            case '\0':
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                if(inword){
                    inword=0;
                    count++;
                }
                break;
            default:
                inword=1;

        }
    }while (*buff++ != 0);
    return count;
}
//char **tokenize(char *buff,char *sep,int tokc){

//    char **tokens= (char **)calloc(tokc +1, sizeof (char *));

  //  tokens[tokc]=NULL;

    //char *p=strtok(buff,sep);
   // int i=0;
   // while(p!=NULL){
     //   tokens[i++] = p;
       // p= strtok(NULL, sep);
    //}


    //return tokens;
//}


void fork_exec_pipe (char ** args,int pipeN, int *fileIn, int *fileOut,bool async ) {


    int c_count=pipeN;
    int pipe_count=c_count-1;
   int fd[pipe_count *2],
   *current_fd=fd,
   *previous_fd=NULL;




 //  for(int stage= 0; stage<c_count;stage++){
   //    printf("%s\n",args[stage]);
    //}

   for(int stage= 0; stage<c_count;stage++) {
       previous_fd = current_fd - 2;
       int ct = 1;

       char *arg_buffer = malloc(strlen(args[stage]) + 1);
       char *argv[MAXLIST];

       strcpy(arg_buffer, args[stage]);
       argv[0] = strtok(arg_buffer, " ");
       for (int token = 1; (argv[token] = strtok(NULL, " ")) != NULL; token++) {

           ct++;
       }
       //  argv[ct+1]=NULL;
     //  for (int j = 0; j < ct; j++) {
       //    printf("%s\n", argv[j]);
       //}

       if (stage < pipe_count)
           pipe(current_fd);

       pid_t pid = fork();
       if (pid == -1) {
           perror("Fork failed!");
           exit(EXIT_FAILURE);
       } else if (pid == 0) {
           if (stage < c_count - 1) {
               close(current_fd[0]);
               dup2(current_fd[1], STDOUT_FILENO);
               close(current_fd[1]);
           }
           if (stage > 0) {
               close(previous_fd[1]);
               dup2(previous_fd[0], STDIN_FILENO);
               close(previous_fd[0]);
           }
           if (execvp(argv[0], argv) == -1) {
               perror("Exec failed!");
               exit(EXIT_FAILURE);
           }
           exit(EXIT_SUCCESS);
       }
       if (stage >= 1) {
           close(previous_fd[0]);
           close(previous_fd[1]);
       }

       current_fd += 2;
       wait(NULL);
   }
     wait(NULL);
}

void PipedTwo(char** parsed, char** parsedP)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0) {

        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
        wait(NULL);
    } else {

        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }


        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedP[0], parsedP) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
            wait(NULL);
        }
    }
}


//NEW IMPLEMENTATION
int reopen(int fd,char *pNmae, int flags, mode_t mode){
  int o_fd= open(pNmae,flags,mode);
    if(o_fd == fd || o_fd<0)
       return o_fd;

    int dup_fd = dup2(o_fd,fd);
    close(o_fd);

    return dup_fd == -1 ? dup_fd: fd;
}
int redirect_input(char *input){
    printf("Redirecting input from [%s]\n",input);
    return(reopen(STDIN_FILENO,input, O_RDONLY,S_IRUSR));
}
int redirect_output(char *output){
    printf("Redirecting output to [%s]\n",output);
    return(reopen(STDIN_FILENO,output, O_RDWR | S_IRUSR | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH));
}

void oInfo()
{
    puts("\n***WELCOME TO MY SHELL ***"
         "\n-Combining terminal navigation and excellence..."
         "\nList of Commands supported:"
         "\n>cd"
         "\n>ver"
         "\n>exit"
         "\n>cwd "
         "\n>all other general commands available in UNIX shell"
         "\n>pipe handling"
         "\n>improper space handling");


}

char** strpiped;
int parseP(char* str, int tCount)
{
    int i;

    for (i = 0; i < tCount; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
        tCount++;
    }

    if (strpiped[1] == NULL) {
        return 0; // returns zero if no pipe is found.
    }
    else {
        return 1;
    }
}

// function for parsing command words
void parseS(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int BuiltInCMD(char** parsed)
{
    int NoOfOwnCmds = 4, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char * cwd;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "ver";
    ListOfOwnCmds[3] = "cwd";

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
        case 1:
            printf("\nGoodbye\n");
            exit(0);
        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
            oInfo();
            return 1;
        case 4:
          CWD();
            return 1;
        default:
            break;
    }

    return 0;
}

int parseRedirect(char* arg, char** strRedirect ){
    int i;
    for (i = 0; i < 2; i++) {

        strRedirect[i] = strsep(&arg, "<" );
        if (strRedirect[i] == NULL)
            break;
    }

    if (strRedirect[1] == NULL)
        return 0; // returns zero if no redirect is found.
    else {
        return 2;
    }
}
int execArgsRedirect(char** parsed, char** parsedRedirect,int count){
    if(count< 3){
        printf("Invalid number of arguments!\n");
        exit(EXIT_FAILURE);
    }
    pid_t pid=fork();
    if(pid==-1){
        perror("Failed to fork process");
        exit(EXIT_FAILURE);
    }else if(pid==0){
        redirect_input(parsedRedirect[1]);
        redirect_output(parsedRedirect[2]);

        if(execl(parsedRedirect[1],parsedRedirect[1],NULL)==-1){
            perror("Failed to execute the program");
            exit(EXIT_FAILURE);
        }

    }

    return (EXIT_SUCCESS);
}


int processString(char* str, char** parsed, char** parsedPipe,char** parsedRedirect, int tC) {
    char *strRedirect[2];

    int piped;
    int redirect;

    piped = parseP(str, tC);
    redirect = parseRedirect(str,strRedirect);




        if (piped) {
            parseS(strpiped[0], parsed);
            parseS(strpiped[1], parsedPipe);
        } else if(redirect) {
            parseS(strRedirect[0], parsed);
            parseS(strRedirect[1], parsedRedirect);
        }else{
            parseS(str, parsed);
        }


        if (BuiltInCMD(parsed))
            return 0;
        else if(tC==1 || tC==2)
            return 1 ;
        else  if (piped)
        return 1+piped;
        else if (redirect) {
            return 1+redirect;
        }



}