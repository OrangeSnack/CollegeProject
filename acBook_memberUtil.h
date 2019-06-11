/*
담당자 : 강성근
개요 : 로그인, 회원가입등을 지원하는 파일
기능 :
    로그인
    회원가입
*/
#ifndef acBook_memberUtil
    #define acBook_memberUtil

#define MAXNUM 20

#include "acBook_util.h"
#include "acBook_orderConverter.h"
/* #include <logindb.h> //회원정보 DB 헤더 */

/*
//ID DB에 출력해주는 함수
char* idOut(char *id);

//PW DB에 출력해주는 함수
char* pwOut(char *pw);
*/

////양희찬

//수정 명령
int modify(char* id, char* pw);

//가입 명령
int registeration(char* id, char* pw);

//로그인 명령
int login(char* id, char* pw);

//로그인 명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int loginBool();

//회원가입 명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int registerationBool();

//회원정보수정명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int modifyBool();

#endif
