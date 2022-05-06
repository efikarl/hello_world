/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    求解连续数列(K = |i|{Ki}|N|, S = sum {Ki}|0,N|, 0 < S < 100000, 0 < N < 100000)
---------------------------------------------------------------------------------
    [i] S N
    [o]
        有解：K
        无解：-1
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

typedef struct {
    int             s;
    int             n;
    int             i;
} solution_t;

solution_t          g_slt;

int in_parse(char *input)
{
    char *p, *end;

    p = end = input;
    g_slt.s = (int) strtol(p, &end, 10);
    p = end;
    g_slt.n = (int) strtol(p, &end, 10);
}

int solution(char *input)
{
    int found = 0;

    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    in_parse(input);

    g_slt.i = (g_slt.s/g_slt.n) - (g_slt.n>>1) + 1;
    if ((2*g_slt.i + g_slt.n-1)*g_slt.n == 2*g_slt.s) {
        found = 1;
    }

    if (found) {
        for (int i = g_slt.i; i < g_slt.i + g_slt.n; i++) {
            printf("%d", i);
            if (i != g_slt.i + g_slt.n - 1) {
                printf(" ");
            } else {
                printf("\n");
            }
        }
    } else {
        printf("-1\n");
    }
}

int main(int argc, char const *argv[])
{
    solution("525 6\n");

    return 0;
}
