#include "acBook_memberRegister.h"


//회원가입 메인함수
int registerMain()
{
  char id[MAXNUM] = {'\0'};
  char pw[MAXNUM] = {'\0'};
  char temp[MAXNUM] = {'\0'};

  printf("ID : ");
  inputSys(0, id);

  printf("\n");

  printf("PW : ");
  inputSys(0, pw);

  registeration(id, pw);

  printf("\n\n회원가입이 완료되었습니다!\n");

  return 0;
}

/*
//아이디 중복 확인 함수
void idCheck(char *id)
{
  while(ID비교 결과 출력 함수 == TRUE)
  {
    printf("\n같은ID가 존재합니다!!\n");
    printf("ID : ");
    inputSys(0, id);
  }
}
*/
