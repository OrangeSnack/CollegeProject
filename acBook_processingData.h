/*
담당자 : 양희찬
개요 : DB와 연계, 테이블을 추출, 생성, 조작, 반환하는 함수군
특징 : 
    'acBook_operationTable' 파일의 'table_manager' 함수가 호출
기능 : 
    DB 관련 : 
        DB 데이터 추출
        DB 데이터 임시 변경
        DB 데이터 확정 변경
    테이블 관련 : 
        테이블 생성.
        테이블 조작.
        테이블 제거.
        테이블 반환.
*/
#ifndef acBook_processingData
    #define acBook_processingData

    #include "acBook_operationTable.h"
    

    // 모든 조건 테이블은 자동으로 삭제됨.
    // 통계 관련 함수는 새로운 테이블을 생성해 반환함.
    // 정렬, 필터링 등의 함수는 타겟 테이블을 수정해 반환함.

    // 자료 비교 함수.
    // target : 비교 작업을 진행할 대상 테이블
    // condition : 비교 작업을 수행할 조건 테이블    
    Table* operator_in (Table* target, Table* condition, Table_list** total_list);

#endif