/* 
개요 : 각종 에러 관리 함수 모음
*/

#ifndef acBook_error
    #define acBook_error

    #include <stdio.h>

    /*
    개요 : 에러 코드 안내 메세지 반환함수. (HTML 에러 코드 참고)
    에러코드 : 
    - 100 : 계속(다음 작업을 요구)
    - 200 : 성공(해당 작업이 완료됨)
    - 400 : 잘못된 요청(명령전달 혹은 인수가 잘못됨)
    - 404 : 찾을 수 없음(찾는 자료가 존재하지 않음)
    - 409 : 충돌(기존 데이터와 충돌이 발생함)
    - 501 : 구현되지 않음(해당 기능이 아직 구현되지 않음)
    - 507 : 용량 부족(생성함수 등을 수행할 때 필요한 여유 용량이 부족함)
    */
    void show_error_message(char* target, int error_code);

#endif