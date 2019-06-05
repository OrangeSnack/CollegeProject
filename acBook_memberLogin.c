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

  //로그인 성공여부
  int check = /* loginCheck(id,pw) */;

  if(check == TRUE)
    printf("로그인 성공!!\n");
  else
    printf("로그인 실패..\n");

  return check;
}

//문자열 입력받고 p1에다가 복사
//secure에 1 입력시 입력받는 문자들을 *로 출력
void inputSys(const int secure, char *p1)
{
	char s1[MAXNUM] = { '\0' };
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

//로그인 입력부 출력
//복사받을 문자열주소, 입력할 개체, 보안모드
void loginSys(char *p1, const char *str, const int secure)
{
  char arr[MAXNUM] = {'\0'};

  printf("%s : ", str);
  inputSys(secure, arr);

  strcpy(p1, arr);
}

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
