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

//수정 명령
int modify(char* id, char* pw) {
    char temp[30] = {0, };

    sprintf(temp, "%s%s%s%s%s%s", "modify ", "(", logined_id, ", ", id, ", ", logined_pw, ", ", pw, ")");

    order_manager(temp);

    return modifyBool();
}

//가입 명령
int registeration(char* id, char* pw) {
    char temp[30] = {0, };

    sprintf(temp, "%s%s%s%s%s%s", "register ", "(", id, ", ", pw, ")");

    order_manager(temp);

    return registerationBool();
}

//로그인 명령
int login(char* id, char* pw) {
    char temp[30] = {0, };

    sprintf(temp, "%s%s%s%s%s%s", "login ", "(", id, ", ", pw, ")");

    order_manager(temp);

    return loginBool();
}

//로그인 명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int loginBool()
{
  return TRUE;
}

//회원가입 명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int registerationBool()
{
  return TRUE;
}

//회원정보수정명령 성공여부 분석해서 TRUE, FALSE 반환해주는 함수
int modifyBool()
{
  return TRUE;
}
