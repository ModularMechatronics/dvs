#ifndef MC_LOGGING_H_
#define MC_LOGGING_H_

#include "preprocessor_settings.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_COL_RESET "\033[0m"
#define CMD_COL_GREEN "\033[32m"
#define CMD_COL_CYAN "\033[36m"
#define CMD_COL_YELLOW "\033[33m"
#define CMD_COL_BLUE "\033[34m"
#define CMD_COL_RED "\033[31m"

#define kInvalidString "<INVALID-STRING>"

#if defined(__cplusplus)
extern "C" {
#endif

inline const char* const getFileName(const char* const fn)
{
#ifdef LOG_SHOW_FILE_FULL_PATH
    return fn;
#else
    if((!fn) || (!(fn[0])))
    {
        return kInvalidString;
    }
    else
    {
        const int num_chars = strlen(fn);
        if(num_chars == 0)
        {
            return kInvalidString;
        }
        int idx = num_chars - 1;
        while(fn[idx] != '/')
        {
            idx--;
        }
        return &(fn[idx + 1]);
    }
#endif
}

#define MC_ASSERT(_0__cond, ...)                                     \
{                                                                    \
    if(!_0__cond)                                                    \
    {                                                                \
        printf("%sAssertion error in file '%s' on line '%d':%s ",    \
            CMD_COL_RED,                                             \
            getFileName(__FILE__),                                   \
            __LINE__,                                                \
            CMD_COL_RESET);                                          \
        char _q__formatted_string[256];                              \
        sprintf(_q__formatted_string, __VA_ARGS__);                  \
        printf("%s\n", _q__formatted_string);                        \
        exit(-1);                                                    \
    }                                                                \
}

#define MC_EXIT(...)                                 \
{                                                    \
    printf("%sExit from file '%s' on line '%d':%s ", \
        CMD_COL_RED,                                 \
        getFileName(__FILE__),                       \
        __LINE__,                                    \
        CMD_COL_RESET);                              \
    char _q__formatted_string[256];                  \
    sprintf(_q__formatted_string, __VA_ARGS__);      \
    printf("%s\n", _q__formatted_string);            \
    exit(-1);                                        \
}

#define MC_LOG(...)                                \
{                                                  \
    char _q__formatted_string[256];                \
    sprintf(_q__formatted_string, __VA_ARGS__);    \
    printf("%s[LOG][%s][%d]:%s %s\n",              \
        CMD_COL_GREEN,                             \
        getFileName(__FILE__),                     \
        __LINE__,                                  \
        CMD_COL_RESET,                             \
        _q__formatted_string);                     \
}

#define MC_INFO(...)                               \
{                                                  \
    char _q__formatted_string[256];                \
    sprintf(_q__formatted_string, __VA_ARGS__);    \
    printf("%s[INF][%s][%d]:%s %s\n",              \
        CMD_COL_GREEN,                             \
        getFileName(__FILE__),                     \
        __LINE__,                                  \
        CMD_COL_RESET,                             \
        _q__formatted_string);                     \
}

#define MC_ERROR(...)                              \
{                                                  \
    char _q__formatted_string[256];                \
    sprintf(_q__formatted_string, __VA_ARGS__);    \
    printf("%s[ERR][%s][%d]:%s %s\n",              \
        CMD_COL_RED,                               \
        getFileName(__FILE__),                     \
        __LINE__,                                  \
        CMD_COL_RESET,                             \
        _q__formatted_string);                     \
}

#define MC_FATAL(...)                              \
{                                                  \
    char _q__formatted_string[256];                \
    sprintf(_q__formatted_string, __VA_ARGS__);    \
    printf("%s[FAT][%s][%d]:%s %s\n",              \
        CMD_COL_RED,                               \
        getFileName(__FILE__),                     \
        __LINE__,                                  \
        CMD_COL_RESET,                             \
        _q__formatted_string);                     \
}

#define MC_DEBUG(...)                              \
{                                                  \
    char _q__formatted_string[256];                \
    sprintf(_q__formatted_string, __VA_ARGS__);    \
    printf("%s[DBG][%s][%d]:%s %s\n",              \
        CMD_COL_CYAN,                              \
        getFileName(__FILE__),                     \
        __LINE__,                                  \
        CMD_COL_RESET,                             \
        _q__formatted_string);                     \
}

#define MC_WARNING(...)                            \
{                                                  \
    char _q__formatted_string[256];                \
    sprintf(_q__formatted_string, __VA_ARGS__);    \
    printf("%s[WRN][%s][%d]:%s %s\n",              \
        CMD_COL_YELLOW,                            \
        getFileName(__FILE__),                     \
        __LINE__,                                  \
        CMD_COL_RESET,                             \
        _q__formatted_string);                     \
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif
