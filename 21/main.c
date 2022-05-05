/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    转骰子[初始状态（左右前后上下）：123456]
---------------------------------------------------------------------------------
    [i] 操作：L|R|F|B|A|C|
    [o] 最终状态
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
    int             l;
    int             f;
    int             u;
} dice_t;

typedef struct {
    dice_t          d;
} solution_t;

solution_t          g_slt;

int dice_op(int v) {
    switch (v)
    {
    case 1:
        return 2;
    case 2:
        return 1;
    case 3:
        return 4;
    case 4:
        return 3;
    case 5:
        return 6;
    case 6:
        return 5;
    default:
        return 0;
    }
}

void dice_ro(int *a, int *b)
{
    int t = dice_op(*a); *a = *b; *b = t;
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    // code
    g_slt.d.l = 1;// d
    g_slt.d.f = 3;// f
    g_slt.d.u = 5;// l

    char *end = input;

    while (end[0] != '\0')
    {
        switch (end[0]) {
        case 'L':
            dice_ro(&g_slt.d.l, &g_slt.d.u);
            break;
        case 'R':
            dice_ro(&g_slt.d.u, &g_slt.d.l);
            break;
        case 'F':
            dice_ro(&g_slt.d.f, &g_slt.d.u);
            break;
        case 'B':
            dice_ro(&g_slt.d.u, &g_slt.d.f);
            break;
        case 'A':
            dice_ro(&g_slt.d.f, &g_slt.d.l);
            break;
        case 'C':
            dice_ro(&g_slt.d.l, &g_slt.d.f);
            break;
        default:
            break;
        }
        end++;
    }

    printf("%d%d%d%d%d%d\n"
        , g_slt.d.l, dice_op(g_slt.d.l)
        , g_slt.d.f, dice_op(g_slt.d.f)
        , g_slt.d.u, dice_op(g_slt.d.u)
        );

    return 0;
}

int main(int argc, char const *argv[])
{
    solution("L");
    solution("R");
    solution("F");
    solution("B");
    solution("A");
    solution("C");

    return 0;
}
