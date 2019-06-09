#include "acBook_fileIO.h"

#define MAX_LINES 10000

/*
Table* read_file(char* name) {
    // 파일 객체 동적 생성
    FILE* fp;
    char* url;
    sprintf(url, "%s%s%s", "data\\", name, ".txt");
    fp = fopen(url, "r");       // 읽기 모드로 파일 오픈
    // 파일이 존재하는지 확인.
    if (fp == NULL) {
        show_error_message("read_file()", 404);
        return NULL;
    }

    // 파일이 적합한 양식으로 구성되었는지 확인
    char* temp;
    char* compare_text[6] = {
        "name : ",
        "date : ", 
        "num_col : " , 
        "num_record : ", 
        "column : ",
        "data : "
    };
    for (int i = 0; i < 6; i++) { 
        fgets(temp, MAX_LINES, fp);
        if (strncmp(temp, compare_text[i], strlen(compare_text[i]))) {
            show_error_message("read_file()", 400);
            fclose(fp);
            return NULL;
        }
    }
    fseek(fp, 0, SEEK_SET);

    // 테이블 객체 동적 생성
    Table* table = (Table*) malloc(sizeof(Table));

    // 테이블 데이터로 파일 값을 대입.

    // 파일 객체 삭제

    // 테이블 객체 반환
}
*/

void write_table(Table* tagert) {
    // 파일 객체 동적 생성 

    // 테이블의 이름과 동일한 파일을 wt 방식으로 오픈

    // 테이블의 메타 데이터를 입력

    // 테이블의 데이터를 입력

    // 파일 객체 삭제

}