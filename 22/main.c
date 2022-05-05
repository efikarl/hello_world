/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    数字涂色（公共因子存在的共色）
---------------------------------------------------------------------------------
    [i]
        line 1: N
        line 2: N个空格分隔的整数
    [o]
        颜色个数
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
    int             val;
    int             vld;
} num_t;

typedef struct {
    int             n;
    num_t           *nums;
} solution_t;

solution_t          g_slt;

int ip_parse(char *input)
{
    char *p, *end;

    // code
    p = end = input;
    g_slt.n = strtol(p, &end, 10);
    end++;

    g_slt.nums = malloc(sizeof(num_t)*g_slt.n);

    int i = 0;
    do {
        if (i >= g_slt.n) {
            break;
        }
        p = end;
        g_slt.nums[i].val = strtol(p, &end, 10);
        g_slt.nums[i].vld = 1;
        i++; end++;
    } while(end[0] && end[0] != '\n');

    if (i < g_slt.n) {
        g_slt.n = i;
    }

    return 0;
}

int icmp(const void *a, const void *b)
{
    int x = ((num_t *) a)->val;
    int y = ((num_t *) b)->val;

    return (x > y) - (x < y);
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
    qsort(g_slt.nums, g_slt.n, sizeof(num_t), icmp);
    for (int i = 0; i < g_slt.n - 1; i++) {
        for (int j = i + 1; j < g_slt.n; j++) {
            if (!g_slt.nums[j].vld) {
                continue;
            }
            if (g_slt.nums[j].val % g_slt.nums[i].val == 0) {
                g_slt.nums[j].vld = 0;
            }
        }
    }

    int count = 0;
    for (int i = 0; i < g_slt.n; i++) {
        if (g_slt.nums[i].vld) {
            count++;
        }
    }
    printf("%d\n", count);

    if (g_slt.nums) {
        free(g_slt.nums);
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    solution(
        "10\n"
        "11 2 3 4 5 6 7 8 9 10\n"
    );

    return 0;
}
