#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd;

    fd = open("tmp.bbb", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Create tmp.bbb");
        exit(1);
    }

    // fd에 대하여 표준 입출력 1에 대해
    // 복사해버리면 파일에 출력된다.    
    dup2(fd, 1);

    printf("DUP2 : Standard Output Redirection\n");
    close(fd);

    return 0;
} 