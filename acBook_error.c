#include "acBook_error.h"

void show_error_message(const char* target, int error_code) {
    char* error_message;

    switch(error_code) {
        case 100:
            error_message = (char*)"계속 : 다음 작업을 진행해 주십시오.";
            break;
        case 200:
            error_message = (char*)"성공 : 해당 작업이 완료되었습니다.";
            break;
        case 400:
            error_message = (char*)"잘못된 요청 : 전달된 명령의 양식이 잘못되었습니다.";
            break;
        case 404:
            error_message = (char*)"찾을 수 없음 : 요청하신 자료를 찾을 수 없습니다.";
            break;
        case 407: 
            error_message = (char*)"충돌 : 이미 사용중인 이름입니다.";
            break;
        case 501:
            error_message = (char*)"구현 안됨 : 해당 기능은 아직 구현되지 않았습니다.";
            break;
        case 507:
            error_message = (char*)"용량 부족 : 해당 기능을 수행하기 위한 여유 메모리가 부족합니다.";
            break;
        default: 
            error_message = (char*)"식별되지 않은 오류 : 알 수 없는 오류가 발생했습니다.";
    }

    printf("위치 : %s\n", target);
    printf("%s\n", error_message);
}