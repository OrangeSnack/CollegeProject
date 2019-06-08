#include "acBook_memberUtil.h"

//ID DB에 출력해주는 함수
char* idOut(char *id)
{
  return id;
}

//PW DB에 출력해주는 함수
char* pwOut(char *pw)
{
  return pw;
}


Table_list* login(char* id, char* pw) {
    char temp[30] = {0, };
    
    sprintf(temp, "%s%s%s%s%s%s", "login ", "(", id, ", ", pw, ")");
    
    return order_manager(temp);
}
