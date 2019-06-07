#include "acBook_memberModify.h"

//메인 함수
int modifyMain()
{
  char* id[MAXNUM];
  char* pw[MAXNUM];

  while(1)
  {
    if(loginMain())
      break;
    else
    {
      printf("\n다시 하시겠습니까? (y/n) : \n");
      if(select(getch()))
      {
        system("cls");
        continue;
      }
      else
       return 0;
    }
  }

  return 0;
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
