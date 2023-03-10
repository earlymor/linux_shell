#include <stdio.h>
void loop();
char* input();
char** purse(char* string);
void do_command(char** tokens);
int main() {
    // loop循环
    loop();
    
    return 0;
}
void loop() {
    char* string;
    char** tokens;
    while (1) {
        printf("[myshell]-> ");
        // 输入
        string=input();
        // 分析
        tokens=parse(string);
        // 执行
        do_command(tokens);
    }
}
char* input(){

}
char** purse(char* string){

}
void do_command(char** tokens){

}
