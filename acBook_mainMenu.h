/*
제작자 : 강성근
개요 : 메뉴 관련 함수 모음
기능 :
    로그인 페이지
    작업 페이지
*/
#ifndef acBook_mainMenu
    #define acBook_mainMenu

    #include "acBook_util.h"
    #include "acBook_memberUtil.h"

//첫페이지 선택한 번호 출력
int mainMenu(void);

//메인메뉴 선택 링크 함수
void mainSelection(int selection);

//로그인 메뉴 선택한 번호 출력
int loginMenu(void);

//로그인 선택 링크 함수
void loginSelection(int selection);


#endif
