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
    int     n;      // 字串长度
    int     r;      // 子串合规?
    int     depth;  // 递归深度: sub_s_split
    int     rc;     // 结果计数
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

int sub_s_sum(char *s, int n)
{
    int sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += s[i];
    }

    return sum;
}

int sub_s_split(char *s, int n, bool init)
{
    if (init) {
        memset(&g_slt, 0, sizeof(g_slt));
    }
    g_slt.depth++;
    if (n > 0) {
        g_slt.r = is_in_rule(sub_s_sum(s, n));
        if (g_slt.r) {
            g_slt.sc++;
            g_slt.rc++;
        }
#if APP_DEBUG
        printf("****.****(%c) : %.*s\n", g_slt.r?'+':'-', n, s);
#endif
    }

    for (int i = 1; i < n; i++) {
        g_slt.r = is_in_rule(sub_s_sum(s, i));
#if APP_DEBUG
        printf("%04d.%04d(%c) : %.*s\n", n, i, g_slt.r?'+':'-', i, s);
#endif
        if (g_slt.r) {
            g_slt.sc = g_slt.depth;
            sub_s_split(&s[i], n-i, false);
        }
    }

    g_slt.depth--;
    switch (g_slt.rc) {
    case 0:
        return g_slt.rc;
    case 1:
        return g_slt.sc;
    default:
        return -1;
    }
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    while (input[g_slt.n] != '\0')  g_slt.n++;

    return sub_s_split(input, g_slt.n, true);
}

int main(int argc, char const *argv[])
{
    printf("s = ABCDE$018W#EFI$: %d\n", solution("ABCDE$018ABCDE$"));
}
