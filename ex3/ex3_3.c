#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

void exec_general_mode();
void exec_fifo_mode();
void cmpexecmode(char *optarg);
void recievemsg(int fd, char *buf);

char *file_name;

int main(int argc, char *argv[]) {
    int n;
    extern char *optarg;
    extern int optind;

    file_name = getenv("COM_FILE");

    while ((n = getopt(argc, argv, "t:f:")) != -1) {
        switch (n)
        {
        case 't':
            cmpexecmode(optarg);
            break;
        case 'f':
            file_name = optarg;
            break;
        default:
            printf("unhandled option\n");
            break;
        }
    }

    if (n == -1) {
        cmpexecmode("r");
    }

    return 0;
}

void cmpexecmode(char *optarg) {
    switch (*optarg)
    {
    case 'f':
        exec_fifo_mode();
        break;
    case 'r':
        exec_general_mode();
        break;
    default:
        printf("unhandled option\n");
        break;
    }
}

void exec_general_mode() {
    // ########################################
    // ########################################
    // fd: 파일 디스크립터 번호를 저장하는 변수
    // n : R/W가 진행된 byte를 담고 있는 변수
    // buf : R/W를 위해 자료를 임시적으로 담은 버퍼
    // sb : inode 파일 정보를 구조체의 형태로 저장
    // ########################################
    // ########################################
    int fd, n;
    DIR *dp;
    char buf[BUFSIZ];
    struct stat sb;

    if (file_name == NULL) {
        file_name = "data.txt";
    }
    
    // ########################################
    // ########################################
    // DIR *opendir(const char *dirname);
    // Input: 디렉토리 경로
    // Output: 
    //     - (성공시) DIR 포인터
    //     - (실패시) NULL
    // ----------------------------------------
    // [개요] 현재 프로그램이 위치한 디렉토리 열기
    // ########################################
    // ########################################
    if ((dp = opendir(".")) == NULL) {
        perror("opendir error");
        exit(1);
    }

    // ########################################
    // ########################################
    // struct dirent *readdir(DIR *dirp);
    // Input: 디렉토리 포인터
    // Output: 
    //     - DIR 엔트리
    // ----------------------------------------
    // [개요]
    // dir을 계속 순회하면서 entry를 살펴본다.
    // data.txt가 감지되면 다음 절차로 넘어간다.
    // ########################################
    // ########################################
    struct dirent *dent;
    int flag = 1;
    while (1) {
        while ((dent = readdir(dp))) {
            if (!strcmp(dent->d_name, file_name)) {
                flag = 0;
                break;
            }
        }
        if (flag) {
            rewinddir(dp);
        } else {
            break;
        }
    }
    
    // ########################################
    // ########################################
    // [개요]
    // 파일에 기반한 프로세스 통신이 수행된다.
    // ########################################
    // ########################################
    fd = open(file_name, O_CREAT | O_TRUNC);
    if (fd == -1) {
        perror("Open file");
        exit(1);
    }

    // ########################################
    // ########################################
    // int fstat(int fd, struct stat *buf);
    // Input : file descriptor, stat buf
    // Output : 0 (Success), -1 (Error)
    // ---------------------------------------
    // [개요]
    // 파일의 inode 정보를 가져오기 위한 fstat
    // 실패시 에러 출력 후 프로그램 종료
    // #######################################
    // #######################################
    if (fstat(fd, &sb) == -1) {
        perror("fstat error");
        exit(1);
    }

    // #######################################
    // #######################################
    // POSIX 표준 C MACRO 함수
    // S_ISREG(mode)
    // Input : mode (stat 구조체, 파일 종류 및 권한 정보를 담음)
    // Output : True (주어진 파일이 일반파일일 때!)
    // ---------------------------------------
    // [개요]
    // data.txt가 일반 파일이 아니라면 오류 출력
    // #######################################
    // #######################################
    if (!S_ISREG(sb.st_mode)) {
        perror("Not regular file");
        exit(1);
    }

    // #######################################
    // #######################################
    // 파일 접근 권한 확인 상수
    // S_IRGRP : 그룹 읽기 권한
    // S_IWGRP : 그룹 쓰기 권한
    // S_IROTH : other 읽기 권한
    // S_IWOTH : other 쓰기 권한
    // ---------------------------------------
    // [개요]
    // Group/Other에 대해 읽기 쓰기 권한이 있다면
    // 에러 출력
    // #######################################
    // #######################################
    if (S_IRGRP & sb.st_mode | S_IWGRP & sb.st_mode | 
        S_IROTH & sb.st_mode | S_IWOTH & sb.st_mode) {
        perror("file must be protected");
        exit(1);
    }

    recievemsg(fd, buf);

    // ########################################
    // ########################################
    // [개요]
    // 프로그램이 종료되면 파일디스크립터 사용을
    // 해제한다.
    // ########################################
    // ########################################
    close(fd);
    closedir(dp);
};

void exec_fifo_mode() {
    // ########################################
    // ########################################
    // fd: 파일 디스크립터 번호를 저장하는 변수
    // n : R/W가 진행된 byte를 담고 있는 변수
    // buf : R/W를 위해 자료를 임시적으로 담은 버퍼
    // sb : inode 파일 정보를 구조체의 형태로 저장
    // ########################################
    // ########################################
    int fd, n;
    DIR *dp;
    char buf[BUFSIZ];
    struct stat sb;

    if (file_name == NULL) {
        file_name = "data.fifo";
    }
    
    // ########################################
    // ########################################
    // DIR *opendir(const char *dirname);
    // Input: 디렉토리 경로
    // Output: 
    //     - (성공시) DIR 포인터
    //     - (실패시) NULL
    // ----------------------------------------
    // [개요] 현재 프로그램이 위치한 디렉토리 열기
    // ########################################
    // ########################################
    if ((dp = opendir(".")) == NULL) {
        perror("opendir error");
        exit(1);
    }

    // ########################################
    // ########################################
    // struct dirent *readdir(DIR *dirp);
    // Input: 디렉토리 포인터
    // Output: 
    //     - DIR 엔트리
    // ----------------------------------------
    // [개요]
    // dir을 계속 순회하면서 entry를 살펴본다.
    // data.txt가 감지되면 다음 절차로 넘어간다.
    // ########################################
    // ########################################
    struct dirent *dent;
    int flag = 1;
    while (1) {
        while ((dent = readdir(dp))) {
            if (!strcmp(dent->d_name, file_name)) {
                flag = 0;
                break;
            }
        }
        if (flag) {
            rewinddir(dp);
        } else {
            break;
        }
    }
    
    // ########################################
    // ########################################
    // [개요]
    // 파일에 기반한 프로세스 통신이 수행된다.
    // ########################################
    // ########################################
    fd = open(file_name, O_TRUNC | O_RDONLY);
    if (fd == -1) {
        perror("Open file");
        exit(1);
    }

    // ########################################
    // ########################################
    // int fstat(int fd, struct stat *buf);
    // Input : file descriptor, stat buf
    // Output : 0 (Success), -1 (Error)
    // ---------------------------------------
    // [개요]
    // 파일의 inode 정보를 가져오기 위한 fstat
    // 실패시 에러 출력 후 프로그램 종료
    // #######################################
    // #######################################
    if (fstat(fd, &sb) == -1) {
        perror("fstat error");
        exit(1);
    }

    // #######################################
    // #######################################
    // POSIX 표준 C MACRO 함수
    // S_ISREG(mode)
    // Input : mode (stat 구조체, 파일 종류 및 권한 정보를 담음)
    // Output : True (주어진 파일이 일반파일일 때!)
    // ---------------------------------------
    // [개요]
    // data.txt가 일반 파일이 아니라면 오류 출력
    // #######################################
    // #######################################
    if (!S_ISFIFO(sb.st_mode)) {
        perror("Not FIFO file");
        exit(1);
    }

    // #######################################
    // #######################################
    // 파일 접근 권한 확인 상수
    // S_IRGRP : 그룹 읽기 권한
    // S_IWGRP : 그룹 쓰기 권한
    // S_IROTH : other 읽기 권한
    // S_IWOTH : other 쓰기 권한
    // ---------------------------------------
    // [개요]
    // Group/Other에 대해 읽기 쓰기 권한이 있다면
    // 에러 출력
    // #######################################
    // #######################################
    if (S_IRGRP & sb.st_mode | S_IWGRP & sb.st_mode | 
        S_IROTH & sb.st_mode | S_IWOTH & sb.st_mode) {
        perror("file must be protected");
        exit(1);
    }

    recievemsg(fd, buf);

    // ########################################
    // ########################################
    // [개요]
    // 프로그램이 종료되면 파일디스크립터 사용을
    // 해제한다.
    // ########################################
    // ########################################
    close(fd);
    closedir(dp);
}

void recievemsg(int fd, char *buf) {
    // ########################################
    // ########################################
    // [개요]
    // 파일에 기반한 프로세스 통신이 수행된다.
    // ########################################
    // ########################################
    while (1) {
        int n = read(fd, buf, 255);
        buf[n] = '\0';
        if (n == -1) {
            perror("Read error");
        } else if (n == 0) {
            lseek(fd, 0, SEEK_END);
            continue;
        }
        write(1, "Recv>> ", 7);
        write(1, buf, n);
        if (n == 1 && buf[0] == 'q') {
            write(1, "Terminate\n", 10);
            break;
        }
    }
}