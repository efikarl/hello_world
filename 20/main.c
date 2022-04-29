/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    字符统计及重排
---------------------------------------------------------------------------------
    [i] 字母字串
    [o] {字母:次数;?}+（从大到小）
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

typedef struct {
    char            *str;
    int              len;
} solution_t;

solution_t          g_slt;

int ccnt(char a)
{
    char *end = g_slt.str;
    int   cnt = 0;

    while (end[0]) {
        if (end[0] == a) {
            cnt++;
        }
        end++;
    }
    return cnt;
}

int icmp(const void *a, const void *b)
{
    char l = *(char *)a;
    char r = *(char *)b;
    // 计数逻辑不必每次重计，可以优化
    int lc = ccnt(l);
    int rc = ccnt(r);

    if (lc == rc) {
        if ((islower(l) && islower(r)) || (isupper(l) && isupper(r))) {
            return (l > r) - (l < r);
        } else {
            return (l < r) - (l > r);
        }
    } else {
        return (lc < rc) - (lc > rc);
    }
}

int ip_parse(char *input)
{
    char *p, *end;

    // code
    g_slt.len = strlen(input);
    g_slt.str = strdup(input);

    // ccnt
    qsort(g_slt.str, g_slt.len, sizeof(char), icmp);

    return 0;
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    ip_parse(input);

    // code
    char *p, *end;
    end = g_slt.str;
    while (end[0]) {
        p = end;
        while(end[0] == end[1]) end++;
        end++;
        printf("%c:%d", p[0], (int) (end - p));
        if (p[0] != g_slt.str[g_slt.len-1]) {
            printf(";");
        } else {
            printf("\n");
        }

    }

    return 0;
}

int main(int argc, char const *argv[])
{
    solution("xyxyXX");

    return 0;
}
