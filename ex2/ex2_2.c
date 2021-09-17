// [예제 2-2] 파일 생성
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

    // OFLAG
    // O_CREATE: 파일이 없다면 새로운 파일 생성
    // O_EXCL: 파일이 있다면 오류 없다면 새롭게 생성
    fd = open("unix.txt", O_CREAT | O_EXCL, mode);
    if (fd == -1) {
        perror("Creat");
        exit(1);
    }

    // fd를 닫지 않고 계속 열어두면
    // 파일디스크립터가 쌓이며
    // 최대값이 존재하기 때문에 
    // 오류 발생 가능선 높음!
    close(fd);

    return 0;
}