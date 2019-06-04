/*
담당자 : 전원
개요 : 자주 쓰이는 함수 모음
함수 내역 :
    커서 이동 함수.
    정렬 함수.
*/
#ifndef acBook_util
    #define acBook_util

    #define TRUE 1
    #define FALSE 0

    #include <windows.h>
    #include <conio.h>

    //커서위치 이동
    void gotoxy(int x, int y);

    //키보드 방향키 출력
    //0: 다른것, 1: ↑, 2: ←, 3: →, 4: ↓, 5: 엔터, 6: Esc
    int kbArrow();

    //selectUi 하위함수
    int checkOverLine(int line, int current);

    //키보드로 입력받는 선택GUI.
    //시작하는 x,y좌표, 선택할수 있는 라인 갯수를 입력
    //사용하기전 사용할 라인에 \t를 하나씩 출력해놓아야함
    //선택한 라인 번수 출력 (1~line)
    //Esc버튼 누를시 0 출력
    int selectUi(int x, int y, int line);

#endif
