/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    区间合并
---------------------------------------------------------------------------------
    [i] 区间列表
    [o] 升序排列的公共区间的合并区间列表
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

#define MAX_M_RANGE 1000
#define MAX_X_RANGE (((MAX_M_RANGE-1)*MAX_M_RANGE)>>1)

typedef int range_t[2];

typedef struct {
    int     l_limit[MAX_X_RANGE];
    int     r_limit[MAX_X_RANGE];
    int     x_index;
    range_t m_range[MAX_M_RANGE];
    int     m_index;
} solution_t;

solution_t  g_slt;

bool is_ro(range_t r, range_t t)
{
    if (r[1] < t[0] || r[0] > t[1]) {
        return true;
    } else {
        return false;
    }
}

int max(int a, int b)
{
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int min(int a, int b)
{
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

int icmp(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

void r_qsort(int *nums, int l, int r)
{
    qsort(nums, r - l + 1, sizeof(int), icmp);
}

//
// [1] 交集生成
//
void r_cross(range_t *range, int l, int r)
{
    if (l >= r) {
        return;
    }
    for (int i = l + 1; i <= r; i++) {
        if (!is_ro(range[l], range[i])) {
            g_slt.l_limit[g_slt.x_index] = max(range[l][0], range[i][0]);
            g_slt.r_limit[g_slt.x_index] = min(range[l][1], range[i][1]);
#if APP_DEBUG
            printf("range: [%d:%d] x [%d:%d] = [%d:%d]\n",
                range[l][0], range[l][1], range[i][0], range[i][1],
                g_slt.l_limit[g_slt.x_index], g_slt.r_limit[g_slt.x_index]);
#endif
            g_slt.x_index++;
        }
    }

    r_cross(range, l + 1, r);
}

//
// [2] 交集合并
//
void r_merge()
{
    for (int i = 0; i < g_slt.x_index; i++) {
        g_slt.m_range[g_slt.m_index][0] = g_slt.l_limit[i];
        while (g_slt.r_limit[i] >= g_slt.l_limit[i+1] && (i+1 < g_slt.x_index)) i++;
        g_slt.m_range[g_slt.m_index][1] = g_slt.r_limit[i];
        printf("%d %d\n", g_slt.m_range[g_slt.m_index][0], g_slt.m_range[g_slt.m_index][1]);

        g_slt.m_index++;
    }
}

int solution(range_t *range, int n)
{
    if (!range) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    r_cross(range, 0, n - 1);
    r_qsort(g_slt.l_limit, 0, g_slt.x_index - 1);
    r_qsort(g_slt.r_limit, 0, g_slt.x_index - 1);
    r_merge();

    return 0;
}

range_t test1[] = { {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5} };
range_t test2[] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5} };
range_t test3[] = { {0, 3}, {1, 4}, {2, 5}, {3, 6}, {4, 7} };
range_t test4[] = { {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9} };

int main(int argc, char const *argv[])
{
    printf("--------\n");
    solution(test1, sizeof(test1)/sizeof(range_t));
    printf("--------\n");
    solution(test2, sizeof(test2)/sizeof(range_t));
    printf("--------\n");
    solution(test3, sizeof(test3)/sizeof(range_t));
    printf("--------\n");
    solution(test4, sizeof(test4)/sizeof(range_t));
    printf("--------\n");
    return 0;
}
