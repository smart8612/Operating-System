#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
 
int main(void) {
    // 파일 디스크립터는 정수형이다.
    int fd;
    mode_t mode;

    // 모드를 살펴보면 사용자, 그룹, 기타 묶음으로
    // 나뉘어져 있으며 3개의 비트 조합으로 이루어짐
    // READ: 읽기, Write: 쓰기, Execute: 실행
    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    fd = open("unix.txt", O_CREAT, mode);
    if (fd == -1) {
        perror("Creat");
        exit(1);
    }
    close(fd);

    return 0;
}