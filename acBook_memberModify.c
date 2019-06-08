#include "acBook_memberModify.h"

//메인 함수
int modifyMain()
{
  char* id[MAXNUM];
  char* pw[MAXNUM];

  willModify();


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
