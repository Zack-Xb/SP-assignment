#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "linenoise.h"
#include <string.h>
#include "linenoise.c"
#include "tokenizer.c"

void completion(const char *buf, linenoiseCompletions *lc) {
    if (buf[0] == 'h') {
        linenoiseAddCompletion(lc,"hello");
        linenoiseAddCompletion(lc,"hello there");
    }
}

char *hints(const char *buf, int *color, int *bold) {
    if (!strcasecmp(buf,"hello")) {
        *color = 35;
        *bold = 0;
        return " World";
    }
    return NULL;
}

int main() {
    char *line, *parsedArgs[MAXLIST];

    linenoiseSetCompletionCallback(completion);
    linenoiseSetHintsCallback(hints);


    linenoiseHistoryLoad("history.txt");



    while((line = linenoise("waveCeo$ > ")) != NULL) {

        if (line[0] != '\0' && line[0] != '/') {

            linenoiseHistoryAdd(line);
            linenoiseHistorySave("history.txt");

            int token_count = tokc(line);
            printf("\ntokc: [%d]\n", token_count);
            //char **tokens = tokenize(line, " ", token_count);

            //print_tokens(tokens, token_count);

            //if (!strcmp("exit", tokens[0]))
              //  break;
            //else
              //  fork_exec_pipe(tokens);
            tokenizer(line,parsedArgs,token_count);
            wait(NULL);
           // execFlag= processString(line,parsedArgs,parsedArgsPiped,parsedArgsRedirected,token_count);

            //if(execFlag==1)
              //  fork_exec(parsedArgs);
           // wait(NULL);
            //if(execFlag==2)
            //    fork_exec_pipe(parsedArgs);
            //wait(NULL);
            //if(execFlag==3)
             //   execArgsRedirect(parsedArgs,parsedArgsRedirected,token_count);
            //wait(NULL);


        } else if (!strncmp(line,"/historylen",11)) {
            int len = atoi(line+11);
            linenoiseHistorySetMaxLen(len);
        } else if (!strncmp(line, "/mask", 5)) {
            linenoiseMaskModeEnable();
        } else if (!strncmp(line, "/unmask", 7)) {
            linenoiseMaskModeDisable();
        } else if (line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }
        free(line);
    }
    return 0;
}
