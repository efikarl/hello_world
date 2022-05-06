/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 组合(combination)
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "comb.h"

// #define NDEBUG
#ifdef  LIB_DEBUG
#undef  LIB_DEBUG
#define LIB_DEBUG   0
#else
#define LIB_DEBUG   0
#endif

struct {
    void            *set;
    void            *sub;
    size_t          unit;
    int             n;
    int             m;
    comb_callback_t fn;
    int             i;
} __lsz_comb__;
//
// C(N,M)
//
void _lsz_comb(int next)
{
    //  当前子集元素 + 集合剩余元素 < 子集元素总数 => 不可构造
    if (__lsz_comb__.i + (__lsz_comb__.n - next) < __lsz_comb__.m) {
        return;
    }
    if (__lsz_comb__.i >= __lsz_comb__.m) {
        // 排列完成时，执行：callback()
        __lsz_comb__.fn(__lsz_comb__.sub, __lsz_comb__.i);
    } else {
        // sub[i] = set[next]
        memcpy((char *)__lsz_comb__.sub + __lsz_comb__.unit*__lsz_comb__.i, (char *)__lsz_comb__.set + __lsz_comb__.unit*next, __lsz_comb__.unit);
        __lsz_comb__.i++; //  当前位置
        _lsz_comb(next + 1);
        __lsz_comb__.i--; // !当前位置
        _lsz_comb(next + 1);
    }
}
//
// API
//
void lsz_comb(void *set, size_t unit, int n, int m, comb_callback_t fn)
{
    __lsz_comb__.set    = calloc(n, unit);
    __lsz_comb__.sub    = calloc(m, unit);
    __lsz_comb__.unit   = unit;
    __lsz_comb__.n      = n;
    __lsz_comb__.m      = m;
    __lsz_comb__.fn     = fn;
    __lsz_comb__.i      = 0;

    memcpy(__lsz_comb__.set, set, n*unit);

    _lsz_comb(0);

    free(__lsz_comb__.set);
    free(__lsz_comb__.sub);
}

#if LIB_DEBUG
void cout(int *sub, int m)
{
    for (int i = 0; i < m; i++) {
        printf("%04d", sub[i]);
        if (i != m - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

int main(int argc, char const *argv[])
{
    int set[] = { 0, 1, 2, 3, 4 };

    lsz_comb(set, sizeof(int), sizeof(set)/sizeof(int), 3, cout);

    return 0;
}
#endif
