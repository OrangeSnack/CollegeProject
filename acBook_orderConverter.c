#include "acBook_orderConverter.h"

Table_list* order_manager(char* order_string) {
    Order_queue* order = (Order_queue*) malloc(sizeof(Order_queue));
    order->order = NULL;
    order->next = NULL;
    Order_queue* order_head = &order;

    char temp[100] = {0, };     // 문자열의 일부를 잘라 전달하기 위한 임시 문자 배열
    int i = 0;
    int tag = 0;        // 문자열이 잘린 시점을 임시로 저장할 태그.

    for (i = 0; ; i++) {
        if (order_string[tag + i] == '(') 
            act_convert(order, temp, i, tag, 0);
        else if (order_string[tag + i - 1] == ')') 
            act_convert(order, temp, i, tag, 0);     
        else if (order_string[tag + i] == ' ') 
            act_convert(order, temp, i, tag, 1);
        else 
            temp[i] = order_string[tag + i];

        if ( order_string[tag + i] != '\0')
            break;
    }

    return table_manager(order_head);
}

// 반복되는 작업 내용을 추출한 중간다리 함수.
// 변환 함수 호출 및 기타 작업을 수행.
void act_convert(Order_queue* order, char* order_string, int* i, int* tag, int next_is_space) {
    order_string[*i + 1] = "\0";
    order = convert_str_to_order(order, order_string, *i + 1);
    *tag = i;
    if (next_is_space == 1) 
        *tag += 1;
    *i = -1;
}

Order_queue* convert_str_to_order(Order_queue* order, char* order_string, int string_length) {
    Order_queue* new_order = (Order_queue*) malloc(sizeof(Order_queue));
    order->next = &new_order;
    new_order->next = NULL;

    new_order->order = (char*) malloc(sizeof(char) * (string_length + 1));
    new_order->order = order_string;

    return &new_order;
}