/*
담당자 : 양희찬
명칭 : 명령 변환기 
개요 : 입력된 문자열을 데이터 처리 시스템용 명령어로 변환
기능 : 
    명령어 해석.
    명령어 실행.
*/
#ifndef acBook_orderConverter
    #define acBook_orderConverter

    #include <stdlib.h>
    #include <string.h>

    // 테이블 기초 함수.
    #include "acBook_operationTable.h"
    // 테이블 연산 함수.
    #include "acBook_processingData.h"      

    typedef struct _Table Table;

    // 단일 명령 실행 전반을 처리하는 함수
    // 단일 문자열 형태의 명령어를 받아 변환, 수행하고, 테이블을 반환.
    // 반환하는 테이블은 
    Table_list* order_manager(char* order_string);

    enum order_input_type {START, END, ADD};

    // 문자열을 명령어 큐로 변환해주는 함수.
    Order_queue* convert_str_to_order(Order_queue* order, char* order_string, int string_length, enum order_input_type type);

    // 명령어를 해석해서 문자열 리스트 형으로 반환하기 위한 'queue'
    // - char**  order;
    // - Order_queue* next;
    typedef struct _Order_queue {
        char*  order;       // 명령어 문자열. 동적할당. 'table_manager'에서 반환.
        Order_queue* next;  // 다음 명령을 가리키는 포인터.
    } Order_queue;    

    // 조작 명령을 받아 테이블 연산을 실행해 반환.
    Table_list* table_manager(Order_queue* order, int repeat_check);

#endif