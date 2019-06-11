#include "acBook.h"

int main() {
    system("chcp 65001");
    system("cls");
    /*
    // 정석 유니코드 사용법.
    // 혹시 문자열 처리가 제대로 되지 않으면 참고
    _wsetlocale(LC_ALL, L"korean");

    wchar_t testString[100] = L"유니코드 테스트 코드";
    printf("%S\n", testString);
    */

    int selection = 0;


	   while(1)
	    {
		      system("cls");

          while(1)
    		{
          selection = mainMenu();

          if(selection == 1)
          {
            system("cls");
            break;
          }
          else if(selection == 2)
          {
            system("cls");
      			printf("\n구현안된 항목입니다..\n메인메뉴로 돌아갑니다\n");
      			system("pause");
      			system("cls");
      			continue;
          }
          else
            exit(1);
          }

          if(selection == 1)
          {
            selection = loginMenu();

            if(selection == 1)
            {
              system("cls");
              loginMain();
              system("pause");
            }

			else if(selection == 2)
			{
				system("cls");
				registerMain();
        system("pause");
			}

			else if(selection == 3)
			{
				system("cls");
				modifyMain();
        system("pause");
			}

			else
			{
				system("cls");
				continue;
			}
		}
	}

    system("pause");
    return 0;
}
