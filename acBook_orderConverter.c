#include "acBook_orderConverter.h"

Table_list* order_manager(char* order_string) {
    
    // 임시용 테이블 리스트 포인터
    Table_list* temp_list;

    char temp[1000] = {0, };     // 문자열의 일부를 잘라 전달하기 위한 임시 문자 배열
    int i = 0;
    int tag = 0;        // 문자열이 잘린 시점을 임시로 저장할 태그.
    int repeat_check = 0;        // 해당 

    while(1) {
        // ouder_queue 객체 생성 및 초기화
        Order_queue* order = (Order_queue*) malloc(sizeof(Order_queue));
        order->order = NULL;
        order->next = NULL;
        // table_manager로 전달 될 첫 노드를 미리 헤더 포인터로 받아둠.
        Order_queue* order_head = order;

        // 문자열 order 변환
        for (i = 0; ; i++) {
            // '(' 가 입력되면 START 형식의 order로 변형
            if (order_string[tag + i] == '(') {
                temp[i] = '\0';
                order = convert_str_to_order(order, temp, i, START);
                strcpy(temp, "");
                tag = i + 1;
                i = -1;
            }
            // ')'가 입력되면 END 형식의 order로 변경.
            else if (order_string[tag + i] == ')') {
                temp[i] = '\0';
                order = convert_str_to_order(order, temp, i, END);
                strcpy(temp, "");
                tag = i + 1;
                i = -1;
            }
            // 콤마가 입력되면 ADD 형식의 order로 변경.
            else if (order_string[tag + i] == ',') {
                temp[i] = '\0';
                order = convert_str_to_order(order, temp, i, ADD);
                strcpy(temp, "");
                tag = i + 1;
                i = -1;
            }
            // 줄바꿈이나 공백은 입력하지 않고 그냥 넘김.
            else if (order_string[tag + 1] == '\n' ||
                     order_string[tag + 1] == ' ') {
                tag = i + 1;
                i = -1;
            }
            else {
                temp[i] = order_string[tag + i];
            }
        }

        temp_list = table_manager(order_head, repeat_check);

        repeat_check = 1;
        
        // 반환된 리스트의 첫번째 배열(additional_order)에 예약된 명령어가 없다면 탈출.
        if (temp_list->address[0]->num_record == 0) {
            break;
        }
        // 아니라면 해당 배열로 새로운 order_string 을 만들어 반복.
        else {
            strcpy(order_string, "");
            move_cursor_default(temp_list->address[0]->cursor);
            for(int i = 0; i < temp_list->address[0]->num_record; i++) {
                move_cursor_record(temp_list->address[0]->cursor, 1);    
                strcat(order_string, temp_list->address[0]->cursor->pos_record[0]->content);
            }

            i = 0;
            tag = 0;
        }
    }

    return temp_list;
}

// 반복되는 작업 내용을 추출한 중간다리 함수.
// 변환 함수 호출 및 기타 작업을 수행.

Order_queue* convert_str_to_order(Order_queue* order, char* order_string, int string_length, enum order_input_type type) {
    // 새 Order_queue 객체 생성
    Order_queue* new_order = (Order_queue*) malloc(sizeof(Order_queue));
    // 새로운 객체에 연결
    if (order->next != NULL)
        new_order->next = order->next;
    else
        new_order->next = NULL;
    order->next = new_order;
    

    order->order = (char*) malloc(sizeof(char) * string_length);
    strcpy(order->order, order_string);

    if (type == START) {
        return convert_str_to_order(new_order, "(", 2, ADD);
    }
    else if (type == END) {
        return convert_str_to_order(new_order, ")", 2, ADD);
    }
    else {
        return new_order;
    }
}

Table_list* table_manager(Order_queue* order, int repeat_check) {
    static Table_list main_table_list;      // 삭제 혹은 프로그램 종료 전까지 유지되는 테이블 리스트
    static Table_list temp_table_list;      // 매 명령 실행 요청이 들어올 때마다 자동으로 삭제되는 임시 테이블 리스트
    static Table_list return_table_list;    // 반환값으로 전달 될 테이블 리스트. 다른 리스트들의 값을 복사해 이용.
    static int check_first_call = 0;        // 테이블 매니저의 첫 실행을 구분.

    // 테이블 매니저 첫 호출 때 초기화 실시
    if (check_first_call == 0) {
        table_list_default_setting(&main_table_list, 50);
        table_list_default_setting(&temp_table_list, 50);
        table_list_default_setting(&return_table_list, 20);
    }

    // 커스텀 함수로 인한 반복 실행이 아닌 경우
    if (repeat_check == 0) {
        // 임시 테이블 리스트 전체 초기화.
        // 해당 리스트에 테이블이 남아있다면 그 테이블의 메모리를 삭제함.
        for (int i = 0; i < temp_table_list.max_size; i++) {
            if (temp_table_list.address[i] =! NULL) {
                delete_table(temp_table_list.address[i]);
            }
            // 만약 남은 임시 테이블이 없다면 삭제 절차를 중단.
            if (temp_table_list.num == 0)
                break;
        }

        // 반환 테이블 리스트 전체 초기화.
        // 해당 리스트에 테이블이 남아있더라도, 메모리는 조작하지 않음.
        for (int i = 0; i < return_table_list.max_size; i++) {
            if (return_table_list.address[i] != NULL) {
                return_table_list.address[i] = NULL;
                return_table_list.name[i] = NULL;
                return_table_list.num -= 1;
            }
            // 만약 모든 데이터가 제거되었다면 제거 작업 중단.
            if (return_table_list.num == 0) {
                break;
            }
        }
    }
    
    // 명령 수행부.
    order_processor(order, &main_table_list, &temp_table_list, &return_table_list);
    

    return &return_table_list;
}

void order_processor(Order_queue* order, Table_list* main_list, Table_list* temp_list, Table_list* return_list) {
    // 테이블 리스트들을 전달하기 위한 배열
    // 0 : 메인 리스트
    // 1 : 임시 리스트
    // 2 : 반환 리스트
    Table_list* total_list[3] = {main_list, temp_list, return_list};
    // 함수명을 임시로 받을 문자열 포인터
    char* function_string;
    // 매개변수들을 임시로 받을 문자열 배열 포인터. 
    char* parameter_string[20] = {0, };
    // 입력 모드를 표시할 변수
    // 1 : 매개변수 입력 시작.
    // 0 : 매개변수 입력 종료.
    int mode = 0;
    // 매개변수 입력 시 사용할 인덱스 변수
    int index = 0;
    // 커스텀 함수의 사용으로 실행을 반복해야되는지 여부
    int check_custom_function_act = 0;

    while(function_string = read_order_line(order)) {
        index = 0;

        // "(" 이 반환되면 매개변수 입력 시작.
        if ((parameter_string[0] = read_order_line(order)) == "(") {
            free(parameter_string[0]);
            mode = 1;
        }

        // 매개변수 입력 시작
        while(mode == 1) {
            parameter_string[index] = read_order_line(order);
            // ")"이 반환되면 매개변수 입력 종료.
            if (parameter_string[index] == ")") {
                free(parameter_string[index]);
                parameter_string[index] = NULL;
                mode = 0;
            }
            else {
                index += 1;
            }
        }
        // 입력값에 따른 함수 실행
        check_custom_function_act = call_function(function_string, parameter_string, total_list, index);

        // 사용된 모든 동적 문자열 반환.
        free(function_string);
        for(int i = 0; i < 20; i++) {
            // 
            if (parameter_string[i] == NULL) {
                break;
            }
            free(parameter_string[i]);
        }

        // 만약 커스텀 함수가 실행되었다면 중간에 실행을 끊고, 다시 order_manager를 거침.
        if (check_custom_function_act == 1) {
            break;
        }
        else if (check_custom_function_act == 2) {
            show_error_message("order_processor()", 400);
        }
    }
}

// order_queue를 받아, 문자열을 동적 할당하여 반환하고, queue는 다음칸으로 넘기는 함수.
char* read_order_line(Order_queue* order) {
    // 인수로 들어온 Order_queue가 NULL이라면 NULL을 반환.
    if (order == NULL) {
        return NULL;
    }
    // 명령용 문자열 동적 할당.
    char* order_string = (char*) malloc((strlen(order->order)+1) * sizeof(char));

    // 명령어 복사
    strcpy(order_string, order->order);    

    // 해당 queue 반환.
    Order_queue* temp_order = order->next;
    free(order);
    order = temp_order;

    return order_string;
}

// 전달받은 함수명에 적합한 함수를 호출 하는 함수
// 반환값은 없으며, 대신 result_table_list를 수정함.
int call_function(char* function_name, char** parameters, Table_list** total_list, int num_parameters) {
    // 임시용 포인터들
    Table_list* temp_list;
    Table* temp_table;
    int index;
    // 인수의 앞 2칸은 무조건 다음 요소로 사용됨.
    enum parameter_type {
        TABLE_NAME = 0,
        TABLE_LIST_NAME = 1
    }    
    temp_list = get_table_list_by_name(parameters[TABLE_LIST_NAME], total_list);
    index = get_tableIndex_by_name(parameters[TABLE_NAME], temp_list);

    // 기본 함수에서 찾는 구역.
    if (index != -1)
        temp_table = temp_list->address[index];
    // 테이블 생성.
    // 매개변수 : 생성할 테이블의 이름, 생성할 테이블 리스트의 이름
    if (strcmp(function_name, "new_table")) {
        temp_list = get_table_list_by_name(parameters[1], total_list);
        new_table(parameters[TABLE_NAME], temp_list);
    }
    // 테이블 삭제.
    // 매개변수 : 삭제할 테이블의 이름, 삭제할 테이블이 위치한 테이블 리스트의 이름.
    else if (strcmp(function_name, "delete_table")) {
        delete_table(temp_table);
    }
    // 테이블의 속성을 "임시"로 바꾸는 함수.
    // 인수 : 속성을 변경하고 싶은 테이블의 이름, 그 테이블이 속한 테이블 리스트의 이름.
    else if (strcmp(function_name, "set_table_temp")) {
        set_table_temp(temp_table, total_list[1]);
    }
    // 테이블의 속성을 "메인"으로 바꾸는 함수.
    // 인수 : 속성을 변경하고 싶은 테이블의 이름, 그 테이블이 속한 테이블 리스트의 이름.
    else if (strcmp(function_name, "set_table_main")) {
        set_table_main(temp_table, total_list[0]);
    }
    // 해당 테이블을 반환 테이블 리스트에 올리는 함수.
    // 인수 : 반환하고 싶은 테이블의 이름, 그 테이블이 속한 테이블 리스트의 이름
    else if (strcmp(function_name, "set_table_return")) {        
        set_table_return(temp_table, total_list[2]);
    }
    else if (strcmp(function_name, "new_col")) {
        new_col(temp_table, parameters[2]);
    }
    else if (strcmp(function_name, "delete_col")) {
        delete_col(temp_table);
    }
    else if (strcmp(function_name, "merge_col")) {
        // 미구현
    }
    else if (strcmp(function_name, "move_col")) {
        move_col(temp_table, parameters[2]);
    }
    else if (strcmp(function_name, "new_record")) {
        char** temp_contents = (char**) calloc(num_parameters - 2, sizeof(char*));
        for (int i = 0; i < (num_parameters - 2); i++) {
            temp_contents[i] = parameters[i + 2];
        }
        new_record(temp_table, temp_contents, num_parameters - 2);
        free(temp_contents);
    }
    else if (strcmp(function_name, "detele_record")) {
        delete_record(temp_table);
    }
    else if (strcmp(function_name, "edit_data")) {
        edit_data(temp_table, parameters[2]);
    }
    else if (strcmp(function_name, "edit_allData")) {
        edit_allData(temp_table, parameters[2], parameters[3], parameters[4]);
    }
    else if (strcmp(function_name, "sort_data")) {
        // 미구현
    }
    else if (strcmp(function_name, "move_cursor_by_colName")) {
        move_cursor_by_colName(temp_table->cursor, parameters[2]);
    }
    else if (strcmp(function_name, "move_cursor_col")) {
        int temp_direction;
        if (parameters[2][0] == '1')
            temp_direction = 1;
        else if (parameters[2][0] == '0')
            temp_direction = 0;
        else if (parameters[2][0] == '-1')
            temp_direction = -1;
        move_cursor_col(temp_table->cursor, temp_direction);
    }
    else if (strcmp(function_name, "move_cursor_record")) {
        int temp_direction;
        if (parameters[2][0] == '1')
            temp_direction = 1;
        else if (parameters[2][0] == '0')
            temp_direction = 0;
        else if (parameters[2][0] == '-1')
            temp_direction = -1;
        move_cursor_record(temp_table->cursor, temp_direction);
    }
    else if (strcmp(function_name, "move_cursor_default")) {
        move_cursor_default(temp_table->cursor);
    }
    else if (strcmp(function_name, "convert_file_to_table")) {
        convert_file_to_table(parameters[TABLE_NAME], temp_list);
    }
    else if (strcmp(function_name, "convert_table_to_file")) {
        convert_table_to_file(temp_table);
    }
    // 비교 연산
    else if (strcmp(function_name, "operator_in")) {
        Table_list* temp_condition_list = get_table_list_by_name(parameters[3], total_list);
        Table* temp_condition_table = get_tableIndex_by_name(parameters[2], temp_condition_list);
        operator_in(temp_table, temp_condition_table, total_list);
    }
    else if (strcmp(function_name, "")) {

    }
    else {
        // custom_table을 불러옴.
        int function_table_index = get_tableIndex_by_name("custom_function", total_list[0]);
        Table* function_table = total_list[0]->address[function_table_index];
        move_cursor_default(function_table->cursor);

        // 값 전달용 한칸짜리 char*형 배열.
        char* temp_string[1];

        int i;
        // custom_table에서 해당 함수 명을 검색하여, 존재하면 다음열의 데이터를 additional_order 테이블에 옯겨적음.
        for (i = 0; i < function_table->num_record; i++) {
            move_cursor_record(function_table->cursor, 1);
            new_col(total_list[1]->address[0], "order");
            if (strcmp(function_table->cursor->pos_record[0]->content, function_name) == 0) {
                temp_string[0] = function_table->cursor->pos_record[0]->content;
                new_record(total_list[2]->address[0], temp_string, 1);
                break;
            }
        }
        if (i == function_table->num_record) {
            show_error_message("call_function()", 400);
            return 2;
        }

        return 1;
    }

    return 0;

    // 없으면 function 테이블에서 해당 함수의 정의가 있는지 확인.
        // 있으면 해당 부분만 잘라서 임시 리스트에 "custum_function" 테이블을 만들어 기록, return_table_list에도 기록하여 반환.
        // 이렇게 반환된 테이블은 order_manager에서 포착하여 다시 해석을 돌리고 최종 처리까지 대기
        // 이 과정에서 유지되어야 할 임시 테이블이 있다면 해당 테이블을 잠시 메인 리스트에 올리고, 최종 실행 때 다시 임시로 내림.
    // 아예 해당 함수를 찾을 수 없다면 예외처리하고 오류 테이블을 반환함.
}