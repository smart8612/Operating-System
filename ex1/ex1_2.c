// [예제 1-1] 시스템 호출 오류 처리하기

// [개요] 현재 디렉토리에 unix.txt 파일이 존재하는지
// 확인해보고 없다면 상응하는 error no를 출력한다. 

// 시스템 콜이 정의된 헤더 파일
#include <stdlib.h>

#include <stdio.h>
#include </usr/include/errno.h>

// 오류 코드는 전역변수 errno에 저장된다.
extern int errno;

int main(void) {
    // C언어에서 제공하는 파일 구조체 형식의 포인터
    // 운영체제가 관리하는 파일 디스크립터랑 혼동하지 말자!
    FILE *fp;

    // unix.txt 파일에 접근해주는 라이브러리 함수
    // 오류가 발생하면 NULL 혹은 -1을 리턴함
    if ((fp = fopen("unix.txt", "r")) == NULL) {
        printf("errorno=%d\n", errno);
        exit(1);
    }

    fclose(fp);

    return 0;
}