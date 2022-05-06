/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 快速排序
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "base.h"
#include "sort.h"

// #define NDEBUG
#ifdef  LIB_DEBUG
#undef  LIB_DEBUG
#define LIB_DEBUG   0
#else
#define LIB_DEBUG   0
#endif

typedef int (* lsz_compare_t) (const void *a, const void *b);

struct {
    void *          data;
    size_t          unit;
    int             n;
    lsz_compare_t   fn;
} __lsz_qsort__;

void _lsz_qsort(int *data, int l, int r)
{
    int i = l, j = r -1;

    if (l >= r) {
        return;
    }
    // let pivot = data[r], p = i
    // partition: find where pivot should be(p)
    while (1)
    {
        while ((__lsz_qsort__.fn(void_of(data, i, __lsz_qsort__.unit), void_of(data, r, __lsz_qsort__.unit)) <  0))
            i++;
        while ((__lsz_qsort__.fn(void_of(data, j, __lsz_qsort__.unit), void_of(data, r, __lsz_qsort__.unit)) >= 0) && (j >= 0))
            j--;
        if (i < j) {
            lsz_swap(void_of(data, i, __lsz_qsort__.unit), void_of(data, j, __lsz_qsort__.unit), __lsz_qsort__.unit);
        } else {
            break;
        }
    }
    if (i != r) {
        lsz_swap(void_of(data, i, __lsz_qsort__.unit), void_of(data, r, __lsz_qsort__.unit), __lsz_qsort__.unit);
    }

    _lsz_qsort(data, l, i - 1);
    _lsz_qsort(data, i + 1, r);
}

void lsz_qsort(void *data, size_t unit, int n, lsz_compare_t fn)
{
    __lsz_qsort__.data  = data;
    __lsz_qsort__.unit  = unit;
    __lsz_qsort__.n     = n;
    __lsz_qsort__.fn    = fn;

    _lsz_qsort(data, 0, n - 1);
}

int lsz_icmp(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) - (*(int *)a < *(int *)b);
}

int lsz_scmp(const void *a, const void *b)
{
    return strcmp(a, b);
}

#if LIB_DEBUG

int main(int argc, char const *argv[])
{
    int n = 0;

    int v[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5 }; n = sizeof(v)/sizeof(int);
    lsz_qsort(v, sizeof(int), n, lsz_icmp);
    for (int i = 0; i < n; i++) {
        printf("%d", v[i]);
        if (i != n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }

    char s[][4] = { "cba", "abc", "acb", "bca", "bac", "cab" }; n = 3*2*1;
    lsz_qsort(s, 4, n, lsz_scmp);
    for (int i = 0; i < n; i++) {
        printf("%s", s[i]);
        if (i != n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }

    return 0;
}

#endif
