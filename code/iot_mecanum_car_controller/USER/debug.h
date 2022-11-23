/*
 * @Date: 2022-11-20 03:48:35
 * @LastEditors: 没人要的野指针
 * @LastEditTime: 2022-11-20 21:24:31
 * @Description: 调试日志输出
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#define YELLOW_FONT     "\\032[31m"
#define GREEN_FONT      "\\032[31m"
#define RED_FONT        "\\032[31m"
#define DEFAULT_FONT    "\\032[31m"

#define DEBUG_LOGI(info) printf( GREEN_FONT "[info] [%s] : %s \n" DEFAULT_FONT , __FILE__ , info )
#define DEBUG_LOGW(warn) printf( YELLOW_FONT "[warning] [%s] : %s \n" DEFAULT_FONT , __FILE__ , warn )
#define DEBUG_LOGE(err)  printf( RED_FONT "[error] [%s] : %s \n" DEFAULT_FONT , __FILE__ , err )

#define ERROR_CHECK(code,pass_code) \
    do{ \
        if(code != pass_code){\
            printf("\\033[31m"); \
            printf("%s - %s - line:%s\n", __FILE__ , __func__ , __LINE__)\
            printf("\\033[0m");   \
        }\
    }while(0);

#endif  //_DEBUG_H_

