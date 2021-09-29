#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd, n;
    char buf[BUFSIZ];
    // #######################################
    struct stat sb;
    // #######################################
    mode_t mode;

    mode = S_IRUSR | S_IWUSR;

    fd = open("data", O_CREAT | O_WRONLY, mode);
    if (fd == -1) {
        perror("Open data.txt");
        exit(1);
    }

    // #######################################
    // 파일의 inode 정보를 가져오기 위한 fstat
    if (fstat(fd, &sb) == -1) {
        perror("fstat error");
        exit(1);
    }
    // #######################################
    // data.txt가 일반 파일이 아니라면 오류 출력
    if (!S_ISREG(sb.st_mode)) {
        perror("data.txt : Not regular file");
        exit(1);
    }
    // #######################################
    // Group/Other에 대해 읽기 쓰기 권한이 있다면
    // 에러 출력
    if (S_IRGRP & sb.st_mode | S_IWGRP & sb.st_mode | 
        S_IROTH & sb.st_mode | S_IWOTH & sb.st_mode) {
        perror("data.txt must be protected");
        exit(1);
    }
    // #######################################

    while (1) {
        write(1, ">> ", 3);
        n = read(0, buf, 255);
        buf[n] = '\0';

        if (n > 0) {
            if (write(fd, buf, n) != n) {
                perror("Write error");

            }
            
        } else if (n == -1) {
            perror("Read error");

        }

        if (n == 1 && buf[0] == 'q') {
            write(1, "Terminate\n", 10);
            break;
            
        }

        write(1, buf, n);
    }

    close(fd);

    return 0;
}