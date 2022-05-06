/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    矩形相交的面积
---------------------------------------------------------------------------------
    [i] 左上x 左上y 宽度 高度
        line 1
        line 2
        line 3
    [o] 相交的面积（不相交则0）
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
    int             x;
    int             y;
} point_t;

typedef struct {
    point_t         l;  // 左上
    point_t         r;  // 右下
} rectangle_t;


typedef struct {
    rectangle_t     rect[3];
    int             i;
} solution_t;

solution_t          g_slt;

int in_parse(char *input)
{
    char *p, *end;

    // code
    end = input;
    do {
        if (g_slt.i >= 3) {
            break;
        }
        p = end;
        g_slt.rect[g_slt.i].l.x = strtol(p, &end, 10);
        p = end;
        g_slt.rect[g_slt.i].l.y = strtol(p, &end, 10);
        p = end;
        g_slt.rect[g_slt.i].r.x = g_slt.rect[g_slt.i].l.x + strtol(p, &end, 10);
        p = end;
        g_slt.rect[g_slt.i].r.y = g_slt.rect[g_slt.i].l.y - strtol(p, &end, 10);
        g_slt.i++; // '\n'
    } while (end[0] != '\0');

    return 0;
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    in_parse(input);

    // code
    rectangle_t cross;
    cross.l.x = -1000;
    cross.l.y =  1000;
    cross.r.x =  1000;
    cross.r.y = -1000;
    // (max(l.x), min(l.y))
    // (min(r.x), max(r.y))
    for (int i = 0; i < g_slt.i; i++) {
        if (cross.l.x < g_slt.rect[i].l.x) {
            cross.l.x = g_slt.rect[i].l.x;
        }
        if (cross.l.y > g_slt.rect[i].l.y) {
            cross.l.y = g_slt.rect[i].l.y;
        }
        if (cross.r.x > g_slt.rect[i].r.x) {
            cross.r.x = g_slt.rect[i].r.x;
        }
        if (cross.r.y < g_slt.rect[i].r.y) {
            cross.r.y = g_slt.rect[i].r.y;
        }
    }

    if ((cross.l.x >= cross.r.x) || (cross.l.y <= cross.r.y)) {
        printf("0\n");
    } else {
        printf("%d\n", (cross.r.x - cross.l.x)*(cross.l.y - cross.r.y));
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    solution(
        "1 6 4 4\n"
        "3 5 3 4\n"
        "0 3 7 3\n"
    );

    return 0;
}
