#include "acBook_memberUtil.h"

//로그인 상태를 알려주는 변수
int login_status = 0;

// 저장된 ID,PW
char logined_id[MAXNUM] = {'\0'};
char logined_pw[MAXNUM] = {'\0'};

/*
//ID DB에 출력해주는 함수
char* idOut(char *id)
{
  return id;
}

//PW DB에 출력해주는 함수
char* pwOut(char *pw)
{
  return pw;
}
*/

//////양희찬//////

//로그인 명령
int login(char* id, char* pw) {
    char temp[40] = {0, };

    sprintf(temp, "login_check(%s, %s)", id, pw);

    Table_list* return_table_list = order_manager(temp);

    return loginBool(return_table_list);
}

//가입 명령
int registeration(char* id, char* pw) {
    char temp[40] = {0, };

    Table_list* return_table_list;

    sprintf(temp, "register_check(%s, %s)", id, pw);
    return_table_list = order_manager(temp);

    if (registerationBool(return_table_list) == FALSE) {
        return FALSE;
    }

    sprintf(temp, "register(%s, %s)", id, pw);
    order_manager(temp);

    return TRUE;
}

//수정 명령
int modify(char* id, char* pw) {
    char temp[60] = {0, };

   
    sprintf(temp, "modify(%s, %s, %s, %s)", logined_id, id, logined_pw, pw);
    order_manager(temp);
    return TRUE;
}



//로그인 명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int loginBool(Table_list* return_table_list)
{
    Table* temp_table_id_check = return_table_list->address[1];
    Table* temp_table_pw_check = return_table_list->address[2];
    int checker = 0;
    if (temp_table_id_check != NULL) {
        move_cursor_default(temp_table_id_check->cursor);
        move_cursor_record(temp_table_id_check->cursor, 1);
        if (atoi(temp_table_id_check->cursor->pos_record[1]->content) > 0)
            checker += 1;
    }
    else {
        return FALSE;
    }

    if (temp_table_pw_check != NULL) {
        move_cursor_default(temp_table_pw_check->cursor);
        move_cursor_record(temp_table_pw_check->cursor, 1);
        if (atoi(temp_table_pw_check->cursor->pos_record[1]->content) > 0)
            checker += 1;
    }
    else {
        return FALSE;
    }


    if (checker == 2)
        return TRUE;
    else 
        return FALSE;
}


//회원가입 명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int registerationBool(Table_list* return_table_list)
{
    Table* temp_table_check = return_table_list->address[1];

    if (temp_table_check == NULL)
        return TRUE;
    else 
        return FALSE;
}

//회원정보수정명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int modifyBool(Table_list* return_table_list)
{
    return TRUE;
}
