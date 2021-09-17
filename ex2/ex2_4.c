// [예제 2-2] 파일 읽기 쓰기 예제
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int rfd, wfd, n;
    char buf[BUFSIZ];

    rfd = open("unix.txt", O_RDONLY);
    if (rfd == -1) {
        perror("Open unix.txt");
        exit(1);
    }
    
    // unix.bak 파일은 RW, R, R 권한을 갖으며
    // O_CREATE: 파일이 없으면 파일을 생성
    // O_WRONLY: 파일을 쓰기 전용으로 열기
    // O_TRUNC:  파일이 존재하면 기존의 내용을 삭제 후 덮어쓰기
    wfd = open("unix.bak", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (wfd == -1) {
        perror("Open unix.bak");
        exit(1);
    }

    while ((n = read(rfd, buf, 6)) > 0)
    {
        if (write(wfd, buf, n) != n) perror("Write");
        if (n == -1) perror("Read");
    }

    close(rfd);
    close(wfd);

    return 0;
}