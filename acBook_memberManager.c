#include "acBook_memberManager.h"

//두 문자열 입력받고 다르면 0, 같으면 1 출력
int compare(char *p1, char *p2)
{
  if(strcmp(p1,p2) == 0)
    return 1;
  else
    return 0;
}

//문자열 입력받고 p1에다가 복사
//secure에 1 입력시 입력받는 문자들을 *로 출력
void inputSys(const int secure, char *p1)
{
	char s1[20] = { '\0' };
	char c1 = '\0';
	char temp[2] = { '\0', '\0'};

	while(c1 != 13)
	{
		c1 = getch();
		if(c1 == 13)
			break;
    else if(c1 == 8)
      continue;

		temp[0] = c1;
    if(secure == 1)
      printf("*");
    else
      printf("%c", c1);

		strcat(s1, temp);
	}

	strcpy(p1, s1);
}

//로그인 성공여부 체크
//성공시 1, 실패시 0 출력
int loginCheck(char *id, char *pw, char *rid, char *rpw)
{

}

//로그인 시스템
//성공시 1, 실패시 0 출력
int loginSys()
{
  char id[20] = {'\0'};
  char pw[20] = {'\0'};

  //ID 입력
  printf("ID : ");
  inputSys(0, id);

  printf("\n");

  //PW 입력
  printf("PW : ");
  inputSys(1, pw);

  printf("\n");

  if(loginCheck() == 1)
    return 1;
  else
    return 0;
}

//로그인 메인함수
int loginMain()
{

}
