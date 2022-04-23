/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    非严格递增连续数字序列
---------------------------------------------------------------------------------
    [i] 字串（字母+数字 & 长度<255）
    [o] 最长的非严格递增连续数字序列的长度
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// #define NDEBUG
#ifndef APP_DEBUG
#define APP_DEBUG   1
#endif

#define MAX_CHAR    255

typedef struct {
    int             max;
} solution_t;

solution_t          g_slt;

int solution(char *input)
{
    char *end = &input[1];
    char *from;

    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    while ((end[0] != '\0') && (end - input < MAX_CHAR)) {
        from = &end[-1];
        while(((end[0] == end[-1]) || (end[0] == end[-1]+1)) && (end - input < MAX_CHAR)) end++;
        // stop at 1st i not continous
        if (g_slt.max < end - from) {
            g_slt.max = end - from;
        }
        end++;
    }

    printf("%d\n", g_slt.max);
}

int main(int argc, char const *argv[])
{
    solution("abc2234019A334bc");
    return 0;
}
