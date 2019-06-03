/*
담당자 : 양희찬
개요 : 각종 테이블 연산을 별도로 분리한 파일
기능 : 
    열 관련 : 
        열 추가
        열 삭제
        열 병합
    행 관련 : 
        데이터 추가
        데이터 병합
    통계 관련 : 
        요소별 합계.
        요소별 평균.
        요소별 갯수.
*/
#ifndef acBook_operationTable
    #define acBook_operationTable

    typedef struct _BplusTree {
        char* name_tree;    // 트리의 이름을 가리킬 포인터
        Node* root;         // 뿌리 노드를 가리키는 포인터
        Node* first_leaf;   // 시작 잎 노드를 가리키는 포인터
        Node* pointer;      // 검색 등에서 사용되는 변동 포인터.
        Record* name_col;   // 해당 테이블의 열 이름들을 모아둔 레코드 포인터.
        int col;            // 검색 등에서 기준이 되는 column 번호.
        int direction;      // 정렬 방향. 0 : 오름차순, 1 : 내림차순
    } BplusTree;

    // 생성함수 : BplusTree
    void init_BplusTree (BplusTree* target);        
    // 소멸함수 : BplusTree
    void destroy_BplusTree (BplusTree* target);     
    // 변환 : 문자열로 입력된 열 이름 -> 열 번호
    int convert_col_to_num(BplusTree* target, char* col_name);
    // 레코드 검색 : 일치하는 값이 있으면 해당 값을, 없으면 NULL을 반환.
    char* search (BplusTree* target, int col, char* string);    
    // 레코드 삽입 : 
    int insert(BplusTree* target, int col, Record data);        
    // 변환 : 'B+tree' -> 2차원 문자열 배열.
    char*** convert_to_arr (BplusTree* target);     


    // 'record'를 정돈하고 모아 관리하는 'node'구조체. 'B+tree'의 일부.
    typedef struct _Node {
        char** data;        // 문자열 배열을 동적 할당 받을 2차원 char형 포인터
        Node** child;       // 자식 노드들을 가리킬 포인터 배열을 받을 2차원 구조체 포인터, leaf 노드는 다음 노드만를 가리킴.
        int position;       // 위치가 어디인지 확인. index : 0, leaf : 1;
        int num_data;       // 해당 노드에 몇개의 데이터가 저장되어 있는지 기록.
    } Node;

    void init_Node (Node* target);                  // 생성함수 : Node
    void destroy_Node (Node* target);               // 소멸함수 : Node


    // 테이블의 각 자료를 구성하는 단방향 연결 리스트
    typedef struct _Record {


    } Record;

#endif

/* 
세부 설계
struct table : 
    string col[];
    string key_record[];

*/