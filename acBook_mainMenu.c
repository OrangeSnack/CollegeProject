#include "acBook_mainMenu.h"

extern int logined_id[MAXNUM];

//첫 페이지
int mainMenu(void)
{
  int selection = 0;

	printf("----------------------------------------------------------------\n");
	printf("\n");
	printf("\t\t\t메인 메뉴\n");
  printf("\t\t\t\t\t 현재계정 : %s", logined_id);

	printf("\n\n");

	printf("\t 1.로그인\n");
	printf("\t 2.가계부\n");
	printf("\t 3.종료\n");

	selection = selectUi(0,5,3);

	return selection;
}

/*
//메인메뉴 선택 링크 함수
void mainSelection(int selection)
{
  switch (selection) {
    case 1:
      system("cls");
      loginMenu();
      break;

    case 2:
      system("cls");
      printf("\n아직 구현되지 않은 대상입니다\n");
      system("pause");
      system("cls");
      mainMenu();
      break;

    case 3:
      exit(1);
      break;

    default:
      system("cls");
      printf("\n알수없는 오류로 다시 시작합니다\n");
      system("pause");
      system("cls");
      mainMenu();
  }
}
*/

//로그인 메뉴
int loginMenu(void)
{
  int selection = 0;

  printf("----------------------------------------------------------------\n");
  printf("\n");
  printf("\t\t\t로그인 메뉴\n");
  printf("\t\t\t\t\t 현재계정 : %s", logined_id);

  printf("\n\n");

  printf("\t 1.로그인\n");
	printf("\t 2.회원가입\n");
	printf("\t 3.회원수정\n");
  printf("\t 4.돌아가기\n");

  selection = selectUi(0,5,4);

  return selection;
}

/*
//로그인 선택 링크 함수
void loginSelection(int selection)
{
  switch (selection) {
    case 1:
      system("cls");
      loginMain();
      break;

    case 2:
      system("cls");
      registerMain();
      break;

    case 3:
      system("cls");
      modifyMain();
      break;

    case 4:
      system("cls");
      mainMenu();

    default:
      system("cls");
      printf("\n알수없는 오류로 다시 시작합니다\n");
      system("pause");
      mainMenu();
  }
}
*/
