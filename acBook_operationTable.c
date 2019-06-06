#include "acBook_operationTable.h"


Table** table_manager(Order_queue* order) {
    static Table_list main_table_list;      // 삭제 혹은 프로그램 종료 전까지 유지되는 테이블 리스트
    static Table_list temp_table_list;      // 매 명령 실행 요청이 들어올 때마다 자동으로 삭제되는 임시 테이블 리스트
    static Table_list return_table_list;    // 반환값으로 전달 될 테이블 리스트. 다른 리스트들의 값을 복사해 이용.
    static int check_first_call = 0;        // 테이블 매니저의 첫 실행을 구분.

    // 테이블 매니저 첫 호출 때 초기화 실시
    if (check_first_call == 0) {
        table_list_default_setting(&main_table_list, 50);
        table_list_default_setting(&temp_table_list, 20);
        table_list_default_setting(&return_table_list, 20);
    }

    // 임시 테이블 리스트 전체 초기화.
    // 해당 리스트에 테이블이 남아있다면 그 테이블의 메모리를 삭제함.
    for (int i = 0; i < temp_table_list.max_size; i++) {
        if (temp_table_list.address[i] =! NULL) {
            delete_table(temp_table_list.name[i]);
            temp_table_list.address[i] = NULL;
            free(temp_table_list.name[i]);
        }
    }

    // 반환 테이블 리스트 전체 초기화.
    // 해당 리스트에 테이블이 남아있더라도, 메모리는 조작하지 않음.
    for (int i = 0; i < return_table_list.max_size; i++) {
        if (return_table_list.address[i] != NULL) {
            return_table_list.address[i] = NULL;
            return_table_list.name[i] = NULL;
        }
    }
    
    // 명령 수행부.
        // 열람할 테이블 지정.
    // 수행된 명령은 자동으로 메모리 반환.

    return return_table_list.address;
}

void table_list_default_setting(Table_list* target, int max_size) {
    // 최대 사이즈 지정
    target->max_size = max_size;
    // 이름 배열 동적 할당. 동시에 초기화.
    target->name = (char*) calloc(target->max_size, sizeof(char*));
    // 테이블 주소 배열 동적 할당. 동시에 초기화.
    target->address = (Table*) calloc(target->max_size, sizeof(Table*));
    // 포함 테이블 갯수 초기화
    target->num = 0;
}



Table* new_table(char* name, Table_list* target) {
    int index;

    // 이름 충돌 확인
    for (index = 0; index < target->max_size; index++) {
        if (strcmp(target->name, name) == 0) {
            show_error_message("new_table()", 409);
            return NULL;
        }            
    }

    // 잔여 공간 확인
    for (index = 0; ;index++) {
        if (index == target->max_size) {
            show_error_message("new_table()", 507);
            return NULL;
        }
        else if (target->address[index] == NULL) {
            break;
        }
    }

    // 테이블 객체 동적 할당.
    Table* table = (Table*) malloc(sizeof(Table));
    
    // 테이블 리스트 객체 정보 첨가
    target->name[index] = (char*) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(target->name[index], name);
    target->address[index] = table;

    // 테이블 객체 초기화
    strcpy(table->name, name);
    table->start_col = NULL;
    table->num_col = 0;
    table->num_record = 0;
    table->cursor = (Cursor*)malloc(sizeof(Cursor));

    // 테이블 객체의 전용 커서 객체 초기화
    table->cursor->pos_table = table;
    table->cursor->pos_col = NULL;
    table->cursor->pos_record = NULL;

    return table;
}

void delete_table(Table* target, Table_list* main_list, Table_list* temp_list) {
    
    // 해당 테이블의 잔여 레코드, 컬럼 상태를 확인.
        // 잔여 자료가 있다면 열, 레코드 삭제 함수를 반복 실행, 전부 말소.

    move_cursor_default(target->cursor);
    while(1) {
        if (target->num_col == 0)
            break;

        delete_col(target->cursor);                
    }
    

    // 모든 열이 사라지면 타겟 테이블 메모리 제거.
}


void set_table_temp(Table* target, Table_list* main_list, Table_list* temp_list) {

}

void set_table_main(Table* target, Table_list* main_list, Table_list* temp_list) {

}


Col* new_col(Cursor* cursor, char* name) {
    // 열 객체 동적할당
    // 해당 커서 다음 위치에 push
    // 열 이름 기입.
    // 커서를 이용해 Table 구조체에 접근해서 현재 레코드 갯수만큼 더미 데이터 라인 생성
}

void delete_col(Cursor* cursor, char* name);

int move_col(Cursor* cursor, int move_length);


Data* new_record(Cursor* cursor, char** content);

void delete_record(Cursor* cursor);

void edit_data(Cursor* cursor, char* content);

void edit_allData(Cursor* cursor, char* col_name, char* before, char* content);

void sort_data(Cursor* cursor, char** col_name, int direction);


void new_cursor();

void delete_cursor();

Cursor* move_cursor_by_colName(Cursor* target, char* col_name);

Cursor* move_cursor_col(Cursor* target, int direction);

Cursor* move_cursor_record(Cursor* target, int direction);

Cursor* move_cursor_default(Cursor* target);