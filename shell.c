#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024
void loop();
char* input();
char** cutline(char* string, const char* delim);
void do_command(char** tokens);
char** parse(char* string, const char* delim);
int trim(char* strDst, char* strSrc);
int stat = 1;
int main() {
    // loop循环
    loop();
    //返回程序执行情况
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
char** parse(char* string, const char* delim) {  // 删掉左右空格
    int i = 0;
    char** tokens = (char**)malloc(sizeof(char*) * SIZE);
    char* token;
    /* 获取第一个子字符串 */
    token = strtok(string, delim);
    tokens[i] = token;
    /* 继续获取其他的子字符串 */
    while (token != NULL) {
        // printf("%s\n", tokens[i]);
        i++;
        token = strtok(NULL, " ");
        tokens[i] = token;
    }
    return tokens;
}
char** cutline(char* string, const char* delim) {  // 以" "进行分割
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
    return tokens;
}
void do_command(char** tokens) {
    int i = 0;
    while (tokens[i]) {
        if (strcmp(tokens[i], "ls") == 0) {
            printf("do_ls\n");
            // do_ls();
        } else if (strcmp(tokens[i], "cd") == 0) {
            printf("do_cd\n");
            // do_cd();
        } else if (strcmp(tokens, "exit") == 0) {
            exit(0);
        } else {
            printf("zsh: command not found: %s", tokens[i]);
            stat = 0;
            return;
        }
        i++;
    }
}

int trim(char* strDst, char* strSrc) {
    int i = 0;
    int j = 0;

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