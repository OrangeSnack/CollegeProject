#include "acBook_memberLogin.h"

//로그인 메인함수
int loginMain()
{
  char id[MAXNUM] = {'\0'};
  char pw[MAXNUM] = {'\0'};

  //id 입력
  loginSys(id, "ID", 0);
  printf("\n");

  //pw 입력
  loginSys(pw, "PW", 1);
  printf("\n");

  //DB에 ID,PW 전송
  idOut(id);
  pwOut(pw);

  //로그인 성공여부
  int check = /* loginCheck(id,pw) */;

  if(check == TRUE)
    printf("로그인 성공!!\n");
  else
    printf("로그인 실패..\n");

  return check;
}

//로그인 입력부 출력
//복사받을 문자열주소, 입력할 개체, 보안모드
void loginSys(char *p1, const char *str, const int secure)
{
  char arr[MAXNUM] = {'\0'};

  printf("%s : ", str);
  inputSys(secure, arr);

  strcpy(p1, arr);
}
