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

#include "acBook_util.h"
#include <string.h>
/* #include <logindb.h> //회원정보 DB 헤더 */


//문자열 입력받고 p1에다가 복사
//secure에 1 입력시 입력받는 문자들을 *로 출력
void inputSys(int secure);

//로그인 시스템
void loginSys(char *p1, char *str, int secure);

//ID DB에 출력해주는 함수
char* idOut(char *id);

//PW DB에 출력해주는 함수
char* pwOut(char *pw)

//로그인 메인함수
int loginMain();
#endif
