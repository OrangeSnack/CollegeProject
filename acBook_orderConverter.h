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
    #include "acBook_error.h"     

    typedef struct _Order_queue Order_queue;

    // 명령어를 해석해서 문자열 리스트 형으로 반환하기 위한 'queue'
    // - char**  order;
    // - Order_queue* next;
    typedef struct _Order_queue {
        char*  order;       // 명령어 문자열. 동적할당. 'table_manager'에서 반환.
        Order_queue* next;  // 다음 명령을 가리키는 포인터.
    } Order_queue;    

    // 단일 명령 실행 전반을 처리하는 함수
    // 단일 문자열 형태의 명령어를 받아 변환, 수행하고, 테이블을 반환.
    // 반환하는 테이블은 
    Table_list* order_manager(char* order_string);

    enum order_input_type {START, END, ADD};

    // 문자열을 명령어 큐로 변환해주는 함수.
    Order_queue* convert_str_to_order(Order_queue* order, char* order_string, int string_length, enum order_input_type type);

    // 조작 명령을 받아 테이블 연산을 실행해 반환.
    Table_list* table_manager(Order_queue** order, int repeat_check);

    // 명령 실행부
    void order_processor(Order_queue** order, Table_list* main_list, Table_list* temp_list, Table_list* return_list);

    // 명령 큐 해독부
    char* read_order_line(Order_queue** order_pointer);

    // 함수 호출부
    int call_function(char* function_name, char** parameters, Table_list** total_list, int num_parameters);

    // 모든 테이블 리스트를 제거하고 프로그램을 종료할 준비를 해주는 함수.
    void end_program(Table_list** total_list);


#endif