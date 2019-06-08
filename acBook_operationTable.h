/*
담당자 : 양희찬
개요 : 각종 테이블 연산을 별도로 분리한 파일
기능 : 
    열 관련 : 
        열 추가
        열 삭제
        열 병합
    행 관련 : 
        데이터 추가
        데이터 병합
    통계 관련 : 
        요소별 합계.
        요소별 평균.
        요소별 갯수.
*/
#ifndef acBook_operationTable
    #define acBook_operationTable

    #include <stdlib.h>
    #include <string.h>

    #include "acBook_processingData.h"      // 테이블 연산 파일
    #include "acBook_error.h"               // 에러 처리 파일

    typedef struct _Order_queue Order_queue;
    typedef struct _Table_list Table_list;
    typedef struct _Table Table;
    typedef struct _Col Col;
    typedef struct _Data Data;
    typedef struct _Cursor Cursor;

    // 테이블 리스트 구조체
    // - char** name;
    // - Table** address;
    // - int num;
    // - int max_size;
    typedef struct _Table_list {
        char** name;            // 테이블의 이름.
        Table** address;        // 테이블의 주소
        int num;                // 해당 구조체에 포함되는 테이블의 갯수.
        int max_size;           // 최대로 담을 수 있는 공간의 크기. 동적배열에 이용.
    } Table_list;

    // 개요 : 테이블 구조체
    // - char** name;    
    // - Table** address; 
    // - Col* start_col;
    // - int num_col;
    // - int num_record;
    // - Cursor* cursor;
    typedef struct _Table {
        char** name;        // 테이블 리스트에 기록된 테이블 이름 위치
        Table** address;    // 테이블 리스트에 기록된 테이블 주소 위치
        Col* start_col;     // 시작열을 가리키는 포인터.
        int num_col;        // 열 갯수
        int num_record;     // 행 갯수.
        Cursor* cursor;     // 커서 포인터
    } Table;

    // 조작 명령을 받아 테이블 연산을 실행해 반환.
    Table** table_manager(Order_queue* order);
    // 기타 부속함수.
    void table_list_default_setting(Table_list* target, int max_size);

    // 테이블 생성 함수.
    // name : 생성할 테이블의 이름
    // target : 테이블을 생성할 위치(메인, 임시 리스트)
    Table* new_table(char* name, Table_list* target);
    // 테이블 삭제 함수.
    // target : 삭제할 테이블 주소
    void delete_table(Table* target);
    // 임시 테이블로 변경
    // target : 변경시킬 테이블 주소
    // main_list : 메인 테이블 리스트
    // temp_list : 임시 테이블 리스트 
    void set_table_temp(Table* target, Table_list* main_list, Table_list* temp_list);
    // 메인 테이블로 변경
    // 임시 테이블 지정.
    // target : 변경시킬 테이블 주소
    // main_list : 메인 테이블 리스트
    // temp_list : 임시 테이블 리스트 
    void set_table_main(Table* target, Table_list* main_list, Table_list* temp_list);


    // 개요 : 열 구조체
    // 비고 : 열 이름은 절대 중복될 수 없음.
    // - char* name;
    // - Col* next;
    // - Col* prev;
    // - Data* start_data;
    // - int is_primaryKey;
    typedef struct _Col {
        char* name;         // 해당 열의 이름.
        Col* next;          // 다음열을 가리키는 포인터. 끝 열이면 NULL
        Col* prev;          // 이넞열을 가리키는 포인터. 시작 열이면 NULL
        Data* start_data;   // 해당 열의 시작행을 가리키는 포인터
        int is_primaryKey;  // 해당 열이 기준 열(primary key)인지 확인. 0 : 아님, 1 : 맞음.
    } Col;

    // 개요 : 열 생성 함수.
    // 비고 : 처음 생성하는 열은 자동으로 'primary key'가 됨.
    // 생성위치 : 현재 커서가 지정하고 있는 위치 다음.
    // 레코드가 있는 상태에서 열을 추가시키면, 그 열의 데이터는 NULL 으로 초기화.
    // name : 새로 생성할 열의 이름.
    Col* new_col(Table* target, char* name);
    // 열 삭제 함수.
    // cursor : 삭제할 열의 위치
    void delete_col(Table* target);
    // 열 추가 함수.
    // 생성위치 : 현재 커서가 지정하고 있는 위치 다음.
    Col* merge_col(Table* target, char* name, Data* data_start);
    // 열 이동 함수.
    // name : 이동시킬 위치 열 이름. 해당 열 다음에 삽입됨.
    void move_col(Table* target, char* name);


    // 개요 : 데이터 구조체. 
    // 비고 : 첫번째 데이터는 항시 head 역할을 위해 자동생성되며, 데이터를 저장하지 않음.
    // - char* contents;  
    // - Data* next;      
    // - Data* prev;      
    typedef struct _Data {
        char* content;      // 해당 위치에 기록된 데이터 내용.
        Data* next;         // 다음 레코드 데이터. 끝이면 NULL.
        Data* prev;         // 이전 레코드 데이터. 시작이면 NULL.
    } Data;

    // 레코드 생성 함수.
    // 생성 위치는, 커서의 다음.
    // content : 레코드에 삽입될 문자열 배열
    Data** new_record(Table* target, char** content);
    // 레코드 삭제 함수 : 커서가 가리키고 있는 레코드를 삭제.
    void delete_record(Table* target);
    // 데이터 수정 함수 : 커서가 가리키고 있는 특정 데이터의 값을 변경.
    // content : 기존 값을 대체할 문자열.
    void edit_data(Table* target, char* content);
    // 데이터 전체 수정 함수 : 특정열에서 특정 값을 가진 모든 데이터를 변경.
    // col_name : 해당 함수를 적용할 열의 이름
    // before : 바뀌기 전 값.
    // content : 바꿀 값.
    void edit_allData(Table* target, char* col_name, char* before, char* content);
    // 데이터 정렬 함수.
    // col_name : 정렬기준이 될 열 이름 배열. 낮은 인덱스가 가장 우선시 됨.
    // direction : 정렬 방향. 1 : 오름차순, -1 : 내림차순
    void sort_data(Table* target, char** col_name, int direction);


    // 개요 : 검색, 삽입, 삭제 등을 시행할 때 관측하고 있는 위치를 기록하는 구조체
    // 비고 : 메인 커서는 테이블 객체에 포함되고, 기타 필요에 따라 보조 임시 객체 생성, 활용.
    // - Table*  pos_table;  
    // - Col*    pos_col;    
    // - Data**  pos_record; 
    typedef struct _Cursor {
        Table*  pos_table;  // 현재 가리키고 있는 데이터의 테이블 주소
        Col*    pos_col;    // 현재 가리키고 있는 데이터의 열 주소
        Data**  pos_record; // 현재 가리키고 있는 레코드의 전체 데이터 주소.
    } Cursor;

    // 커서 생성 함수.
    Cursor* new_cursor(Table* target);
    // 커서 제거 함수.
    void delete_cursor(Cursor* target);
    // 커서 이동 함수 : 지정한 열로 이동
    // col_name : 이동할 열의 이름
    Cursor* move_cursor_by_colName(Cursor* target, char* col_name);
    // 커서 이동 함수 : 열 이동.
    // direction : 이동할 방향. 1 : 다음, -1 : 이전. 0 : 첫 열로
    Cursor* move_cursor_col(Cursor* target, int direction);
    // 커서 이동 함수 : 행(레코드) 이동
    // direction : 이동할 방향. 1 : 다음, -1 : 이전. 0 : 헤더로
    Cursor* move_cursor_record(Cursor* target, int direction);
    // 커서 이동 함수 : 첫번째 행, 헤더 레코드로 이동.
    Cursor* move_cursor_default(Cursor* target);

#endif