/*
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
	printf("\n");
}

void UserRegister()
{
	printf("���� �ý��ۿ� ȸ�� ����Ͻ÷��� ���� ������ �Է��ϼž� �մϴ�.\n\n");

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

	printf("ȸ���� �ǽŰ� ���ϵ帳�ϴ�.. \n\n");
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
					printf("���������� �α��� �߽��ϴ�. ^^; \n\n");
				}
				else printf("��ȣ�� Ʋ�Ƚ��ϴ�. �Ѥ�! \n\n");
				break;
			}
		}

		if (index == g_user_count) {
			printf("���� �Է��Ͻ� ID�� ����� �Ǿ����� �ʽ��ϴ�. ����Ͻðڽ��ϱ�? ( y / n ): ");
			input = getche();
			printf("\n\n");
			if (input == 'y' || input == 'Y') UserRegister();
			else printf("�ٽ� �Է��ϼ���...\n\n");
		}
	}

	saveuserdata();
}*/