#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "do_ls.c"
#define SIZE 1024
#define CMDMAX 256
#define ARGMAX 256
void loop();
char* input();
char** cutline(char* string, const char* delim);
void do_command(char** tokens);
void parse(char* token);
int trim(char* strDst, char* strSrc);
int state = 1;
char cmd[CMDMAX][ARGMAX];
int cmdcnt = 0;
int argc = 0;
char** argv;
int main() {
    // loop循环
    loop();
    // 返回程序执行情况
    return 0;
}
void loop() {
    char* string;
    char** token;
    char** tokens = (char**)malloc(sizeof(char*) * SIZE);
    while (stat) {
        printf("[myshell]-> ");
        // 输入
        string = input();
        if (!string)
            continue;
        // 记录cmd命令行
        strcpy(cmd[cmdcnt], string);
        cmdcnt++;
        // 分析
        token = cutline(string, "&&");

        // 去除左右空格
        for (int i = 0; token[i]; i++) {
            tokens[i] = (char*)malloc(sizeof(char) * SIZE);
            trim(tokens[i], token[i]);
        }

        // 执行
        do_command(tokens);

        // free()
        for (int i = 0; tokens[i]; i++)
            free(tokens[i]);
        for (int i = 0; token[i]; i++)
            free(token[i]);
    }
}
char* input() {  // 读取用户输入
    char* token = (char*)malloc(sizeof(char) * BUFSIZ);
    gets(token);
    return token;
}
void parse(char* token) {  // 分析argc,argv
    argv = cutline(token, " ");
}
char** cutline(char* string, const char* delim) {  // 以delim进行分割
    int i = 0;
    char** tokens = (char**)malloc(sizeof(char*) * SIZE);
    char* token;

    /* 获取第一个子字符串 */
    token = strtok(string, delim);
    /* 继续获取其他的子字符串 */
    while (token != NULL) {
        tokens[i] = (char*)malloc(sizeof(char) * SIZE);
        strcpy(tokens[i], token);
        token = strtok(NULL, delim);
        i++;
    }
    argc = i;
    return tokens;
}
void do_command(char** tokens) {
    int i = 0;
    while (tokens[i]) {
        parse(tokens[i]);
        if (tokens[i][0] == 'l' && tokens[i][1] == 's') {
            printf("do_ls\n");
            /* printf("argc=%d\n", argc);
            for (int i = 0; argv[i]; i++) {
                printf("argv[%d]=%s\n", i, argv[i]);
            } */

            // do_ls(argc, argv);
        } else if (tokens[i][0] == 'c' && tokens[i][1] == 'd') {
            printf("do_cd\n");
            /* printf("argc=%d\n", argc);
            for (int i = 0; argv[i]; i++) {
                printf("argv[%d]=%s\n", i, argv[i]);
            } */

            // do_cd(argc, argv);
        } else if (strcmp(tokens[i], "exit") == 0) {
            exit(0);
        } else {
            printf("zsh: command not found: %s", tokens[i]);
            state = 0;
            return;
        }
        i++;
    }
}

int trim(char* strDst, char* strSrc) {
    int i = 0;
    int j = 0;
    if (!strSrc)
        return;
    else {
        j = strlen(strSrc) - 1;

        while (strSrc[i] == ' ') {
            ++i;
        }

        while (strSrc[j] == ' ') {
            --j;
        }

        strncpy(strDst, strSrc + i, j - i + 1);
        strDst[j - i + 1] = '\0';

        return 0;
    }
}