#include "acBook.h"

void order_test();

int main() {
    system("chcp 65001");
    system("cls");
    /*
    // 정석 유니코드 사용법.    
    // 혹시 문자열 처리가 제대로 되지 않으면 참고
    _wsetlocale(LC_ALL, L"korean");

    wchar_t testString[100] = L"유니코드 테스트 코드";
    printf("%S\n", testString);
    */

    int selection = 0;


    
    while(1)
    {
        system("cls");
        selection = mainMenu();

        if(selection == 1) {
            selection = loginMenu();

            if(selection == 1) {
                system("cls");
                loginMain();
                system("pause");
            }
            else if(selection == 2) {
                system("cls");
                registerMain();
                system("pause");
            }
            else if(selection == 3) {
                system("cls");
                modifyMain();
                system("pause");
            }
            else {
                system("cls");
            }

            continue;
        }
        else if(selection == 2) {
            system("cls");
            printf("\n구현안된 항목입니다..\n메인메뉴로 돌아갑니다\n");
            system("pause");
            system("cls");
            continue;
        }
        else if (selection == 3) {
            order_test();
        }
        else {
            order_manager((char*)"end_program()");
            break;
        }        
    }

    printf("\n프로그램 종료\n");
    system("pause");
    return 0;
}

// 테스트용 함수.
// 명령어를 직접 입력해볼 수 있음.
void order_test() {
    char* temp_string = (char*) calloc(1000, sizeof(char));
    char temp_input[100];

    system("cls");

    Table_list* temp_list;

    while(1) {
        strcpy(temp_string, "");
        while (1) {
            fflush(stdin);
            printf("테스트용 입력(; 입력시 종료) : ");
            scanf("%[^\n]", temp_input);
            if (temp_input[0] == ';')
                break;
            else 
                strcat(temp_string, temp_input);
        }
        if (temp_string[0] == 'q')
            break;
        printf("%s\n", temp_string);

        temp_list = order_manager(temp_string);
        if (strcmp(temp_string, "test()") == 0) {

            Table* temp_table = temp_list->address[1];
            move_cursor_default(temp_table->cursor);

            printf("테이블 시작\n");
            printf("%s\n", temp_table->name);
            for(int i = 0; i < temp_table->num_col; i++) {
                printf("%s\t", temp_table->cursor->pos_col->name);
                move_cursor_col(temp_table->cursor, 1);
            }
            printf("\n%s\n", "====================================");

            for(int i = 0; i < temp_table->num_record; i++) {
                move_cursor_record(temp_table->cursor, 1);
                for (int j = 0; j < temp_table->num_col; j++) {
                    printf("%s\t", temp_table->cursor->pos_record[j]->content);
                }
                printf("\n");
            }
            printf("테이블 끝\n");
        }
    }

    free(temp_string);
}