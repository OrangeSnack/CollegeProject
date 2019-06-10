/*
담당자 : 양희찬
개요 : 파일 입출력 전담 함수.
기능 : 
    요청에 따른 파일 열기 및 한 줄씩 읽어 반환하기.
    요청에 따른 파일 내용 수정 혹은 추가.
비고 : 
    사용하지 않을 것으로 예상됨.
*/
#ifndef acBook_fileIO
    #define acBook_fileIO

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "acBook_error.h"

    typedef struct _Table Table;

    // 파일 -> 테이블 변환
    // 파일명을 입력받고, 지정된 위치에서 찾아내서 테이블로 변환해 반환하는 함수.
    Table* read_file(char* name);

    // 테이블 -> 파일 변환
    // 해당 테이블의 이름과 같은 명칭의 파일을 테이블 내용으로 대체.
    void write_table(Table* target);


#endif

/*
파일 내용 양식:
    name : <테이블의 이름>
    date : <마지막 수정 시각>
    num_col : 열 갯수
    num_record : 행(레코드) 갯수
    column : <열1>, <열2>, ...
    data : 
    [
        <데이터1>, <데이터2>, ...
    ]
*/