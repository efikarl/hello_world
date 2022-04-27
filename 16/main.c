/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    篮球比赛（经典：组合问题）
---------------------------------------------------------------------------------
    [i] 以空格分隔的10个队员的战力
    [o] 最佳训练效果时的战斗力差值（10人分两队的最小战斗力差值）
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

// #define NDEBUG
#ifndef APP_DEBUG
#define APP_DEBUG   0
#endif

typedef struct {
    int             people[10];
    int             total;
    int             group[5];
    int             i;
    int             best;
} solution_t;

solution_t          g_slt;

int ip_parse(char *input)
{
    char *p, *end, i = 0;

    // code
    p = end = input;
    do {
        p = end;
        g_slt.people[i++] = (int) strtol(p, &end, 10);
    } while(p != end);

    for (i = 0; i < 10; i++) {
        g_slt.total += g_slt.people[i];
    }
    return 0;
}

int this_diff()
{
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += g_slt.group[i];
#if APP_DEBUG
        printf("%02d", g_slt.group[i]);
        if (i != 4) {
            printf(", ");
        }
#endif
    }

#if APP_DEBUG
    printf(" | abs(%-2d - %-2d) = %2d\n", sum, g_slt.total - sum, abs(g_slt.total - sum - sum));
#endif

    return abs(g_slt.total - sum - sum);
}

void group(int next)
{
    if (g_slt.i + 10 - next < 5) {
        return;
    }
    if (g_slt.i >= 5) {
        int diff = this_diff();
        if (diff < g_slt.best) {
            g_slt.best = diff;
        }
        return;
    } else {
        g_slt.group[g_slt.i] = g_slt.people[next];
        g_slt.i++;
        group(next + 1);
        g_slt.i--;
        group(next + 1);
    }
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }
    g_slt.best = __INT_MAX__;

    ip_parse(input);

    // code
    group(0);
    printf("%d\n", g_slt.best);

    return 0;
}

int main(int argc, char const *argv[])
{
    solution("10 9 8 7 6 5 4 3 2 1");

    return 0;
}
