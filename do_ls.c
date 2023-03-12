#include <dirent.h>
#include <grp.h>  //struct group
#include <pwd.h>  //struct passwd
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>  //extern char *ctime (const time_t *__timer) __THROW;
#include <unistd.h>
//-a、-l、-R、-t、-r、-i、-s
// -a：–all的缩写，显示所有的文件，包括隐藏文件(以.开头的文件)
// -l：列出长数据串，显示出文件的属性与权限等数据信息(常用)
// -t：以修改时间排序
// -r：–reverse，将排序结果以倒序方式显示
// -i：结合-l参数，列出每个文件的inode
// -s: –size 以块大小为单位列出所有文件的大小
// -R: –recursive 同时列出所有子目录层
#define SIZE 1024
typedef struct sum {
    char filename[NAME_MAX];
} SUM;
void mode_to_letters(int mode, char str[]);
char* uid_to_name(uid_t);
char* gid_to_name(gid_t);
void display(char* path);
void sort(SUM* a, int n);
int color(char* path);
void getdir(char* path);
void getfile(char* path);
void show_inode(char* pathname);
void show_info(char* path, char* filename);
void show_block(char* pathname);
void paramAnaly(char* argv, int i);
int do_ls(int argc, char** argv);
int ls_a = 0;
int ls_l = 0;
int ls_R = 0;
int ls_r = 0;
int ls_t = 0;
int ls_i = 0;
int ls_s = 0;
int per_R = 0;
int per_r = 0;
int count = 0;

int do_ls(int argc, char** argv) {
    // 判断-参数
    int cnt = 0;  // -参数组数
    int num = 0;  // 文件名参数个数
    struct stat info;
    char file[SIZE][NAME_MAX];
    char dir[SIZE][NAME_MAX];
    char pathname[PATH_MAX];
    int k = 0,  // dirname
        l = 0;  // filename
    // 目录文件及普通文件个数
    for (int i = 1; i < argc; i++) {  // 读参 存储文件名目录名
        //-参数
        if (argv[i][0] == '-') {
            paramAnaly(argv[i], i);
            cnt++;
        } else {
            stat(argv[i], &info);
            if (info.st_mode & S_IFDIR) {  // 目录文件
                strcpy(dir[k], argv[i]);
                k++;
            } else {  // 普通文件
                strcpy(file[l], argv[i]);
                l++;
            }
        }
    }

    // 排序

    // 展示函数
    if (cnt + 1 == argc) {  // 纯参数 当前文件夹
        display(".");
        // display("/home");
    } else {
        for (int i = 0; i < l; i++) {  // 文件

            display(file[i]);
        }
        for (int i = 0; i < k; i++) {  // 目录文件
            if (i >= 1)
                printf("\n\n");
            if (ls_R != 1)
                printf("%s:\n", dir[i]);

            display(dir[i]);
        }
    }
}

void display(char* path) {  // 路径 及参数
    struct stat info;
    if (stat(path, &info) != 0) {
        printf("ls: 无法访问 '%s': 没有那个文件或目录\n", path);
        exit(1);
    }
    if (info.st_mode & S_IFDIR) {  // 目录文件

        getdir(path);
    } else {
        getfile(path);
    }
}
void getfile(char* filename) {
    if (ls_i == 1) {
        show_inode(filename);
    }
    if (ls_s == 1) {
        show_block(filename);
    }
    if (ls_l == 1) {
        show_info(filename, filename);
    } else {
        printf("\033[%d;1m%s  \033[0m", color(filename), filename);
    }
    per_r = 1;  // 切换
}
void getdir(char path[]) {
    DIR* pdir = opendir(path);
    // 非目录返回上级函数
    if (pdir == NULL) {
        closedir(pdir);
        return;
    }
    char pathname[PATH_MAX];
    SUM* entry = malloc(sizeof(SUM[PATH_MAX * 64]));
    int cnt = 0;  // 总文件名数量

    struct stat info;
    struct dirent* pdirent = NULL;
    // 读取目录下文件->entry[cnt].filename->pathname
    while ((pdirent = readdir(pdir)) != NULL) {
        if ((ls_a != 1) && pdirent->d_name[0] == '.')
            continue;
        strcpy(entry[cnt].filename, pdirent->d_name);
        sprintf(pathname, "%s/%s", path, entry[cnt].filename);
        if (lstat(pathname, &info) == 0)
            count += info.st_blocks / 2;
        cnt++;
    }
    sort(entry, cnt);
    closedir(pdir);
    // 总输出
    if (ls_R == 1) {
        if (per_R == 0)
            printf("%s:\n", path);
        else {
            printf("\n\n%s:\n", path);
        }
    }
    // 目录文件
    if (ls_a == 1 && ls_l == 0) {  //-a
        printf("\033[34m.  ");
        printf("..  \033[0m");
    }
    if (ls_l == 1) {
        printf("总计 %d\n", count);
        count = 0;
    }
    for (int i = 0; i < cnt; i++) {
        sprintf(pathname, "%s/%s", path, entry[i].filename);
        if (entry[i].filename[0] == '.' ||
            !strcmp(entry[i].filename, "..") && ls_l == 0)
            continue;
        if (ls_i == 1) {
            show_inode(pathname);
        }
        if (ls_s == 1) {
            show_block(pathname);
        }

        if (ls_l == 1) {
            show_info(pathname, entry[i].filename);
        } else {
            printf("\033[%d;1m%s  \033[0m", color(pathname), entry[i].filename);
        }
    }
    // 递归

    // 遍历目录
    char path_R[PATH_MAX];
    for (int i = 0; ls_R == 1 && i < cnt; i++) {
        if (!strcmp(entry[i].filename, ".") ||
            !strcmp(entry[i].filename, "..")) {
            // free(entry[i].filename);
            continue;
        }
        sprintf(path_R, "%s/%s\0", path, entry[i].filename);
        if (lstat(pathname, &info) == -1) {
            continue;
        }
        // printf("%s\n", path_R);
        DIR* dirp = opendir(path_R);
        // 非目录返回上级函数
        if (info.st_mode & S_IFDIR) {
            per_R = 1;  // 第一次递归

            getdir(path_R);
        }
    }
    free(entry);
    entry = NULL;
}

void show_info(char* path, char* filename) {
    struct stat info;
    if (lstat(path, &info) == 0) {
        char modestr[11];
        mode_to_letters(info.st_mode, modestr);
        printf("%s", modestr);
        printf("%8d ", (int)info.st_nlink);
        printf("%-8s ", uid_to_name(info.st_uid));
        printf("%-8s ", gid_to_name(info.st_gid));
        printf("%8ld ", (long)info.st_size);
        printf("%.12s ", 4 + ctime(&info.st_mtime));
        printf("\033[%d;1m%s\033[0m\n", color(path), filename);
    }
}

void paramAnaly(char* argv, int i) {
    for (int j = 1; j < strlen(argv); j++) {
        switch (argv[j]) {
            case 'a':
                ls_a = 1;
                break;
            case 'l':
                ls_l = 1;
                break;
            case 'R':
                ls_R = 1;
                break;
            case 'r':
                ls_r = 1;
                break;
            case 't':
                ls_t = 1;
                break;
            case 'i':
                ls_i = 1;
                break;
            case 's':
                ls_s = 1;
                break;
            default:
                printf("ls: 不适用的选项 -- %c\n", argv[j]);
                printf("请尝试执行 \"ls --help\" 来获取更多信息。\n");
                exit(0);
        }
    }
}

char* gid_to_name(gid_t gid) {
    struct group* grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else
        return grp_ptr->gr_name;
}
char* uid_to_name(uid_t uid) {
    struct passwd* pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else
        return pw_ptr->pw_name;
}
int color(char* path) {
    struct stat st;
    lstat(path, &st);
    if (S_ISDIR(st.st_mode)) {
        return 34;
    } else if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR)) {
        return 32;
    } else if (S_ISLNK(st.st_mode)) {
        return 36;
    } else if (S_ISCHR(st.st_mode)) {
        return 33;
    }
    return 37;
}
void mode_to_letters(int mode, char str[]) {
    strcpy(str, "----------");
    if (S_ISFIFO(mode))
        str[0] = 'p';
    if (S_ISSOCK(mode))
        str[0] = 's';
    if (S_ISDIR(mode))
        str[0] = 'd';
    if (S_ISCHR(mode))
        str[0] = 'c';
    if (S_ISBLK(mode))
        str[0] = 'b';
    if (mode & S_IRUSR)
        str[1] = 'r';
    if (mode & S_IWUSR)
        str[2] = 'w';
    if (mode & S_IXUSR)
        str[3] = 'x';
    if (mode & S_IRGRP)
        str[4] = 'r';
    if (mode & S_IWGRP)
        str[5] = 'w';
    if (mode & S_IXGRP)
        str[6] = 'x';
    if (mode & S_IROTH)
        str[7] = 'r';
    if (mode & S_IWOTH)
        str[8] = 'w';
    if (mode & S_IXOTH)
        str[9] = 'x';
}
void sort(SUM* a, int n) {
    char temp[PATH_MAX];
    if (ls_t == 1) {
        struct stat info1;
        struct stat info2;
        if (ls_r == 1) {
            for (int i = 0; i < n - 1; i++) {
                for (int j = i + 1; j < n; j++) {
                    stat(a[i].filename, &info1);
                    stat(a[j].filename, &info2);
                    if (info1.st_mtim.tv_sec > info2.st_mtim.tv_sec) {
                        strcpy(temp, a[i].filename);
                        strcpy(a[i].filename, a[j].filename);
                        strcpy(a[j].filename, temp);
                    }
                }
            }
            return;
        }
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                stat(a[i].filename, &info1);
                stat(a[j].filename, &info2);
                if (info1.st_mtim.tv_sec < info2.st_mtim.tv_sec) {
                    strcpy(temp, a[i].filename);
                    strcpy(a[i].filename, a[j].filename);
                    strcpy(a[j].filename, temp);
                }
            }
        }
        return;
    }
    if (ls_r == 1) {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (strcmp(a[i].filename, a[j].filename) < 0) {
                    strcpy(temp, a[i].filename);
                    strcpy(a[i].filename, a[j].filename);
                    strcpy(a[j].filename, temp);
                }
            }
        }
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(a[i].filename, a[j].filename) > 0) {
                strcpy(temp, a[i].filename);
                strcpy(a[i].filename, a[j].filename);
                strcpy(a[j].filename, temp);
            }
        }
    }
    return;
}
void show_block(char* pathname) {
    struct stat info;
    stat(pathname, &info);
    printf("%8d ", info.st_blocks / 2);
}
void show_inode(char* pathname) {
    struct stat info;
    lstat(pathname, &info);
    printf("%-8d ", info.st_ino);
}