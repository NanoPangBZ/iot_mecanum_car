/********************************************************
 * 调试日志输出实现 - 格式化
 * 庞碧璋 2023/1/26
*******************************************************/

#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_

#include <stdarg.h>

#define ENABLE_ERROR_LOG    (1)
#define ENABLE_WARN_LOG     (1)
#define ENABLE_DEBUG_LOG    (1)
#define ENABLE_INFO_LOG     (1)

#if ENABLE_ERROR_LOG
#define ERROR_LOG(tag,...)  log_printf( "***[err]-[" tag "]:" __VA_ARGS__ )
#else
#define ERROR_LOG(tag,...)
#endif

#if ENABLE_WARN_LOG
#define WARN_LOG(tag,...)   log_printf( "---[warn]-[" tag "]:" __VA_ARGS__ )
#else
#define ERROR_LOG(tag,...)
#endif

#if ENABLE_DEBUG_LOG
#define DEBUG_LOG(tag,...)  log_printf( "[dbg]-[" tag "]:" __VA_ARGS__ )
#else
#define DEBUG_LOG(tag,...)
#endif

#if ENABLE_INFO_LOG
#define INFO_LOG(tag,...)   log_printf( "[info]-[" tag "]:" __VA_ARGS__ )
#else
#define INFO_LOG(tag,...)
#endif

#ifdef __cplusplus
extern "C"{
#endif

int log_printf(const char* fmt , ... );

#ifdef __cplusplus
}
#endif

#endif  //_DEBUG_LOG_H_

