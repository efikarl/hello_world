/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    正方形数量
---------------------------------------------------------------------------------
    [i] 坐标信息
        line 1    : N
        line 2(1) : x1 y1
        line  (n) : xn yn
    [o] 构成的正方形数量
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

typedef struct
{
    int             x;
    int             y;
} point_t;


typedef struct {
    int             n;
    point_t         *p;
    point_t         s[4]; // 四点组合
    point_t         t[4]; // 临时空间，备份 s
    int             i;
    int             count;
} solution_t;

solution_t          g_slt;

int pcmp(const void *p1, const void *p2)
{
    point_t *a = (point_t *) p1;
    point_t *b = (point_t *) p2;

    int xd = (a->x > b->x) - (a->x < b->x);
    int yd = (a->y > b->y) - (a->y < b->y);

    if (xd) {
        return xd;
    } else {
        return yd;
    }
}

int ip_parse(char *input)
{
    char *p, *end;

    // code
    p = end = input;
    g_slt.n = strtol(p, &end, 10);
    end++;
    g_slt.p = calloc(g_slt.n, sizeof(point_t));

    do {
        if (g_slt.i >= g_slt.n) {
            break;
        }
        p = end;
        g_slt.p[g_slt.i].x = (int) strtol(p, &end, 10);
        end++;
        p = end;
        g_slt.p[g_slt.i].y = (int) strtol(p, &end, 10);
        end++;
        g_slt.i++;
    } while (end[0] != 0);
    g_slt.i = 0; // reset i for square_points

    // 为了整体性能，先整体排序，那么组合时就是良序
    qsort(g_slt.p, g_slt.n, sizeof(point_t), pcmp);

#if APP_DEBUG
    for (int i = 0; i < g_slt.n; i++) {
        printf("(%d, %d)", g_slt.p[i].x, g_slt.p[i].y);
        if (i != g_slt.n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
#endif

    return 0;
}

// x0 y0
// x0 y1
// x1 y0
// x1 y1
int is_square()
{
    // 因为排序会破环原有数据，备份必要！
    memmove(g_slt.t, g_slt.s, sizeof(g_slt.s)); qsort(g_slt.t, 4, sizeof(point_t), pcmp);

#if APP_DEBUG
    for (int i = 0; i < 4; i++) {
        printf("(%d, %d)", g_slt.t[i].x, g_slt.t[i].y);
        if (i != 3) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
#endif

    if (g_slt.t[2].x == g_slt.t[0].x) {
        return 0;
    }
    if ((g_slt.t[0].x == g_slt.t[1].x) && (g_slt.t[2].x == g_slt.t[3].x) &&
        (g_slt.t[0].y == g_slt.t[2].y) && (g_slt.t[1].y == g_slt.t[3].y)) {
        if (abs(g_slt.t[3].x - g_slt.t[0].x) == abs(g_slt.t[3].y - g_slt.t[0].y)) {
            return 1;
        }
    }
    return 0;
}

void square_points(int next)
{
    if (g_slt.i + g_slt.n - next < 4) {
        return;
    }

    if (g_slt.i >= 4) {
        if (is_square()) {
            g_slt.count++;
        }
    } else {
        g_slt.s[g_slt.i] = g_slt.p[next];
        g_slt.i++;
        square_points(next + 1);
        g_slt.i--;
        square_points(next + 1);
    }
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
    square_points(0);
    printf("%d\n", g_slt.count);

    free(g_slt.p);
    return 0;
}

int main(int argc, char const *argv[])
{
    solution(
        "7\n"
        "1 1\n"
        "1 0\n"
        "0 1\n"
        "0 0\n"
        "1 2\n"
        "2 1\n"
        "2 2\n"
    );

    return 0;
}
