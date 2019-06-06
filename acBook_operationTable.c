#include "acBook_operationTable.h"


Table* table_manager(Order_queue* order) {
    static char* list_table_name[50];           // 테이블 이름 리스트
    static Table* list_table_address[50];       // 테이블 주소 리스트
    
    // 명령 수행부.
    // 수행된 명령은 자동으로 메모리 반환.

    // return table;
}



Table* new_table(char* name);

void delete_table(Table* target);


Col* new_col(Cursor* cursor, char* name);

void delete_col(Cursor* cursor, char* name);

int move_col(Cursor* cursor, int move_length);


Data* new_record(Cursor* cursor, char** content);

void delete_record(Cursor* cursor);

void edit_data(Cursor* cursor, char* content);

void edit_allData(Cursor* cursor, char* col_name, char* before, char* content);

void sort_data(Cursor* cursor, char** col_name, int direction);


void new_cursor();

void delete_cursor();

Cursor* move_cursor_by_colName(char* col_name);

Cursor* move_cursor_col(int direction);

Cursor* move_cursor_record(int direction);