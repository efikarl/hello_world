/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    任务最优调度
---------------------------------------------------------------------------------
    [i] 
        line 1: 逗号分隔的任务类型
        line 2: 同类任务的冷却时间
    [o] 
        所有任务完成最短时间
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// #define NDEBUG
#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

#define MAX_TASK    1000

typedef struct {
    int             task[MAX_TASK];
    int             i;
    int             cd;
    int             time;
} solution_t;

solution_t          g_slt;

int in_parse(char *input)
{
    char *p, *end;

    end = input;
    while (*end != '\n') {
        while (*end == ',') end++;
        if (g_slt.i < MAX_TASK) {
            g_slt.task[g_slt.i++] = *end - '0';
        }
        end++;
    }
    end++;
    assert(*end != '\n');

    p = end;
    g_slt.cd = (int) strtol(p, &end, 10);
    assert(g_slt.cd > 0);

    return 0;
}

int next_one(int t)
{
    int i, n, found = 0;

    if (t >= g_slt.i) {
        return 0;
    }

    n = t + 1;
    for (i = n; i < g_slt.i; i++) {
        if (g_slt.task[i] != g_slt.task[t]) {
            found = 1;
            // 交换任务位置
            int t; t = g_slt.task[n]; g_slt.task[n] = g_slt.task[i]; g_slt.task[i] = t;
            break;
        }
    }

    g_slt.time++;
    if (!found) {
        if (n < g_slt.i) {
            g_slt.time += g_slt.cd;
        }
    }
#if APP_DEBUG
    printf("g_slt.task:[%d][%d] = %d.%d\n", t, n, g_slt.task[t], g_slt.task[n]);
#endif
    return 1;
}

int dispatch()
{
    for (int i = 0; next_one(i); i++);
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }
    in_parse(input);

    dispatch();

    printf("%d\n", g_slt.time);
}

int main(int argc, char const *argv[])
{
    solution(
        "2,2,2,3\n"
        "2\n"
        );

    return 0;
}
