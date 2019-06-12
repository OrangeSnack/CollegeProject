#include "acBook_util.h"


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
  
	strcpy(p1, strtok(s1, "\n"));
}

//두 문자열 입력받고 다르면 0, 같으면 1 출력
int compare(char *p1, char *p2)
{
  if(strcmp(p1,p2) == FALSE)
    return TRUE;
  else
    return FALSE;
}

//커서위치 이동
void gotoxy(int x, int y)
{
  COORD Pos;
  Pos.X = x;
  Pos.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Pos);
}

//키보드 방향키 출력
//0: 다른것, 1: ↑, 2: ←, 3: →, 4: ↓, 5: 엔터, 6: Esc
int kbArrow()
{
  int key;

  if(kbhit())
  {
    key = getch();
    if(key == 224 || key == 0)
    {
      key = getch();
      switch (key)
      {
        case 72:
          return 1;
          break;
        case 75:
          return 2;
          break;
        case 77:
          return 3;
          break;
        case 80:
          return 4;
          break;
        default:
          return 0;
          break;
      }
    }
    else if(key == 13)
      return 5;
    else if(key == 27)
      return 6;
    else
     return 0;
  }
}

//selectUi 하위함수
int checkOverLine(int line, int current)
{
  if(current < 0 || current > line-1)
    return TRUE;
  else
    return FALSE;
}

//키보드로 입력받는 선택GUI. x,y좌표, 선택할수 있는 라인 갯수를 입력
//사용하기전 사용할 라인에 \t를 하나씩 출력해놓아야함
//선택한 라인번수 출력 (1~line)
//Esc버튼 누를시 0 출력
int selectUi(int x, int y, int line)
{
  int currentSelection = 0;
  int keyinput;

  for(int i = 0; i <= line; i++)
  {
    gotoxy(x, y+i);
    printf("\t");
  }

  gotoxy(x, y);
  printf(">>");

  while(1)
  {
    keyinput = kbArrow();
    if(keyinput == 4)
    {
      if(!checkOverLine(line, currentSelection+1))
      {
        gotoxy(x, y+currentSelection);
          printf("%c%c",32, 32);
          currentSelection++;
          gotoxy(x, y+currentSelection);
          printf(">>");
  }
    }
    else if(keyinput == TRUE)
    {
      if(!checkOverLine(line, currentSelection-1))
      {
        gotoxy(x, y+currentSelection);
          printf("%c%c",32, 32);
          currentSelection--;
          gotoxy(x, y+currentSelection);
          printf(">>");
  }
    }
    else if(keyinput == 5)
      return currentSelection+1;
    else if(keyinput == 6)
      return 0;
  }
}
/////////////////////////////////////////////////////////////
// 강성근 작성 구역


/////////////////////////////////////////////////////////////
// 김강민 작성 구역


/////////////////////////////////////////////////////////////
// 양희찬 작성 구역
