// [예제 1-1] 시스템 호출 오류 처리하기

// [개요] 현재 디렉토리에 unix.txt 파일이 존재하는지
// 확인해보고 없다면 상응하는 error no를 출력한다. 

// 시스템 콜이 정의된 헤더 파일
#include <unistd.h>

#include <stdio.h>
#include </usr/include/errno.h>

// 오류 코드는 전역변수 errno에 저장된다.
extern int errno;

int main(void) {
    // unix.txt 파일에 접근해주는 시스템 콜
    // 시스템 콜은 정상처리(0) 에러(-1) 리턴
    if (access("unix.txt", F_OK) == -1) {
        printf("errno=%d\n", errno);
    }
    
    return 0;
}