/*
담당자 : 강성근
개요 : 로그인, 회원가입등을 지원하는 파일
기능 :
    로그인
    회원가입
*/
#ifndeacBook_memberUtil
    #define acBook_memberUtil

#define MAXNUM 20

#include "acBook_util.h"
/* #include <logindb.h> //회원정보 DB 헤더 */

//ID DB에 출력해주는 함수
char* idOut(char *id);

//PW DB에 출력해주는 함수
char* pwOut(char *pw);

//수정 명령
Table_list* modify(char* id, char* pw);

//가입 명령
Table_list* register(char* id, char* pw);

//로그인 명령
Table_list* login(char* id, char* pw);
#endif
