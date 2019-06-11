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

    selection = mainMenu();
    mainSelection(selection);

    system("pause");
    return 0;
}
