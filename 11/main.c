/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    找车位：目标=max(停车距离) (经典)
---------------------------------------------------------------------------------
    [i] 逗号分隔的停车标识：1=车 0=空
    [o] max(停车距离)
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

#define MAX_PARKING 100

typedef struct {
    int             p[MAX_PARKING+1];
    int             i;
    int             max_chain_zero;
    bool            at_edge;
} solution_t;

solution_t          g_slt;


int solution(char *input)
{
    int   max = 0;
    char *end = input;

    if (!input) {
        return -1;
    }

    // 参数分析
    for(g_slt.i = 0; end[0] != '\0' && g_slt.i < MAX_PARKING; g_slt.i++) {
        while (end[0] != '0' && end[0] != '1') end++;

        g_slt.p[g_slt.i] = end[0] == '0' ? 0 : 1;
        end++;
    }
    // 尾部添加单一非法元素, 方便链式遍历
    g_slt.p[g_slt.i] = 1;

    // 解决方案
    int i, from, count;
    for(i = 0; i < g_slt.i; i++) {
        if (g_slt.p[i] != 0) {
            continue;
        }
        from = i;
        while ((i < g_slt.i) && (g_slt.p[i + 1] == g_slt.p[i])) i++;
        // stop at last 0
        count = i - from + 1;
        if (count > g_slt.max_chain_zero) {
            g_slt.max_chain_zero = count;
            if (from == 0) {
                g_slt.at_edge = 1;
            } else {
                g_slt.at_edge = 0;
            }
        }
        if ((i == g_slt.i - 1) && (g_slt.max_chain_zero == count)) {
            g_slt.at_edge = 1;
        }
    }
    if (g_slt.at_edge) {
        max = g_slt.max_chain_zero;
    } else {
        max = g_slt.max_chain_zero%2 ? (g_slt.max_chain_zero + 1)/2 : g_slt.max_chain_zero/2;
    }
     
    printf("%d\n", max);
}

int main(int argc, char const *argv[])
{
    solution("0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1");

    return 0;
}
