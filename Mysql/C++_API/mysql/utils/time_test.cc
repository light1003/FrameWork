#include "time.h"
#include <cstring>
#include <stdio.h>

// 测试代码

int main(){

    int ret;

    char DateTime[_DATETIME_SIZE];

    

    memset(DateTime, 0, sizeof(DateTime));

    

    // 获取系统当前日期 

    ret = GetDate(DateTime);

    if(ret == 0) 

        printf("The Local date is %s\n", DateTime);

    else 

        perror("GetDate error!");

    

    memset(DateTime, 0, sizeof(DateTime));

    // 获取当前系统时间 

    ret = GetTime(DateTime);

    if(ret == 0) 

        printf("The Local time is %s\n", DateTime);

    else 

        perror("GetTime error!");

    

    memset(DateTime, 0, sizeof(DateTime));

    // 获取系统当前日期时间 

    ret = GetDateTime(DateTime);

    if(ret == 0) 

        printf("The Local date and time is %s\n", DateTime);

    else 

        perror("GetDateTime error!");

    

    return 0;

}
