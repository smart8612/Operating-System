#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd, n;
    char buf[BUFSIZ];
    mode_t mode;

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (fd == -1) {
        perror("Open data.txt");
        exit(1);
    }

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