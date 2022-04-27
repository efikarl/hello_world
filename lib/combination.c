/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    组合
---------------------------------------------------------------------------------
    [i] 待组数组
    [o] 部分组合
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "comb.h"

// #define NDEBUG
#ifndef APP_DEBUG
#define APP_DEBUG   0
#endif

typedef struct {
    int             *set;
    int             n;
    int             *sub;
    int             m;
    int             i;
    comb_callback_t fn;
} comb_t;

comb_t              g_comb;

#if APP_DEBUG
int cout(int *p, int n)
{
    for (int i = 0; i < n; i++) {
        printf("%04d", p[i]);
        if (i != n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}
#endif

void init(int *set, int n, int *sub, int m, comb_callback_t fn)
{
    g_comb.set  = set;
    g_comb.n    = n;
    g_comb.sub  = sub;
    g_comb.m    = m;
    g_comb.i    = 0;
    g_comb.fn   = fn;
}
//
// C(N,M) = C(N,N-M) = (n*..*(m+1))/(n-m)!
// C(N,M) = C{i|i∈[N->M+1]} + C(N-1,M-1)
//
void comb(int next)
{
    //  当前子集元素 + 集合剩余元素 < 子集元素总数 => 不可构造
    if (g_comb.i + (g_comb.n - next) < g_comb.m) {
        return;
    }
    if (g_comb.i >= g_comb.m) {
        // 当排列完成时，执行：callback()
        g_comb.fn(g_comb.sub, g_comb.i);
    } else {
        g_comb.sub[g_comb.i] = g_comb.set[next];
        g_comb.i++; // 选择当前位置
        comb(next + 1);
        g_comb.i--; // 忽略当前位置
        comb(next + 1);
    }
}

#if APP_DEBUG
int main(int argc, char const *argv[])
{
    int set[ ] = { 0, 1, 2, 3, 4 };
    int sub[3];
    int n = sizeof(set)/sizeof(int);
    int m = sizeof(sub)/sizeof(int);

    init(set, n, sub, m, cout);
    comb(0);

    return 0;
}
#endif
