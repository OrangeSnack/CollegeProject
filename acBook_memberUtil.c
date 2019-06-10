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

//수정 명령
Table_list* modify(char* id, char* pw) {
    char temp[30] = {0, };

    sprintf(temp, "%s%s%s%s%s%s", "modify ", "(", logined_id, ", ", id, ", ", logined_pw, ", ", pw, ")");

    return order_manager(temp);
}

//가입 명령
Table_list* registeration(char* id, char* pw) {
    char temp[30] = {0, };

    sprintf(temp, "%s%s%s%s%s%s", "register ", "(", id, ", ", pw, ")");

    return order_manager(temp);
}

//로그인 명령
Table_list* login(char* id, char* pw) {
    char temp[30] = {0, };

    sprintf(temp, "%s%s%s%s%s%s", "login ", "(", id, ", ", pw, ")");

    return order_manager(temp);
}
