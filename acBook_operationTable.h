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

    #include "acBook_processingData.h"      // 테이블 연산 파일

    typedef struct _Order_queue Order_queue;

    // 테이블 구조체
    typedef struct _Table {
        char* name;         // 테이블의 이름
        Col* start_col;     // 시작열을 가리키는 포인터.
        int num_col;        // 열 갯수
        int num_record;     // 행 갯수.
        Cursor* cursor;     // 커서 포인터
    } Table;

    // 조작 명령을 받아 테이블 연산을 실행해 반환.
    Table* table_manager(Order_queue* order);
    // 테이블 생성 함수.
    // name : 생성할 테이블의 이름
    Table* new_table(char* name);
    // 테이블 삭제 함수.
    // target : 삭제할 테이블 객체의 주소
    void delete_table(Table* target);


    // 열 구조체
    typedef struct _Col {
        char* name;         // 해당 열의 이름.
        Col* next;          // 다음열을 가리키는 포인터. 끝 열이면 NULL
        Col* prev;          // 이넞열을 가리키는 포인터. 시작 열이면 NULL
        Data* start_data;   // 해당 열의 시작행을 가리키는 포인터
        int is_primaryKey;  // 해당 열이 기준 열(primary key)인지 확인. 0 : 아님, 1 : 맞음.
    } Col;

    // 열 생성 함수.
    // 생성 위치는 항상 마지막 열 다음.
    // 레코드가 있는 상태에서 열을 추가시키면, 그 열의 데이터는 NULL 으로 초기화.
    // name : 새로 생성할 열의 이름.
    Col* new_col(Cursor* cursor, char* name);
    // 열 삭제 함수.
    // name : 삭제할 열의 이름
    void delete_col(Cursor* cursor, char* name);
    // 열 이동 함수.
    // 커서가 가리키고 있는 열을 좌, 우 일정칸 움직임. 실행 후 0을 반환.
    // 만약 이동할 공간이 불충분하다면 실행하지 않고 -1을 반환.
    int move_col(Cursor* cursor, int move_length);


    // 데이터 구조체. 
    // 데이터 관련 조작은 단일 데이터 수정 제외하곤 전부 레코드 단위로 동작.
    typedef struct _Data {
        char* contents;     // 해당 위치에 기록된 데이터 내용.
        Data* next;         // 다음 레코드 데이터. 끝이면 NULL.
        Data* prev;         // 이전 레코드 데이터. 시작이면 NULL.
    } Data;

    // 레코드 생성 함수.
    // content : 레코드에 삽입될 문자열 배열
    Data* new_record(Cursor* cursor, char** content);
    // 레코드 삭제 함수 : 커서가 가리키고 있는 레코드를 삭제.
    void delete_record(Cursor* cursor);
    // 데이터 수정 함수 : 커서가 가리키고 있는 특정 데이터의 값을 변경.
    // content : 기존 값을 대체할 문자열.
    void edit_data(Cursor* cursor, char* content);
    // 데이터 전체 수정 함수 : 특정열에서 특정 값을 가진 모든 데이터를 변경.
    // col_name : 해당 함수를 적용할 열의 이름
    // before : 바뀌기 전 값.
    // content : 바꿀 값.
    void edit_allData(Cursor* cursor, char* col_name, char* before, char* content);
    // 데이터 정렬 함수.
    // col_name : 정렬기준이 될 열 이름 배열. 낮은 인덱스가 가장 우선시 됨.
    // direction : 정렬 방향. 1 : 오름차순, -1 : 내림차순
    void sort_data(Cursor* cursor, char** col_name, int direction);


    // 검색, 삽입, 삭제 등을 시행할 때 관측하고 있는 위치를 기록하는 구조체
    // 테이블 객체와 같이 생성되었다가, 같이 삭제됨.
    typedef struct _Cursor {
        Table*  pos_table;  // 현재 가리키고 있는 데이터의 테이블 주소
        Col*    pos_col;    // 현재 가리키고 있는 데이터의 열 주소
        Data**  pos_record; // 현재 가리키고 있는 레코드의 전체 데이터 주소.
    } Cursor;

    // 커서 생성 함수.
    void new_cursor();
    // 커서 제거 함수.
    void delete_cursor();
    // 커서 이동 함수 : 지정한 열로 이동
    // col_name : 이동할 열의 이름
    Cursor* move_cursor_by_colName(char* col_name);
    // 커서 이동 함수 : 열 이동.
    // direction : 이동할 방향. 1 = 다음, -1 = 이전.
    Cursor* move_cursor_col(int direction);
    // 커서 이동 함수 : 행(레코드) 이동
    // direction : 이동할 방향. 1 = 다음, -1 = 이전.
    Cursor* move_cursor_record(int direction);

#endif