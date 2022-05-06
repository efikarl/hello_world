/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    贪吃蛇
---------------------------------------------------------------------------------
    [i] 每行元素间均为空格分隔
        line 1      : 移动: G 方向: U(上) D(下) L(左) R(右)
        line 2      : 空间：N(行数) M(列数)
        line 3(1)   : 元素：H(蛇头) F(食物) E(空白)，注：每行M个元素，蛇头唯一
        line  (N)   : 元素：H(蛇头) F(食物) E(空白)，注：每行M个元素，蛇头唯一
    [o]
        贪吃蛇的最终长度
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

typedef struct {
    int             x;
    int             y;
} coordinate_t;

typedef struct {
    coordinate_t    h;
    coordinate_t    *s;
    int             head;
    int             tail;
    int             s_count;
    int             s_size;
    char            *space;
    char            last_d;
    int             edge_u;
    int             edge_d;
    int             edge_l;
    int             edge_r;
} solution_t;

solution_t          g_slt;

char what_is_for_head()
{
    int   v, i, j;
    char *end = g_slt.space, box;

    for (i = 0; i <= g_slt.edge_d; i++) {
        for (j = 0; j <= g_slt.edge_r; j++) {
            while(end[0] == ' ') end++;

            if ((g_slt.h.x == j) && (g_slt.h.y == i)) {
                box = end[0];
                end[0] = 'E';        // 食物吃掉还存在吗？否。注：input 只读, 所以移动到了 g_slt.space！
                return box;
            }
            end++;
        }
        end++;
    }
#if APP_DEBUG
    printf("game over!!!\n");
#endif
    return 'E';                         // 如果参数绝对正确，永不情况！
}

bool is_hit_edge()
{
    if (((g_slt.h.x >= g_slt.edge_l) && (g_slt.h.x <= g_slt.edge_r)) &&
        ((g_slt.h.y >= g_slt.edge_u) && (g_slt.h.x <= g_slt.edge_d))) {
        return false;
    } else {
        return true;
    }
}

bool is_hit_self()
{
    int count = g_slt.s_count;
    for (int i = g_slt.tail % count; i != (g_slt.head+1) % count; i = (i+1) % count) {
#if APP_DEBUG
        printf("self: head:tail.count=[%04d:%04d].%04d [%d]=(%04d, %04d)\n", g_slt.head, g_slt.tail, count, i, g_slt.s[i].x, g_slt.s[i].y);
#endif
        if ((g_slt.h.x == g_slt.s[i].x) && (g_slt.h.y == g_slt.s[i].y)) {
            return true;
        }
    }
    return false;
}

bool go_next()
{
    int count = g_slt.s_count;

    switch (g_slt.last_d) {
    case 'U':
        g_slt.h.y--;
        break;
    case 'D':
        g_slt.h.y++;
        break;
    case 'L':
        g_slt.h.x--;
        break;
    case 'R':
        g_slt.h.x++;
        break;
    default:
        break;
    }

    if (is_hit_edge() || is_hit_self()) {
        return false;
    } else {
        g_slt.s[g_slt.head] = g_slt.h;
        g_slt.head++;
        if (what_is_for_head() != 'F') {
            g_slt.s[g_slt.tail].x = -1;
            g_slt.s[g_slt.tail].y = -1;
            g_slt.tail++;
        }
        if (g_slt.head >= count) {
            g_slt.head = g_slt.head % count;
        }
        if (g_slt.tail >= count) {
            g_slt.tail = g_slt.tail % count;
        }
        return true;
    }
}

int in_parse(char *input)
{
    int   v, i, j;
    char *p, *end;

    end = input;
    while(end[0] != '\n') end++;
    end++; // '\n'

    p = end;
    g_slt.edge_u = 0;
    g_slt.edge_d = (int) strtol(p, &end,  10) - 1;
    p = end;
    g_slt.edge_l = 0;
    g_slt.edge_r = (int) strtol(p, &end,  10) - 1;

    g_slt.s_count = (g_slt.edge_d+1)*(g_slt.edge_r+1);
    g_slt.s_size = g_slt.s_count*sizeof(coordinate_t);
    g_slt.s = malloc(g_slt.s_size); memset(g_slt.s, -1, g_slt.s_size);

    while(end[0] != '\n') end++;
    end++; // '\n'

    g_slt.space = end;
    for (i = 0; i <= g_slt.edge_d; i++) {
        for (j = 0; j <= g_slt.edge_r; j++) {
            while(end[0] == ' ') end++;

            if (end[0] == 'H') {
                g_slt.h.x = j;
                g_slt.h.y = i;
                g_slt.s[g_slt.head] = g_slt.h;
                g_slt.head++;
                g_slt.tail++;
            }
            end++;
        }
        end++;
    }
    while(end[0] != '\n') end++;
    end++; // '\n'

    char  *orig = g_slt.space;
    size_t size = end - orig;
    g_slt.space = calloc(size, sizeof(char)); memmove(g_slt.space, orig, size);
}

int solution(char *input)
{
    char *end;
    bool game_over = false;

    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    in_parse(input);

    end = input;
    while (end[0] != '\n') {
        switch (end[0]) {
        case 'U':
        case 'D':
        case 'L':
        case 'R':
            g_slt.last_d = end[0];
            break;
        case 'G':
            if (!go_next()) {
                game_over = true;
            }
            break;
        default:
            end++; // ' '
            continue;
        }
#if APP_DEBUG
        printf("step: %c => h(%04d, %04d)\n", end[0], g_slt.h.x, g_slt.h.y);
#endif
        if (game_over) {
            break;
        }
        end++;
    }
    printf("%d\n", (g_slt.head + g_slt.s_count - g_slt.tail + 1) % g_slt.s_count);

    if (g_slt.s) {
        free(g_slt.s);
    }
    if (g_slt.space) {
        free(g_slt.space);
    }
}

int main(int argc, char const *argv[])
{
    solution(
        "U G L G D G G L G U G G\n"
        "3 3\n"
        "F F F\n"
        "F F H\n"
        "E F E\n"
    );

    solution(
        "R G G D G G L G G U G G R G G D G G L G G U G G\n"
        "3 3\n"
        "H E E\n"
        "E F E\n"
        "E E F\n"
    );

    return 0;
}
