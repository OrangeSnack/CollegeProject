#include "acBook_processingData.h"

Table* operator_in (Table* target, Table* condition, Table_list** total_list) {
    // 각 실행의 번호를 지정하는 정적 변수
    static int result_num = 0;
    result_num += 1;
    if (result_num == 10000)
        result_num = 0;

    // 각 테이블의 커서를 초기화
    move_cursor_default(target->cursor);
    move_cursor_default(condition->cursor);

    // condition 테이블의 컬럼과 이름이 같은 컬럼으로 target의 커서를 이동.
    // 만약 해당 이름과 동일한 column이 없다면 NULL을 반환.
    if (move_cursor_by_colName(target->cursor, condition->cursor->pos_col->name) == NULL) {
        show_error_message("operator_in()", 404);
        return NULL;
    }
    
    // 커서위치에 대한 인덱스를 받아옴.
    int index = get_index_by_cursor(target);

    // 확인용 임시 int형 동적배열
    int* num_element = (int*) calloc(condition->num_record, sizeof(int));
    // target 커서를 한칸씩 순회시키면서 일치하는 값이 있는지 확인.
    // 일치하는 값이 있다면, 위에 새롭게 추가한 테이블에 반영.
    for (int i = 0; i < target->num_record; i++) {
        // 한 칸 옮김.
        target->cursor->pos_record[index] = target->cursor->pos_record[index]->next;
        for (int j = 0; j < condition->num_record; j++) {
            condition->cursor->pos_record[0] = condition->cursor->pos_record[0]->next;
            // 값이 같으면 해당 인덱스의 값을 1 올림
            if (strcmp(target->cursor->pos_record[index]->content, condition->cursor->pos_record[0]->content)) {
                num_element[j] += 1;
            }
        }
    }

    // 순회가 끝나면 커서를 다시 초기화
    target->cursor->pos_record[index] = target->cursor->pos_col->start_data;
    condition->cursor->pos_record[0] = condition->cursor->pos_col->start_data;

    // 검색된 내용이 있는지를 확인
    int checker = 0;
    for(int i = 0; i < condition->num_record; i++) {
        if (num_element[i] > 0) {
            checker += 1;
        }
    }
    // 만약 하나도 검색된 사항이 없다면 NULL 반환
    if (checker == 0) {
        return NULL;
    }

    // 임시 테이블 리스트에 테이블을 하나 추가로 생성.
    // 이름은 condition 테이블을 참고해서 코드넘버식으로 부착.
    char* temp_table_name = (char*) malloc((strlen(condition->name) + 1 + 4 + 4) * sizeof(char));
    sprintf(temp_table_name, "%s_in_%d", condition->name, result_num);
    Table* temp_table = new_table(temp_table_name, total_list[1]);
    // 내용은 condition과 유사하나, 해당 요소가 얼마나 확인되었는지 체크하는 컬럼이 하나 더 추가됨.
    char* temp_contents[1];
    new_col(temp_table, condition->cursor->pos_col->name);
    for (int i = 0; i < condition->num_record; i++) {
        temp_contents[0] = condition->cursor->pos_record[0]->content;
        new_record(temp_table, temp_contents, 1);
    }
    new_col(temp_table, (char*)"num");

    // 임시 저장되었던 배열의 값을 전달.
    char* temp;
    move_cursor_by_colName(temp_table->cursor, (char*)"num");
    for (int i = 0; i < temp_table->num_record; i++) {
        move_cursor_record(temp_table->cursor, 1);
        sprintf(temp, "%d", num_element[i]);
        edit_data(temp_table, temp);
    }

    // 해당 테이블의 정보를 반환 리스트에 올림.
    set_table_return(temp_table, total_list[2]);

    // 임시로 동적할당한 객체들 전부 제거.
    free(num_element);

    // 임시 테이블 객체를 반환.
    return temp_table;
}