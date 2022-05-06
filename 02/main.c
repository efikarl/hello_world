/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    字串分割
---------------------------------------------------------------------------------
    [i] 待分割字串
    [o] 情形:
        [1]   失败  : 0
        [2] 1 成功  : 子串数目
        [3] n 失败  : -1
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

typedef struct {
    char    *s;
    int     n;      // 字串长度
    int     sc;     // 子串计数
} solution_t;

solution_t  g_slt;

int is_in_rule(int v)
{
    int i = v % 10, j = v / 10 % 10, k = v / 100;

    if (v < 100 || v > 999) {
        return 0;
    }

    if (i*i*i + j*j*j + k*k*k == v) {
        return 1;
    } else {
        return 0;
    }
}

int sub_s_sumck(char *s, int n)
{
    int sum = 0;

    for (int i = 0; i < n; i++) {
        sum += s[i];
        if (is_in_rule(sum)) {
#if APP_DEBUG
            printf("%.*s\n", i+1, s);
#endif
            g_slt.sc++;
        }
    }

    return sum;
}

void sub_s_split(int next)
{
    if (next >= g_slt.n) {
        return;
    } else {
        sub_s_sumck(&g_slt.s[next], g_slt.n - next);
        sub_s_split(next + 1);
    }
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    g_slt.n = strlen(input);
    g_slt.s = input;

    // code
    sub_s_split(0);

    switch (g_slt.sc) {
    case 0:
    case 1:
        return g_slt.sc;
    default:
        return -1;
    }
}

int main(int argc, char const *argv[])
{
    printf("%d\n", solution("ABCDE$018ABCDE$"));
}
