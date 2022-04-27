/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    排列
---------------------------------------------------------------------------------
    [i] 待排数组
    [o] 完全排列
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "perm.h"

// #define NDEBUG
#ifndef APP_DEBUG
#define APP_DEBUG   0
#endif

typedef struct {
    int             *vec;
    int             n;
    perm_callback_t fn;
} perm_t;

perm_t              g_perm;

#if APP_DEBUG
int pout(int *p, int n)
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

void init(int *vec, int n, perm_callback_t fn)
{
    g_perm.vec  = vec;
    g_perm.n    = n;
    g_perm.fn   = fn;
}

void swap(int a, int b)
{
    int t; t = g_perm.vec[a]; g_perm.vec[a] = g_perm.vec[b]; g_perm.vec[b] = t;
}

//
// P(N,M) = N*..*(N-M+1) => P(N,N) = N!
//
void perm(int next)
{
    if (next >= g_perm.n) {
        // 当排列完成时，执行：callback()
        g_perm.fn(g_perm.vec, g_perm.n);
        return;
    } else {
        for (int i = next; i < g_perm.n; i++) {
            // next 依次取值 [next, n)
            swap(next, i);
            perm(next+ 1);
            // next 复位
            swap(next, i);
        }
    }
}

#if APP_DEBUG
int main(int argc, char const *argv[])
{
    int vec[] = { 0, 1, 2, 3, 4 }; int n = sizeof(vec)/sizeof(int);

    init(vec, n, pout);
    perm(0);

    pout(vec, n);

    return 0;
}
#endif
