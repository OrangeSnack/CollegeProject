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
            delete_table(temp_table_list.address[i]);
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
    table->name = target->name + index;
    table->address = target->address + index;
    table->start_col = NULL;
    table->num_col = 0;
    table->num_record = 0;
    table->cursor = new_cursor(table);

    return table;
}

void delete_table(Table* target) {
    
    // 해당 테이블의 잔여 레코드, 컬럼 상태를 확인.
    // 잔여 자료가 있다면 열, 레코드 삭제 함수를 반복 실행, 전부 말소.
    move_cursor_default(target->cursor);
    while(1) {
        if (target->num_col == 0)
            break;

        delete_col(target->cursor);                
    }

    // 전용 커서 객체 삭제
    delete_cursor(target->cursor);

    // 테이블 리스트에서 해당 테이블의 자료 제거.
    free(target->name);
    target->address = NULL;    
    
    // 테이블 메모리 제거.
    free(target);
}

void set_table_temp(Table* target, Table_list* main_list, Table_list* temp_list) {
    change_table_status(target, main_list, temp_list);
}

void set_table_main(Table* target, Table_list* main_list, Table_list* temp_list) {
    change_table_status(target, temp_list, main_list);
}

// 테이블의 상태를 변경(메인 <-> 임시)
void change_table_status(Table* target, Table_list* before_list, Table_list* after_list) {
    // 목적지 리스트에 여유 공간이 있는지 확인.
    int before_index, after_index;
    for(after_index = 0; ; after_index++) {
        if (after_index == after_list->max_size) {
            show_error_message("change_table_status()", 507);
            return;
        }

        if (after_list->address[after_index] == NULL) {
            break;
        }
    }
    
    // 기존 리스트에서 해당 테이블이 기록된 위치를 확인
    before_index = target->address - before_list->address;
    if (before_index < 0 || before_index >= 50) {       // 거리가 50칸 이내가 아님 -> 다른 리스트에서 찾는 중.
        show_error_message("change_table_status", 404);
        return;
    }

    // 목적지 리스트에 값을 옮겨적음.
    after_list->name[after_index] = (char*) malloc(strlen(before_list->name[before_index]) * sizeof(char));
    strcpy(after_list->name[after_index], before_list->name[before_index]);
    after_list->address[after_index] = before_list->address[before_index];

    // 기존 테이블의 값을 초기화
    free(before_list->name[before_index]);
    before_list->address = NULL;
}


Col* new_col(Table* target, char* name) {
    // 임시 커서 객체 생성.
    Cursor* cursor = new_cursor(target);

    // 임시 커서 객체를 이용, 중복되는 열 이름이 있는지 확인.
    while(1) {
        if ( strcmp(*(cursor->pos_col->name), name) == 0) {
            show_error_message("new_col()", 409);
            return NULL;
        }
        else if (cursor->pos_col->next == NULL) {
            break;
        }
        else {
            move_cursor_col(&cursor, 1);
        }
    }

    // 열 객체 동적 할당.
    Col* col = (Col*) malloc(sizeof(Col));
    // 열 객체 초기화.
    if (target->num_col == 0)
        col->is_primaryKey = 1;
    strcpy(col->name, name);
    col->prev = target->cursor->pos_col;
    col->next = target->cursor->pos_col->next;
    col->start_data = (Data*) malloc(sizeof(Data));
    // 데이터 헤더 정보 초기화
    col->start_data->content = NULL;
    col->start_data->prev = NULL;
    col->start_data->next = NULL;

    // 기존 col 연결 리스트 연결 재정리
    if (target->cursor->pos_col->next->prev != NULL)
        target->cursor->pos_col->next->prev = col;
    target->cursor->pos_col->next = col;

    // 커서 Data 부분 재할당.
    realloc(target->cursor->pos_record, target->num_col * sizeof(Data));
    // 커서 초기화
    move_cursor_default(target->cursor);

    // 커서를 이용해 Table 구조체에 접근해서 현재 레코드 갯수만큼 더미 데이터 라인 생성
    for (int i = 0; i < target->num_record; i++) {
        new_record(target, NULL);
    }

    delete_cursor(cursor);
}

void delete_col(Table* target) {
    // 해당 열 번호를 반환
    int index = 0;
    Cursor* cursor = new_cursor(target);
    cursor = get_index_by_cursor(target->cursor);

    // 첫 레코드로 이동
    move_cursor_record(target->cursor, 0);
    move_cursor_record(target->cursor, 1);
    
    // 해당 col에 데이터가 완전히 제거될 때까지 데이터를 삭제.
    while(target->cursor->pos_record[index] != NULL) {
        target->cursor->pos_record[index]->prev->next = target->cursor->pos_record[index]->next;
        if (target->cursor->pos_record[index]->next != NULL)
            target->cursor->pos_record[index]->next->prev = target->cursor->pos_record[index]->prev;
        cursor->pos_record[index] = target->cursor->pos_record[index]->next;
        free(target->cursor->pos_record[index]);
        target->cursor->pos_record[index] = cursor->pos_record[index];
    }

    // 헤더 데이터를 삭제
    free(target->cursor->pos_col->start_data);

    // col 객체 삭제 및 조정
    target->cursor->pos_col->prev->next = target->cursor->pos_col->next;
    if (target->cursor->pos_col->next != NULL)
        target->cursor->pos_col->next->prev = target->cursor->pos_col->prev;
    free(target->cursor->pos_col);

    // table.num_col 값 수정
    target->num_col -= 1;
    move_cursor_default(target->cursor);

    // 동적 할당된 임시 커서 객체 반환
    delete_cursor(cursor);
}

// 미완성
Col* merge_col(Table* target, char* name, Data* data_start) {
    show_error_message("add_col", 501);
    return target->start_col;
}

void move_col(Table* target, char* name) {
    Cursor* cursor = new_cursor(target);
    Cursor* temp = new_cursor(NULL);

    if (move_cursor_by_colName(cursor, name) != NULL) {
        // 자기 자신의 위치에 옮기는지 확인
        if (target->cursor->pos_col == cursor->pos_col){
            show_error_message("move_col()", 409);
            return;
        }

        // 각 열의 앞 뒤 연결 교체.
        temp->pos_col->prev = target->cursor->pos_col->prev->next;
        if (target->cursor->pos_col->next != NULL) 
            temp->pos_col->next = target->cursor->pos_col->next->prev;
        else 
            temp->pos_col->next = NULL;

        target->cursor->pos_col->prev->next = cursor->pos_col->prev->next;
        if (target->cursor->pos_col->next != NULL)
            target->cursor->pos_col->next->prev = cursor->pos_col->next->prev;

        cursor->pos_col->prev->next = temp->pos_col->prev;
        if (cursor->pos_col->next != NULL)
            cursor->pos_col->next->prev = temp->pos_col->next;

        // 각 열의 연결 고리 교체.
        temp->pos_col->prev = target->cursor->pos_col->prev;
        temp->pos_col->next = target->cursor->pos_col->next;

        target->cursor->pos_col->prev = cursor->pos_col->prev;
        target->cursor->pos_col->next = cursor->pos_col->next;

        cursor->pos_col->prev = temp->pos_col->prev;
        cursor->pos_col->next = temp->pos_col->next;
    }
    else {
        show_error_message("move_col()", 404);
    }

    delete_cursor(cursor);    
}


Data** new_record(Table* target, char** contents) {
    // 임시 레코드 동적 할당.
    Data** data_arr = (Data**) malloc(target->num_col * sizeof(Data*));
    for (int i = 0; i < target->num_col; i++) {
        data_arr[i] = (Data*) malloc(sizeof(Data));
        data_arr[i]->prev = target->cursor->pos_record[i];
        data_arr[i]->next = target->cursor->pos_record[i]->next;
        target->cursor->pos_record[i] = data_arr[i];
        if ((i + 1) <= (sizeof(contents) / sizeof(char*))) {
            data_arr[i]->content = (char*) malloc((strlen(contents[i]) + 1) * sizeof(char));
            strcpy(data_arr[i]->content, contents[i]);
        }
        else {
            data_arr[i]->content = NULL;
        }
    }

    // 임시 레코드 메모리 반환.
    free(data_arr);

    // 커서의 해당 부분 반환.
    return target->cursor->pos_record;
}

void delete_record(Table* target) {
    if (target->num_record == 0) {
        show_error_message("delete_record", 404);
        return;
    }
    else if (target->cursor->pos_record[0] == target->start_col->start_data) {
        show_error_message("delete_record", 400);
    }

    // 임시 커서 객체 생성
    Cursor* cursor = new_cursor(target);

    // 레코드 삭제
    for (int i = 0; i < target->num_col; i++) {
        target->cursor->pos_record[i]->prev->next = target->cursor->pos_record[i]->next;
        if (target->cursor->pos_record[i]->next != NULL)
            target->cursor->pos_record[i]->next->prev = target->cursor->pos_record[i]->prev;
        cursor->pos_record[i] = target->cursor->pos_record[i]->next;
        free(target->cursor->pos_record[i]);
        target->cursor->pos_record[i] = cursor->pos_record[i];
    }

    // 임시 커서 객체 제거
    free(cursor);
}

void edit_data(Table* target, char* content) {
    // 열 번호 받기
    int index = get_index_by_cursor(target);

    // 공간 재할당 및 복사
    realloc(target->cursor->pos_record[index]->content, (strlen(content) + 1) * sizeof(char));
    strcpy(target->cursor->pos_record[index]->content, content);
}

void edit_allData(Table* target, char* col_name, char* before, char* content){
    
    // 열 번호 받기
    int index = get_index_by_name(target, col_name);

    if (index == -1) {
        show_error_message("edit_allData()", 404);
        return;
    }

    // 커서를 시작으로 이동 후, 순회 검색. (해당 col만)
    move_cursor_record(target->cursor, 0);

    while(target->cursor->pos_record[index] != NULL) {
        if (target->cursor->pos_record[index] == before) {
            realloc(target->cursor->pos_record[index]->content, (strlen(content) + 1) * sizeof(char));
            strcpy(target->cursor->pos_record[index]->content, content);
        }
    }
    target->cursor->pos_record[index] = target->cursor->pos_col->start_data;
}

void sort_data(Table* target, char** col_name, int direction) {
    show_error_message("sort_data", 501);
}


Cursor* new_cursor(Table* target) {
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));
    if (target != NULL) {
        cursor->pos_table = target;
        cursor->pos_record = (Data*)malloc(target->num_col * sizeof(Data));
        move_cursor_default(cursor);
    }
    else {
        cursor->pos_table = cursor->pos_col = cursor->pos_record = NULL;
    }
    return cursor;
}

void delete_cursor(Cursor* target) {
    free(target->pos_record);
    free(target);
}

Cursor* move_cursor_by_colName(Cursor* target, char* col_name) {
    // 시작 열로 커서 이동.
    target->pos_col = target->pos_table->start_col;
    
    int index = 0;
    // 이름이 일치할 때 까지 확인
    while(index < target->pos_table->num_col) {
        if (strcmp(*(target->pos_col->name), col_name))
            return target;
        else {
            move_cursor_col(target, 1);
            index += 1;
        }
    }

    // 일치되는 이름이 없으면 커서를 초기화 하고 NULL 반환
    show_error_message("move_cursor_by_colName", 404);
    move_cursor_default(target);
    return NULL;
}

Cursor* move_cursor_col(Cursor* target, int direction) {
    switch(direction) {
        case 1:
            if (target->pos_col->next != NULL)
                target->pos_col = target->pos_col->next;
            else
                show_error_message("move_cursor_col", 404);
            break;
        case -1:
            if (target->pos_col->prev != NULL)
                target->pos_col = target->pos_col->prev;
            else
                show_error_message("move_cursor_col", 404);
            break;
        case 0:
            target->pos_col = target->pos_table->start_col;
            break;
        default:
            show_error_message("move_cursor_col", 400);
            break;            
    }

    return target;
}

Cursor* move_cursor_record(Cursor* target, int direction) {
    switch(direction) {
        case 1:
            if (target->pos_record[0]->next != NULL)
                for (int i = 0; i < target->pos_table->num_col; i++)
                    target->pos_record[i] = target->pos_record[i]->next;
            break;
        case -1:
            if (target->pos_record[0]->prev != NULL)
                for (int i = 0; i < target->pos_table->num_col; i++)
                    target->pos_record[i] = target->pos_record[i]->prev;
            break;
        case 0:
            if (target->pos_table->num_col != 0) {
                Col* temp_pos_col = target->pos_col;
                target->pos_col = target->pos_table->start_col;
                for (int i = 0; i < target->pos_table->num_col; i++) {
                    target->pos_record[i] = target->pos_col->start_data;
                    move_cursor_col(target, 1);
                }
                target->pos_col = temp_pos_col;
            }
            else
                target->pos_record = NULL;
            break;
        default:
            show_error_message("move_cursor_record", 400);
            break;
    }

    return target;
}

Cursor* move_cursor_default(Cursor* target) {
    if (target->pos_table->num_col != 0) {
        move_cursor_col(target, 0);
        move_cursor_record(target, 0);
    }
    else {
        target->pos_col = NULL;
        target->pos_record = NULL;
    }

    return target;
}

int get_index_by_cursor(Table* target) {
    int index = 0;
    Cursor* cursor = new_cursor(target);
    move_cursor_col(cursor, 0);
    while(1) {
        if (cursor->pos_col == target->cursor->pos_col)
            break;
        else {
            move_cursor_col(cursor, 1);
            index += 1;
        }
    }

    delete_cursor(cursor);
    
    return index;
}

int get_index_by_name(Table* target, char* name) {
    int index = 0;
    Cursor* cursor = new_cursor(target);
    move_cursor_col(cursor, 0);
    while(1) {
        if (cursor->pos_col->name == name)
            break;
        else if (index == target->num_col) {
            show_error_message("get_index_by_name", 404);
            index = -1;
            break;
        }
        else {
            move_cursor_col(cursor, 1);
            index += 1;
        }
    }

    delete_cursor(cursor);
    
    return index;

}