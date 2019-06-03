#include<stdio.h>
#include<conio.h>
#include<string.h>

int main()
{

	char studentID[] = "", password[] = "", id[8], p[6]; //추후 데이터베이스 활용하여 로그인  
	int n = 1, x, y;

	do {
		printf("\nStudent_ID:");
		scanf_s("%s", &id);

		printf("\nPassword:");
		scanf_s("%s", &p);

		x = strcmp(id, studentID);
		y = strcmp(p, password);

		if (x == 0 && y == 0) {
			printf("\nSucessfully Logged In");
		}
		else {
			printf("\nWrong Password, try again", 5 - n);
			getch();
			n++;
		}

		if (n > 5) {
			printf("\nAccess Denied");
			getch();
		}

	} while (n <= 5);
}
