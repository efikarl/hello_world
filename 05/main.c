/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    五键键盘
---------------------------------------------------------------------------------
    [i] 以空格分隔的数字，其中数字代表键盘按键
        1: a
        2: ctrl-c
        3: ctrl-x
        4: ctrl-v
        5: ctrl-a
    [o]
        最终屏幕上字母的数量
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

typedef struct
{
    int     input[1000];
    int     count_input;
    bool    select;
    int     count_screen;
    int     count_clipbd;
} solution_t;

solution_t  g_slt;

int ip_parse(char *input)
{
    char *p, *end;

    end = input;
    do {
        p = end;
        g_slt.input[g_slt.count_input++] = (int) strtol(p, &end, 10);
    } while(p != end);
    g_slt.count_input--;

    return 0;
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    ip_parse(input);

    for (int i = 0; i < g_slt.count_input; i++) {
#if APP_DEBUG
        printf("(+) select: %d, screen: %d, clipbd: %d\n", g_slt.select, g_slt.count_screen, g_slt.count_clipbd);
#endif
        switch (g_slt.input[i]) {
        case 1: // a
            if (g_slt.select) {
                g_slt.count_screen = 1;
            } else {
                g_slt.count_screen+= 1;
            }
            g_slt.select = false;
            break;
        case 2: // ctrl-c
            if (g_slt.select) {
                g_slt.count_clipbd = g_slt.count_screen;
            }
            break;
        case 3: // ctrl-x
            if (g_slt.select) {
                g_slt.count_clipbd = g_slt.count_screen;
                g_slt.count_screen = 0;
            }
            break;
        case 4: // ctrl-v
            if (g_slt.select) {
                g_slt.count_screen = g_slt.count_clipbd;
            } else {
                g_slt.count_screen+= g_slt.count_clipbd;
            }
            g_slt.select = false;
            break;
        case 5: // ctrl-a
            g_slt.select = true;
            break;
        default:
            printf("input invalid:[%d] %d\n", i, g_slt.input[i]);
            break;
        }
#if APP_DEBUG
        printf("(-) select: %d, screen: %d, clipbd: %d\n", g_slt.select, g_slt.count_screen, g_slt.count_clipbd);
#endif
    }

    printf("%d\n", g_slt.count_screen);

    return 0;
}

int main(int argc, char const *argv[])
{
    solution("1 1 1 5 3 4");

    return 0;
}
