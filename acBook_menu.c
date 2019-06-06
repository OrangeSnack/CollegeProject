#include <acbook.h>
#include "stdio.h" 
#include "conio.h" 
#include "string.h" 

typedef struct UserInformation {
	char id[16];
	char password[16];
	char address[40];
	char name[16];
	char registration_number[24];
} UI;

#define MAX_USER_DATA  100 

UI g_user_data[MAX_USER_DATA];
int g_user_count;

void loaduserdata()
{
	int index = 0;
	FILE *p_file = fopen("user.dat", "rb");
	if (p_file != NULL) {
		fread(&g_user_count, sizeof(int), 1, p_file);
		for (index = 0; index < g_user_count; index++) {
			fread(g_user_data + index, sizeof(UI), 1, p_file);
		}
		fclose(p_file);
	}
}

void saveuserdata()
{
	int index = 0;
	FILE *p_file = fopen("user.dat", "wb");
	if (p_file != NULL) {
		fwrite(&g_user_count, sizeof(int), 1, p_file);
		for (index = 0; index < g_user_count; index++) {
			fwrite(g_user_data + index, sizeof(UI), 1, p_file);
		}
		fclose(p_file);
	}
}

void getpassword(char parm_password[])
{
	int input = 0, index = 0;
	while (input != 13) {
		input = getch();
		if (input == 13) parm_password[index] = 0;
		else {
			printf("*");
			parm_password[index++] = input;
		}
	}
	printf("\\n");
}

void UserRegister()
{
	printf("저희 시스템에 회원 등록하시려면 다음 정보를 입력하셔야 합니다.\\n\\n");

	printf("1. Input ID: ");
	gets(g_user_data[g_user_count].id);
	printf("2. Input Password: ");
	gets(g_user_data[g_user_count].password);
	printf("3. Input Name: ");
	gets(g_user_data[g_user_count].name);
	printf("4. Input Address: ");
	gets(g_user_data[g_user_count].address);
	printf("5. Input Registration Number: ");
	gets(g_user_data[g_user_count].registration_number);
	g_user_count++;

	printf("회원이 되신걸 축하드립니다.. \\n\\n");
}

main()
{
	int index = 0, input = 0;
	char id[16], password[16], success_login = 0;

	loaduserdata();

	while (!success_login) {
		printf("Input ID: ");
		gets(id);
		printf("Input Password: ");
		getpassword(password);

		for (index = 0; index < g_user_count; index++) {
			if (!strcmp(g_user_data[index].id, id)) {
				if (!strcmp(g_user_data[index].password, password)) {
					success_login = 1;
					printf("성공적으로 로그인 했습니다. ^^; \\n\\n");
				}
				else printf("암호가 틀렸습니다. ㅡㅡ! \\n\\n");
				break;
			}
		}

		if (index == g_user_count) {
			printf("님이 입력하신 ID는 등록이 되어있지 않습니다. 등록하시겠습니까? ( y / n ): ");
			input = getche();
			printf("\\n\\n");
			if (input == 'y' || input == 'Y') UserRegister();
			else printf("다시 입력하세요...\\n\\n");
		}
	}

	saveuserdata();
}