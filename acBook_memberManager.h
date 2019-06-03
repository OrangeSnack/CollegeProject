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

//두 문자열 입력받고 다르면 0, 같으면 1 출력
int compare(char *p1, char *p2)
{
  if(strcmp(p1,p2) == 0)
    return 1;
  else
    return 0;
}


#endif
