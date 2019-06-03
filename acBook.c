#include "acBook.h"

int main() {
    system("chcp 65001");
    system("cls");
    _wsetlocale(LC_ALL, L"korean");     // 정석 유니코드 사용법.

    wchar_t testString[100] = L"유니코드 테스트 코드";      
    printf("%S\n", testString);

    system("pause");
    return 0;
}