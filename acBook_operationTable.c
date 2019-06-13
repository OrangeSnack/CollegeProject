#include "acBook_operationTable.h"

void table_list_default_setting(Table_list* target, int max_size) {
    // 최대 사이즈 지정
    target->max_size = max_size;
    // 이름 배열 동적 할당. 동시에 초기화.
    target->name = (char**) calloc(target->max_size, sizeof(char*));
    // 테이블 주소 배열 동적 할당. 동시에 초기화.
    target->address = (Table**) calloc(target->max_size, sizeof(Table*));
    // 포함 테이블 갯수 초기화
    target->num = 0;
}



Table* new_table(char* name, Table_list* target) {
    int index;

    // 이름 충돌 확인
    for (index = 0; index < target->max_size; index++) {
        if (target->name[index] == NULL) {
            continue;
        }
        if (strcmp(target->name[index], name) == 0) {
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

    // 테이블 객체 초기화
    table->table_list = target;
    table->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(table->name, name);
    table->start_col = NULL;
    table->num_col = 0;
    table->num_record = 0;
    table->cursor = new_cursor(table);
    
    // 테이블 리스트 객체 정보 첨가
    target->name[index] = table->name;
    target->address[index] = table;
    target->num += 1;

    // 동적생성 된 테이블 포인터 반환.
    return table;
}

void delete_table(Table* target) {
    // NULL 포인터가 들어온 경우.
    if (target == NULL) {
        show_error_message("delete_table()", 400);
        return;
    }
    
    // 해당 테이블의 잔여 레코드, 컬럼 상태를 확인.
    // 잔여 자료가 있다면 열, 레코드 삭제 함수를 반복 실행, 전부 말소.
    move_cursor_default(target->cursor);
    while(1) {
        if (target->num_col == 0)
            break;

        delete_col(target);                
    }

    // 테이블 리스트에서 자신의 위치 인덱스를 찾아 반환.
    int index = get_index_on_table_list(target);

    // 테이블 객체에서 동적할당된 객체들 반환
    free(target->name);
    delete_cursor(target->cursor);

    // 테이블 리스트에서 해당 테이블의 자료 제거.
    target->table_list->name[index] = NULL;
    target->table_list->address[index] = NULL;
    target->table_list->num -= 1;
    
    // 테이블 메모리 제거.
    free(target);
}

// 테이블을 다른 리스트로 이동.
void change_table_status(Table* target_table, Table_list* target_list) {
    // 옮길 테이블 리스트가 동일한 장소인지 확인
    if (target_table->table_list == target_list) {
        show_error_message("change_table_status()", 400);
        return;
    }

    // 목적지 리스트에 여유 공간이 있는지 확인.
    int index = get_index_on_table_list(target_table);
    int target_index;
    for(target_index = 0; ; target_index++) {
        if (target_index == target_list->max_size) {
            show_error_message("change_table_status()", 507);
            return;
        }

        if (target_list->address[target_index] == NULL) {
            break;
        }
    }

    // 목적지 리스트에 값을 옮겨적음.
    target_list->name[target_index] = target_table->name;
    target_list->address[target_index] = target_table;
    target_list->num += 1;    

    // 기존 테이블의 값을 초기화
    target_table->table_list->name[index] = NULL;
    target_table->table_list->address[index] = NULL;
    target_table->table_list->num -= 1;

    // table->table_list 값 변경
    target_table->table_list = target_list;
}

void set_table_temp(Table* target, Table_list* temp_list) {
    change_table_status(target, temp_list);
}

void set_table_main(Table* target, Table_list* main_list) {
    change_table_status(target, main_list);
}

void set_table_return(Table* target, Table_list* return_list) {
    // 중복여부 확인
    for (int i = 0; i < return_list->max_size; i++) {
        if (return_list->address[i] == target) {
            return;
        }
    }

    // 비어있는 칸을 확인
    int index;
    for (index = 0; index < return_list->max_size; index++) {
        if (return_list->address[index] == NULL) 
            break;
    }
    if (index == return_list->max_size) {
        show_error_message("set_table_return()", 507);
        return;
    }
    
    // 복사할 테이블의 인덱스
    int target_index = get_index_on_table_list(target);
    // 그 자리에 데이터를 복사해 넣음.
    return_list->name[index] = target->table_list->name[target_index];
    return_list->address[index] = target->table_list->address[target_index];
    return_list->num += 1;
}



Col* new_col(Table* target, char* name) {
    // 임시 커서 객체 생성.
    Cursor* cursor = new_cursor(target);

    // 임시 커서 객체를 이용, 중복되는 열 이름이 있는지 확인.
    if (cursor->pos_table->num_col != 0) {
        while(1) {
            if ( strcmp(cursor->pos_col->name, name) == 0) {
                show_error_message("new_col()", 409);
                return NULL;
            }
            else if (cursor->pos_col->next == NULL) {
                break;
            }
            else {
                move_cursor_col(cursor, 1);
            }
        }
    }

    // 열 객체 동적 할당.
    Col* col = (Col*) malloc(sizeof(Col));
    if (target->num_col == 0)
        target->start_col = col;
    target->num_col += 1;
    
    // 열 객체 초기화.
    col->is_primaryKey = 0;
    col->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(col->name, name);
    if (target->cursor->pos_col != NULL)
        col->prev = target->cursor->pos_col;
    else 
        col->prev = NULL;
    if (target->cursor->pos_col != NULL)
        col->next = target->cursor->pos_col->next;
    else 
        col->next = NULL;
    col->start_data = (Data*) malloc(sizeof(Data));
    
    // 데이터 헤더 정보 초기화
    col->start_data->content = NULL;
    col->start_data->prev = NULL;
    col->start_data->next = NULL;

    // 기존 col 연결 리스트 연결 재정리
    if (col->prev != NULL)
        col->prev->next = col;
    if (col->next != NULL)
        col->next->prev = col;

    // 커서 Data 부분 재할당.
    if (target->cursor->pos_record != NULL)
        free(target->cursor->pos_record);
    target->cursor->pos_record = (Data**) malloc(target->num_col * sizeof(Data*));
    // 커서 초기화
    if (target->cursor->pos_col != NULL) {
        move_cursor_col(target->cursor, 1);
    }
    else {
        target->cursor->pos_col = target->start_col;
    }
    move_cursor_record(target->cursor, 0);

    // 커서를 이용해 Table 구조체에 접근해서 현재 레코드 갯수만큼 더미 데이터 라인 생성
    int index = get_index_by_cursor(target);
    for (int i = 0; i < target->num_record; i++) {
        target->cursor->pos_record[index]->next = (Data*) malloc(sizeof(Data));
        target->cursor->pos_record[index]->next->prev = target->cursor->pos_record[index];
        target->cursor->pos_record[index]->next->next = NULL;
        target->cursor->pos_record[index]->next->content = NULL;
        target->cursor->pos_record[index] = target->cursor->pos_record[index]->next;
    }
    target->cursor->pos_record[index] = target->cursor->pos_col->start_data;

    // 임시 커서 객체 삭제
    delete_cursor(cursor);
}

void delete_col(Table* target) {
    if (target->cursor->pos_col == NULL) {
        show_error_message("delete_col()", 400);
        return;
    }

    // 임시 커서 객체 생성
    Cursor* cursor = new_cursor(target);
    // 해당 열 번호를 반환
    int index = 0;
    index = get_index_by_cursor(target);

    
    // 해당 col에 데이터가 완전히 제거될 때까지 데이터를 삭제.
    while(target->cursor->pos_col->start_data->next != NULL) {        
        move_cursor_record(target->cursor, 0);
        move_cursor_record(target->cursor, 1);
        if (target->cursor->pos_record[index]->next != NULL) {
            target->cursor->pos_record[index]->next->prev = target->cursor->pos_record[index]->prev;
            target->cursor->pos_record[index]->prev->next = target->cursor->pos_record[index]->next;
        }
        else {
            target->cursor->pos_record[index]->prev->next = NULL;
        }
        free(target->cursor->pos_record[index]->content);
        free(target->cursor->pos_record[index]);
    }

    // 헤더 데이터를 삭제
    free(target->cursor->pos_col->start_data->content);
    free(target->cursor->pos_col->start_data);

    // target_col 객체 앞 뒤 노드 연결
    if (target->cursor->pos_col->prev != NULL)
        target->cursor->pos_col->prev->next = target->cursor->pos_col->next;
    if (target->cursor->pos_col->next != NULL) {
        target->cursor->pos_col->next->prev = target->cursor->pos_col->prev;
        if (target->cursor->pos_col == target->start_col)
            target->start_col = target->cursor->pos_col->next;
    }

    // 임시 커서 객체에 해당 col 정보를 저장하고, 원본은 다른 col으로 이동.
    cursor->pos_col = target->cursor->pos_col;
    if (target->cursor->pos_col->next != NULL)
        move_cursor_col(target->cursor, 1);
    else if (target->cursor->pos_col->prev != NULL)
        move_cursor_col(target->cursor, -1);
    else {
        target->cursor->pos_col = NULL;
        free(target->cursor->pos_record);
        target->cursor->pos_record = NULL;
        target->num_record = 0;
    }

    // 최종적으로 col 객체 제거
    free(cursor->pos_col->name);
    free(cursor->pos_col);

    // table.num_col 값 수정
    target->num_col -= 1;

    // table->cursor->pos_record 재할당 및 초기화
    realloc(target->cursor->pos_record, target->num_col * sizeof(Data*));
    move_cursor_record(target->cursor, 0);

    // 동적 할당된 임시 커서 객체 제거
    delete_cursor(cursor);
}

// 미완성
Col* merge_col(Table* target, char* name, Data* data_start) {
    show_error_message("add_col", 501);
    return target->start_col;
}

void move_col(Table* target, char* name) {
    // 임시용 커서 객체 2개
    Cursor* cursor = new_cursor(target);
    Cursor* temp = new_cursor(NULL);

    // 해당 이름을 가진 열이 존재할 시.
    if (move_cursor_by_colName(cursor, name) != NULL) {
        // 자기 자신의 위치에 옮기는지 확인
        if (target->cursor->pos_col == cursor->pos_col){
            show_error_message("move_col()", 409);
            return;
        }

        // 각 열의 앞 뒤 연결 교체.
        if (target->cursor->pos_col->prev != NULL)
            temp->pos_col->prev = target->cursor->pos_col->prev->next;
        else 
            temp->pos_col->prev = NULL;
        if (target->cursor->pos_col->next != NULL) 
            temp->pos_col->next = target->cursor->pos_col->next->prev;
        else 
            temp->pos_col->next = NULL;

        if (target->cursor->pos_col->prev != NULL || cursor->pos_col->prev != NULL)
            target->cursor->pos_col->prev->next = cursor->pos_col->prev->next;
        if (target->cursor->pos_col->next != NULL || cursor->pos_col->next != NULL)
            target->cursor->pos_col->next->prev = cursor->pos_col->next->prev;

        if (cursor->pos_col->prev != NULL)
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
    delete_cursor(temp);
}


Data** new_record(Table* target, char** contents, int num_contents) {
    // 임시 레코드 동적 할당.
    Data** data_arr = (Data**) malloc(target->num_col * sizeof(Data*));

    for (int i = 0; i < target->num_col; i++) {
        // 데이터 객체 동적 생성
        data_arr[i] = (Data*) malloc(sizeof(Data));
        // 데이터 객체의 링커를 현재 커서 데이터에 연결
        data_arr[i]->prev = target->cursor->pos_record[i];
        data_arr[i]->next = target->cursor->pos_record[i]->next;
        // 기존 위치에 있던 데이터의 링크를 새롭게 만든 데이터에 연결
        if (target->cursor->pos_record[i]->next != NULL)
            target->cursor->pos_record[i]->next->prev = data_arr[i];
        target->cursor->pos_record[i]->next = data_arr[i];
        // 데이터 삽입
        if ((i + 1) <= num_contents) {
            data_arr[i]->content = (char*) malloc((strlen(contents[i]) + 1) * sizeof(char));
            strcpy(data_arr[i]->content, contents[i]);
        }
        else {
            data_arr[i]->content = NULL;
        }
        // 기존 커서의 포인터를, 새로이 생성한 데이터 객체로 변경
        target->cursor->pos_record[i] = data_arr[i];
    }
    // num_record 값 증가
    target->num_record += 1;

    // 임시 레코드 메모리 반환.
    free(data_arr);

    // 커서의 해당 부분 반환.
    return target->cursor->pos_record;
}

void delete_record(Table* target) {
    // 레코드가 없는 경우 예외처리
    if (target->num_record == 0) {
        show_error_message("delete_record()", 404);
        return;
    }
    // 커서가 가리키고 있는 데이터가 헤더인 경우 예외처리.
    else if (target->cursor->pos_record[0] == target->start_col->start_data) {
        show_error_message("delete_record()", 400);
        return;
    }

    // 임시 커서 객체 생성
    Cursor* cursor = new_cursor(target);

    // 레코드 삭제
    for (int i = 0; i < target->num_col; i++) {
        // 대상 데이터의 앞 뒤 링크를 절단하고 서로 이어 데이터를 리스트에서 분리.
        target->cursor->pos_record[i]->prev->next = target->cursor->pos_record[i]->next;
        if (target->cursor->pos_record[i]->next != NULL) 
            target->cursor->pos_record[i]->next->prev = target->cursor->pos_record[i]->prev;
        // 다음 데이터가 존재한다면 커서를 다음 데이터로 이동.
        if (target->cursor->pos_record[i]->next != NULL)
            cursor->pos_record[i] = target->cursor->pos_record[i]->next;
        // 아니면 이전 데이터로 커서를 이동.
        else 
            cursor->pos_record[i] = target->cursor->pos_record[i]->prev;
        // 해당 데이터의 메모리를 삭제
        free(target->cursor->pos_record[i]->content);
        free(target->cursor->pos_record[i]);
        // 임시로 받아놨던 대상 위치의 데이터 포인터로 초기화.
        target->cursor->pos_record[i] = cursor->pos_record[i];
    }
    target->num_record -= 1;

    // 임시 커서 객체 제거
    free(cursor);
}

void edit_data(Table* target, char* content) {
    // 열 번호 받기
    int index = get_index_by_cursor(target);

    // 공간 재할당 및 복사
    if (target->cursor->pos_record[index]->content != NULL)
        free(target->cursor->pos_record[index]->content);
    target->cursor->pos_record[index]->content = (char*) malloc((strlen(content) + 1) * sizeof(char));
    strcpy(target->cursor->pos_record[index]->content, content);
}

void edit_allData(Table* target, char* col_name, char* before_text, char* content){
    // 열 이름으로 열 번호 받기
    int index = get_index_by_name(target, col_name);

    // 해당되는 열 이름이 없는 경우 예외처리.
    if (index == -1) {
        show_error_message("edit_allData()", 404);
        return;
    }

    // 커서를 시작으로 이동 후, 순회 검색. (해당 col만)
    move_cursor_record(target->cursor, 0);

    while(target->cursor->pos_record[index] != NULL) {
        // 각 데이터 라인을 순회하며, 대상 데이터가 확인되면, 새로운 데이터로 교체.
        if (strcmp(target->cursor->pos_record[index]->content, before_text) == 0) {
            realloc(target->cursor->pos_record[index]->content, (strlen(content) + 1) * sizeof(char));
            strcpy(target->cursor->pos_record[index]->content, content);
        }
    }
    // 수정이 끝나면 포인터를 다시 헤더를 지칭하도록 조치.
    target->cursor->pos_record[index] = target->cursor->pos_col->start_data;
}

void sort_data(Table* target, char** col_name, int direction) {
    show_error_message("sort_data", 501);
}


Cursor* new_cursor(Table* target) {
    // 커서 객체 동적 생성
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));

    // target이 NULL이 아니라면 target의 데이터로 초기화
    if (target != NULL) {
        cursor->pos_table = target;
        cursor->pos_col = target->start_col;
        cursor->pos_record = (Data**)malloc(target->num_col * sizeof(Data*));
        move_cursor_default(cursor);
    }
    // NULL 이라면 전부 NULL로 초기화.
    else {
        cursor->pos_table = NULL;
        cursor->pos_col = NULL;
        cursor->pos_record = NULL;
    }
    return cursor;
}

void delete_cursor(Cursor* target) {
    // 레코드 포인터 메모리 삭제
    free(target->pos_record);
    // 커서 메모리 삭제
    free(target);
}

Cursor* move_cursor_by_colName(Cursor* target, char* col_name) {
    if (target->pos_table->num_col == 0) {
        return NULL;
    }

    // 시작 열로 커서 이동.
    target->pos_col = target->pos_table->start_col;
    
    int index = 0;
    // 이름이 일치할 때 까지 확인
    while(index < target->pos_table->num_col) {
        if (strcmp(target->pos_col->name, col_name) == 0)
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
    if (target->pos_col == NULL) {
        show_error_message("move_cursor_col", 400);
        return NULL;
    }
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

Cursor* move_cursor_record_by_data(Cursor* target, char* col_name, char* data) {
    // 커서 포인터 정보중 하나라도 NULL 인지 확인.
    if (target == NULL ||
        target->pos_table == NULL ||
        target->pos_col == NULL ||
        target->pos_record == NULL) {
            show_error_message("move_cursor_record_by_data()", 400);
            return NULL;
    }

    // 열 이름으로 커서 이동이 이상없이 된 경우.
    if(move_cursor_by_colName(target, col_name) != NULL) {
        // 커서로 열 번호를 받아옴.
        int index = get_index_by_cursor(target->pos_table);
        // 레코드 커서를 처음으로 이동시킴.
        move_cursor_record(target, 0);
        // 한 칸씩 레코드 커서를 움직이며 일치하는 데이터가 있는지 확인.
        for(int i = 0; i < target->pos_table->num_record; i++) {
            move_cursor_record(target, 1);
            // 일치하면 해당 위치에서 함수를 종료.
            if (strcmp(target->pos_record[index]->content, data) == 0) {
                return target;
            }
        }
        // 일치하지 않으면 다시 초기화 시키고 NULL 반환.
        show_error_message("move_cursor_record_by_data()", 404);
        move_cursor_record(target, 0);        
        return NULL;
    }
    // 아예 해당되는 열 이름이 없는 경우 NULL 반환.
    return NULL;
}

Cursor* move_cursor_record(Cursor* target, int direction) {
    if (target == NULL) {
        show_error_message("move_cursor_record", 400);
        return NULL;
    }
    else if (target->pos_col == NULL) {
        show_error_message("move_cursor_record", 400);
        return NULL;
    }
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
                // 임시로 현재의 col 위치를 기억.
                Col* temp_pos_col = target->pos_col;
                // 커서의 col 부분을 처음으로 전환.
                target->pos_col = target->pos_table->start_col;
                // 커서를 한칸씩 옮겨가며 record 객체의 각 값을 데이터 헤더 값으로 대입
                for (int i = 0; i < target->pos_table->num_col; i++) {
                    target->pos_record[i] = target->pos_col->start_data;
                    move_cursor_col(target, 1);
                }
                // 다시 처음의 자료로 복구
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
    // 임시 커서 객체 생성
    Cursor* cursor = new_cursor(target);
    move_cursor_col(cursor, 0);
    while(1) {
        // 임시 커서의 col 부분 값과, 원본 커서의 col 부분 값이 같으면 루프 탈출.
        if (cursor->pos_col == target->cursor->pos_col)
            break;
        else {
            move_cursor_col(cursor, 1);
            index += 1;
        }
    }

    // 임시 커서 반환
    delete_cursor(cursor);
    
    return index;
}

int get_index_by_name(Table* target, char* name) {
    if (target->num_col == 0) {
        return -1;
    }

    int index = 0;
    // 임시 커서 생성
    Cursor* cursor = new_cursor(target);
    move_cursor_col(cursor, 0);
    while(1) {
        // 임시 커서의 col 부분이 가리키는 열의 이름이 인수랑 동일한 경우 루프 탈출
        if (cursor->pos_col->name == name)
            break;
        // 만약 동일한 이름을 찾지 못하고 전부 순회하면 인덱스를 -1로 설정하고 루프 탈출
        else if (index == target->num_col) {
            show_error_message("get_index_by_name", 404);
            index = -1;
            break;
        }
        // 그 외에는 임시 커서를 한칸씩 옮기며 인덱스를 1씩 증가
        else {
            move_cursor_col(cursor, 1);
            index += 1;
        }
    }

    // 임시 커서 삭제
    delete_cursor(cursor);
    
    return index;

}

int get_index_on_table_list (Table* target) {
    if (target == NULL) {
        return -1;
    }

    int index = 0;
    while(1) {
        // 해당 테이블이 속한 테이블 리스트의 주소부를 순회하며 주소값이 동일한 경우 루프 탈출
        if (target->table_list->address[index] == target) {
            break;
        }
        else {
            index += 1;
        }
    }

    return index;
}

int get_tableIndex_by_name (char* name, Table_list* table_list) {
    int index;
    // 같은 이름을 가진 테이블을 포착할 때 까지 순회.
    for (index = 0; index < table_list->max_size; index++) {
        if (table_list->name[index] == NULL) {
            continue;
        }
        if (strcmp(name, table_list->name[index]) == 0) {
            break;
        }
    }
    // 만약 찾지 못하고 순회를 마친 경우, 인덱스를 -1로 초기화하고 반환.
    if (index == table_list->max_size) {
        index = -1;
    }

    return index;
}

Table_list* get_table_list_by_name(char* name, Table_list** total_list) {
    if (strcmp(name, "main_table_list") == 0) {
        return total_list[0];
    }
    else if (strcmp(name, "temp_table_list") == 0) {
        return total_list[1];
    }
    else if (strcmp(name, "main_table_list") == 0) {
        return total_list[2];
    }
    else {
        return NULL;
    }
}

Table* convert_file_to_table(char* name, Table_list* table_list) {
    // 이미 불러온 파일인지 검사.
    for (int i = 0; i < table_list->max_size; i++) {
        if (table_list->name[i] == NULL)
            continue;
        else if (strcmp(table_list->name[i], name) == 0) {
            show_error_message("convert_file_to_table()", 409);
            return NULL;
        }
    }

    // 파일 객체 동적 생성
    FILE* fp;
    char* url = (char*) malloc(100 * sizeof(char));
    // 작업 환경에서만 필요한 코드. 다른 곳에서 단독으로 실행할 땐 아래 주석 코드로 대체
    // sprintf(url, "%s%s%s", "homeworks\\CollegeProject\\data\\", name, ".txt");
    sprintf(url, "..\\data\\%s.txt", name);
    fp = fopen(url, "r");       // 읽기 모드로 파일 오픈
    // 파일이 존재하는지 확인. 없으면 예외처리
    if (fp == NULL) {
        show_error_message("conver_file_to_table()", 404);
        show_error_message((const char*) url, 100);
        free(url);
        fclose(fp);
        return NULL;
    }
    free(url);

    // 파일이 적합한 양식으로 구성되었는지 확인 및 대입.
    char temp[1000];
    // 파일의 메타 데이터
    // 0 : 테이블의 이름
    // 1 : 파일의 최신 수정 일자. 사용 안함
    // 2 : 열 갯수
    // 3 : 행(레코드) 갯수
    // 4 : 열 목록
    // 5 : 행 목록. 사용안함.
    char meta_data[6][50]; 
    char* compare_text[6] = {
        (char*)"name : ",
        (char*)"date : ", 
        (char*)"num_col : " , 
        (char*)"num_record : ", 
        (char*)"column : ",
        (char*)"data : "
    };
    for (int i = 0; i < 6; i++) { 
        // 한 줄을 입력 받음 : 파일 -> temp
        fflush(stdin);
        fgets(temp, 1000, fp);
        // temp를 앞부분만 검사하여 양식에 맞는지 확인. 일치하지 않으면 예외처리
        if (strncmp(temp, compare_text[i], strlen(compare_text[i])) != 0) {
            show_error_message("convert_file_to_table()", 400);
            fclose(fp);
            return NULL;
        }
        // 일치하면 태그 부분을 제거하고 메타 데이터에 입력
        else {
            strcpy(meta_data[i], strchr(temp, ':') + 2);
            meta_data[i][(strlen(meta_data[i]) - 1)] = '\0';
        }
    }

    // 파일의 이름과 테이블의 이름이 동일한지 확인. 일치하지 않으면 예외처리
    if (strncmp(meta_data[0], name, strlen(name))) {
        show_error_message("convert_file_to_table()", 400);
        fclose(fp);
        return NULL;
    }

    // 테이블 객체 동적 생성.
    Table* table = new_table(meta_data[0], table_list);
    
    // 열 삽입

    // 열 리스트를 임시로 받아둘 동적 문자열 배열 생성.
    char** column_list = (char**) malloc(atoi(meta_data[2]) * sizeof(char*));
    for (int i = 0; i < atoi(meta_data[2]); i++) {
        column_list[i] = (char*) malloc(50 * sizeof(char));
    }

    // column : 이후 문장을 "|" 토큰을 기준으로 전부 잘라내 따로 보관함.
    column_list[0] = strtok(meta_data[4], "|");
    for (int i = 1; i < atoi(meta_data[2]); i++) {
        column_list[i] = strtok(NULL, "|");
    }

    // 해당 데이터로 새로운 열을 생성.
    for (int i = 0; i < atoi(meta_data[2]); i++) {
        new_col(table, column_list[i]);
    }

    // 레코드 삽입
    for (int i = 0; i < atoi(meta_data[2]); i++) {
        realloc(column_list[i], 250 * sizeof(char));
    }

    // 파일의 끝에 도달할 때 까지
    while (fgets(temp, 10000, fp) != NULL) {

        // 위에서 사용한 리스트 재활용.
        // 위와 같은 방법으로 데이터를 잘라 임시 보관.
        column_list[0] = strtok(temp, "|\n");
        for (int i = 1; i < atoi(meta_data[2]); i++) {
            column_list[i] = strtok(NULL, "|\n");
        }        
        // 해당 자료로 새로운 레코드 생성.
        if (column_list[0] == NULL)
            break;
        new_record(table, column_list, atoi(meta_data[2]));
    }

    // 임시 문자열 배열 삭제
    for(int i = 0; i < atoi(meta_data[2]); i++) {
        free(column_list[i]);
    }
    free(column_list);

    // 파일 객체 삭제
    fclose(fp);

    // 테이블 객체 반환
    return table;
}

int convert_table_to_file(Table* target) {
    FILE* fp;

    // 파일의 url
    char url[200];
    // 별도 실행시에는 아래 주석 코드로 대체할 것.
    // sprintf(url, "%s%s%s", "homeworks\\CollegeProject\\data\\", target->name, ".txt");
    sprintf(url, "..\\data\\%s.txt", target->name);
    fp = fopen(url, "wt");
    
    // 메타데이터 입력
    fprintf(fp, "name : %s\n", target->name);
    fprintf(fp, "date : %s\n", "not implement");
    fprintf(fp, "num_col : %d\n", target->num_col);
    fprintf(fp, "num_record : %d\n", target->num_record);
    // 열 이름 입력 구역
    char* temp_col = (char*) calloc(50, sizeof(char));
    move_cursor_default(target->cursor);
    if (target->num_col != 0) {
        while(1) {
            strcat(temp_col, target->cursor->pos_col->name);
            if (target->cursor->pos_col->next != NULL) {
                strcat(temp_col, "|");
                move_cursor_col(target->cursor, 1);
            }
            else
                break;
        }
    }
    fprintf(fp, "column : %s\n", temp_col);
    free(temp_col);

    // 레코드 입력 구역
    // 열 입력과 유사하나, 모든 레코드가 입력될 때까지 같은 동작을 계속 반복.
    fprintf(fp, "data : \n");
    char* temp_record = (char*) calloc(1000, sizeof(char));
    if (target->num_record != 0) {
        while(1) {
            // 임시 문자열 초기화
            strcpy(temp_record, "");
            // 레코드의 끝 검사. 끝이 아니면 커서 이동.
            if (target->cursor->pos_record[0]->next != NULL) {
                move_cursor_record(target->cursor, 1);
            }
            else {
                break;
            }
            // 파일 출력.
            for (int i = 0; i < target->num_col; i++) {
                strcat(temp_record, target->cursor->pos_record[i]->content);
                if (i < target->num_col - 1) {
                    strcat(temp_record, "|");
                }
                else {
                    strcat(temp_record, "\n");
                }
            }
            fprintf(fp, "%s", temp_record);
        }
    }
    free(temp_record);

    fclose(fp);
}

    