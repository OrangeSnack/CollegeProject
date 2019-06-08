/*
담당자 : 강성근
개요 : 로그인, 회원가입등을 지원하는 파일
기능 :
    로그인
    회원가입
*/
#ifndef acBook_memberLogin
    #define acBook_memberLogin

#define MAXNUM 20

#include "acBook_memberUtil.h"

//로그인 시스템
void loginSys(char *p1, char *str, int secure);

//로그인 성공여부 출력
int loginCheck(char *id, char *pw);

//로그인 메인함수
int loginMain();
#endif
