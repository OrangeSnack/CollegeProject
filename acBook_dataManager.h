/*
거래내역 출력.
거래내역 추가.
거래내역 수정.
거래내역 삭제.
*/
#ifndef acBook_dataManager
    #define acBook_dataManager

    #include "acBook_orderConverter.h"
    #include "acBook_util.h"
    #include "acBook_memberUtil.h"

    #include <time.h>

    // 가계부 메인 화면.
    void acBook_mainPage();

    // 거래내역 출력. (20칸씩 잘라서)
    // select UI를 이용한 거래내역 선택 기능 제공
    void acBook_show_list(Table* table);

    // 거래내역 추가.
    // 빈칸을 선택하면 거래내역을 추가함.
    void acBook_add_list(char* serial_num, Table* table);

    // 거래내역 수정.
    // 특정 거래내역을 선택하면 해당 거래내역을 다시 입력함.
    // 날짜 적는 칸에 delete를 입력하면 해당 거래를 아예 삭제함.
    void acBook_edit_list(char* serial_num, Table* table);


#endif

/*
파일 양식 : 
    serial_code : 거래들을 구분하는 이름. time(NULL) 값을 이용해 삽입.
    일시 : 거래 일시. 19-06-13. 14:00 같은 양식
    내용 : 거래 내용. 공백을 입력해야되므로 ""로 묶어서 반영.
    금액 : 거래 금액. 지출은 -, 수익은 +

출력 양식
    상단 : 열 이름
    ==========================================
    중단 : 거래내역 (20칸)
    ==========================================
    하단 : 총 거래갯수, 소계(전체 수익, 지출 합산)
*/