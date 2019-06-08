#include "acBook_memberModify.h"

//메인 함수
int modifyMain()
{
  char* id[MAXNUM];
  char* pw[MAXNUM];

  willModify();

  if(login_status)
  {
    printf("\n아이디 변경 : ");
    inputSys(0, id);
    printf("\n비밀번호 변경 : ")
    inputSys(0, pw);

    modify(id, pw);
    printf("\n변경이 완료되었습니다!\n");
  }
  else
    printf("\n 오류! 변경할 회원정보가 없습니다!\n");

  return 0;
}

//
void willModify()
{
  if(login_status)
    printf("\n로그인 되어있습니다!\n");
  else
  {
    printf("\n로그인 되어있지 않습니다!\n");
    loginSys();
  }
}

//Y or N 선택 함수
int select(const char ch)
{
  switch (ch) {
    case 'y':
      return 1;
    case 'Y':
      return 1;
    case 'n':
      return 0;
    case 'N':
      return 0;
    default:
      return 0;
  }
}
