/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 排列(permutation)
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "base.h"
#include "perm.h"

// #define NDEBUG
#ifdef  LIB_DEBUG
#undef  LIB_DEBUG
#define LIB_DEBUG   0
#else
#define LIB_DEBUG   0
#endif

struct {
    void            *vec;
    size_t          unit;
    int             n;
    perm_callback_t fn;
} __lsz_perm__;

//
// P(N,M) = N*..*(N-M+1) => P(N,N) = N!
//
void _lsz_perm(int next)
{
    if (next >= __lsz_perm__.n) {
        // 排列完成时，执行：callback()
        __lsz_perm__.fn(__lsz_perm__.vec, __lsz_perm__.n);
        return;
    } else {
        for (int i = next; i < __lsz_perm__.n; i++) {
            // vec[next] <=交换=> vec[i]
            lsz_swap(void_of(__lsz_perm__.vec, next, __lsz_perm__.unit), void_of(__lsz_perm__.vec, i, __lsz_perm__.unit), __lsz_perm__.unit);

            _lsz_perm(next + 1);

            // vec[next] <=复位=> vec[i]
            lsz_swap(void_of(__lsz_perm__.vec, next, __lsz_perm__.unit), void_of(__lsz_perm__.vec, i, __lsz_perm__.unit), __lsz_perm__.unit);
        }
    }
}
//
// API
//
void lsz_perm(void *vec, size_t unit, int n, perm_callback_t fn)
{
    __lsz_perm__.vec    = calloc(n, unit);
    __lsz_perm__.unit   = unit;
    __lsz_perm__.n      = n;
    __lsz_perm__.fn     = fn;

    memcpy(__lsz_perm__.vec, vec, n*unit);

    _lsz_perm(0);

    free(__lsz_perm__.vec);
}

#if LIB_DEBUG
void pout(int *vec, int n)
{
    for (int i = 0; i < n; i++) {
        printf("%04d", vec[i]);
        if (i != n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

int main(int argc, char const *argv[])
{
    int vec[] = { 0, 1, 2 };

    lsz_perm(vec, sizeof(int), sizeof(vec)/sizeof(int), pout);

    pout(vec, sizeof(vec)/sizeof(int));

    return 0;
}
#endif
