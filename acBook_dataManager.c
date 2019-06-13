#include "acBook_dataManager.h"

extern char logined_id[MAXNUM];

void acBook_mainPage() {
    if (strcmp(logined_id, "") == 0) {
        system("cls");  
        printf("죄송합니다.\n 이 기능은 로그인을 하셔야 이용하실 수 있습니다.\n");
        printf("아무 키나 누르면 메인메뉴로 돌아갑니다.\n");
        getch();
    }
    else {
        while(1) {
            system("cls");
            printf("    거래 목록\n");

            // 해당 구역 탈출 확인
            int out_check = 0;

            // 유저 아이디에 맞는 거래목록을 받아옴.
            char order_string[100];
            sprintf(order_string, "get_acBook(acBook_%s)", logined_id);
            Table_list* return_list = order_manager(order_string);

            // 만약 기존 기록이 없다면 새롭게 빈 가계부를 만듬.
            if (return_list->address[1] == NULL) {
                sprintf(order_string, "new_acBook(acBook_%s)", logined_id);
                return_list = order_manager(order_string);
            }

            while(1) {
                // 에러 메세지 체크
                // 0 : 이상 없음
                // -1 : 더 이상 확인할 수 있는 이전 레코드가 없음.
                // +1 : 더 이상 확인할 수 있는 다음 레크드가 없음
                int error_check = 0; 
                // 선택을 체크하는 함수.
                int select_check = 0; 
                // 임시로 시리얼 넘버의 주소를 받아둘 문자열 포인터 배열
                char* serial_num[20];
                // 임시로 사용할 테이블 포인터
                Table* table = return_list->address[1];
                // 소계용 데이터
                int num = 0;            // 거래 갯수.
                int total_out = 0;      // 총 지출
                int total_in  = 0;      // 총 수익
                int total_sum = 0;      // 총계
                int credit_index;       // 금액란 인덱스

                // 소계 데이터 삽입
                move_cursor_default(table->cursor);
                credit_index = get_index_by_name(table, (char*)"credit");
                while(1) {
                    int temp;
                    if (table->cursor->pos_record[credit_index]->next != NULL) {
                        table->cursor->pos_record[credit_index] = table->cursor->pos_record[credit_index]->next;
                        if ((temp = atoi(table->cursor->pos_record[credit_index]->content)) >= 0) {
                            total_in += temp;
                        }
                        else if ((temp = atoi(table->cursor->pos_record[credit_index]->content)) < 0) {
                            total_out += temp;
                        }
                        total_sum += temp;
                        num += 1;
                    }
                }
                move_cursor_default(table->cursor);
                // 거래 내역 출력.
                acBook_show_list(table, num, total_in, total_out, total_sum);
                // 해당 20칸의 시리얼 넘버를 임시로 받아옴.
                for (int i = 0, j = 0; ; i++) {
                    if (table->cursor->pos_record == NULL) {
                       serial_num[i] = NULL;
                    }
                    else if (table->cursor->pos_record[0]->next == NULL) {
                        serial_num[i] = NULL;
                    }
                    else {
                        move_cursor_record(table->cursor, 1);
                        serial_num[i] = table->cursor->pos_record[0]->content;
                        j += 1;
                    }
                    if (i == 20) {
                        // 모든 순회 이후 커서는 다시 검색 시작점으로.
                        for ( ; j > 0; j--) {
                            move_cursor_record(table->cursor, -1);
                        }
                        break;
                    }
                }
                // 이전 입력에 따른 오류 메세지를 처리.
                if (error_check == -1) {
                    printf("이미 최초 목록입니다.\n");
                }
                else if (error_check == 1) {
                    printf("더 이상 확인되는 거래 목록이 없습니다.\n");
                }

                // Ui 로 선택하도록 함.
                // 1 : 이전 라인으로.
                // 2 ~ 21 : 해당 위치의 레코드에 접근
                // 22 : 다음 라인으로.
                // 23 : 가계부 모드 종료.
                select_check = selectUi(4, 2, 23);    
                if (select_check == 1) {
                    for (int i = 0, j = 0; i < 20; i++) {
                        if (table->cursor->pos_record == NULL) {
                            error_check = -1;
                            break;
                        }
                        else if (table->cursor->pos_record[0]->prev == NULL) {                            
                            error_check = -1;
                            for ( ; j > 0; j--) {
                                move_cursor_record(table->cursor, 1);
                            }
                            break;
                        }
                        else {
                            move_cursor_record(table->cursor, -1);
                            j += 1;
                        }
                    }
                }
                else if (select_check > 1 && select_check <= 21) {
                    if (serial_num[select_check - 1] == NULL)
                        acBook_add_list(serial_num[select_check - 1], table);
                    else 
                        acBook_edit_list(serial_num[select_check - 1], table);
                }
                else if (select_check == 22) {
                    for (int i = 0, j = 0; i < 20; i++) {
                        if (table->cursor->pos_record == NULL) {
                            error_check = 1;
                            break;
                        }
                        else if (table->cursor->pos_record[0]->next == NULL) {                            
                            error_check = 1;
                            for ( ; j > 0; j--) {
                                move_cursor_record(table->cursor, -1);
                            }
                            break;
                        }
                        else {
                            move_cursor_record(table->cursor, 1);
                            j += 1;
                        }
                    }
                }
                else if (select_check == 23) {
                    out_check = 1;
                    break;
                }
            }

            if (out_check == 1) 
                break;
        }
    }
}

void acBook_show_list(Table* table, int num, int total_in, int total_out, int total_sum) {
    move_cursor_col(table->cursor, 0);
    for (int i = 0; i < table->num_col; i++)
        printf("%s\t", table->cursor->pos_col->name);
    printf("\n==========================================\n");
    for (int i = 0, j = 0; ; i++) {
        printf("\t");
        // 다음 행이 비어있지 않은 경우
        if (table->cursor->pos_record != NULL || table->cursor->pos_record[0]->next != NULL) {
            for (int k = 0; k < table->num_col; k++) {
                // 해당 레코드 포인터가 NULL 이거나, 혹은 데이터가 NULL 인 경우.
                if (table->cursor->pos_record != NULL) {
                    if (table->cursor->pos_record[k]->content != NULL)
                        printf("%s\t", table->cursor->pos_record[k]->content);
                    else 
                        printf(".\t");
                }
                else
                    printf(".\t");
            }
            printf("\n");     
            j += 1;   
        }
        else {
            printf("-\n");
        }

        // 커서 복구
        if (i == 20) {
            for ( ; j > 0; j++) {
                move_cursor_record(table->cursor, -1);
            }
        }
    }
    printf("\n==========================================\n");
    printf("총 거래 갯수 : %d\n", num);
    printf("총 수익 : %d\n", total_in);
    printf("총 지출 : %d\n", total_out);
    printf("총계 : %d\n", total_sum);
}

void acBook_add_list(char* serial_num, Table* table) {
    //printf("미구현된 기능입니다.\n");
}

void acBook_edit_list(char* serial_num, Table* table) {
    //printf("미구현된 기능입니다.\n");
}
