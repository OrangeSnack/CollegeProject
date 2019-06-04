/*
담당자 : 강성근
개요 : 로그인, 회원가입등을 지원하는 파일
기능 :
    로그인
    회원가입
*/
#ifndef acBook_memberManager
    #define acBook_memberManager

#include <stdio.h>
#include <string.h>
//#include <logindb.h> //회원정보 DB 헤더

//두 문자열 입력받고 다르면 0, 같으면 1 출력
int compare(char *p1, char *p2);

//문자열 입력받고 주소 출력
//strcpy 사용해야함
//secure에 1 입력시 입력받는 문자들을 *로 출력
char* inputSys(int secure);

//로그인 성공여부 체크
//성공시 1, 실패시 0 출력
int loginCheck(char *id, char *pw, char *rid, char *rpw);

//로그인 시스템
int loginSys();

//로그인 메인함수
int loginMain();
#endif
