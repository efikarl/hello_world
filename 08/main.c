/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    斗地主：顺子(易错)
---------------------------------------------------------------------------------
    [i]
        line 1: 自己的牌
        line 2: 桌面的牌
    [o]
        与桌面的牌中长度一致的最大顺子，否则: NO-CHAIN
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

#define MAX_CHAIN_CARD  12

typedef struct {
    char        c;          // 卡牌
    int         n;          // 编号
} card_t;

card_t g_cart[] = {
    {  '3', 0x00 },
    {  '4', 0x01 },
    {  '5', 0x02 },
    {  '6', 0x03 },
    {  '7', 0x04 },
    {  '8', 0x05 },
    {  '9', 0x06 },
    {  '0', 0x07 },
    {  'J', 0x08 },
    {  'Q', 0x09 },
    {  'K', 0x0a },
    {  'A', 0x0b },
    // 注：以下不成顺子，不可连续
    {  '2', 0x10 },
    {  'B', 0x11 },
    {  'C', 0x12 },
};

int get_card_num(char card)
{
    int  i;

    for (i = 0; i < sizeof(g_cart)/sizeof(card_t); i++) {
        if (card == g_cart[i].c) {
            break;
        }
    }

    return g_cart[i].n;
}

typedef struct {
    char        self[MAX_CHAIN_CARD+1];
    char        desk[MAX_CHAIN_CARD+1];
    int         desk_chain_index;
    int         desk_chain_lenth;
    int         self_chain_index;
    bool        self_chain_found;
} solution_t;

solution_t      g_slt;

int in_parse(char *input)
{
    int i, j, l;
    char *p = input;

    // 1. 卡牌解析
    while (p[0] != '\n') {
        if (p[0] == '-') {
            p++;
            continue;
        }
        int n = (p[0] == '1') && (p[1] == '0') ? get_card_num(p[1]):get_card_num(p[0]);
        if (n >= MAX_CHAIN_CARD) {
            p++;
            continue;
        }

        if ((p[0] == '1') && (p[1] == '0')) {
            g_slt.self[n] = p[1];
            p++;
        } else {
            g_slt.self[n] = p[0];
        }

        p++;
    }
    if (p[0] != '\n') {
        return -1;
    } else {
        p++; // '\n'
    }

    while (p[0] != '\n') {
        if (p[0] == '-') {
            p++;
            continue;
        }
        int n = (p[0] == '1') && (p[1] == '0') ? get_card_num(p[1]):get_card_num(p[0]);
        if (n >= MAX_CHAIN_CARD) {
            p++;
            continue;
        }

        if ((p[0] == '1') && (p[1] == '0')) {
            g_slt.desk[n] = p[1];
            p++;
        } else {
            g_slt.desk[n] = p[0];
        }

        p++;
    }
    if (p[0] != '\n') {
        return -1;
    } else {
        p++; // '\n'
    }

    // 2. 目标顺子
    for (i = 0; i < MAX_CHAIN_CARD; i++) {
        if (!g_slt.desk[i]) {
            continue;
        }
        j = i;
        while (g_slt.desk[i]) i++;
        if (i - j > g_slt.desk_chain_lenth) {
            g_slt.desk_chain_index = j;
            g_slt.desk_chain_lenth = i - j;
        }
        i--;
    }
    if (g_slt.desk_chain_lenth < 5) {
        g_slt.desk_chain_index = 0;
        g_slt.desk_chain_lenth = 0;
    }
#if APP_DEBUG
    for (i = 0; i < MAX_CHAIN_CARD; i++) {
        printf("%c", g_slt.self[i]?g_slt.self[i]:'*');
    }
    printf("\n");
    for (i = 0; i < MAX_CHAIN_CARD; i++) {
        printf("%c", g_slt.desk[i]?g_slt.desk[i]:'*');
    }
    printf("\n");
#endif

    return 0;
}

int solution(char *input)
{
    int i, j;

    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    in_parse(input);

    if (g_slt.desk_chain_lenth >= 5) {
        for (i = 0; i < MAX_CHAIN_CARD; i++) {
            if (!g_slt.self[i]) {
                continue;
            }
            j = i;
            while (g_slt.self[i]) i++;
            if (i - j >= g_slt.desk_chain_lenth) {
                g_slt.self_chain_found = true;
                g_slt.self_chain_index = i - g_slt.desk_chain_lenth;
            }
            i--;
        }
    }
#if APP_DEBUG
    printf("desk_chain_index = %d, desk_chain_lenth = %d, self_chain_found = %d, self_chain_index = %d\n",
        g_slt.desk_chain_index, g_slt.desk_chain_lenth, g_slt.self_chain_found, g_slt.self_chain_index);
#endif
    if (g_slt.self_chain_found) {
        for (i = g_slt.self_chain_index; i < g_slt.self_chain_index + g_slt.desk_chain_lenth; i++) {
            if (g_slt.self[i] == '0') {
                printf("10");
            } else {
                printf("%c", g_slt.self[i]);
            }
            if (i != g_slt.self_chain_index + g_slt.desk_chain_lenth - 1) {
                printf("-");
            } else {
                printf("\n");
            }
        }
    } else {
        printf("NO-CHAIN\n");
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    solution(
        "A-5-5-3-3-3-3-4-A-4-6-7-8-9-10-J-Q-K\n"
        "2-3-8-8-9-10-J-Q-K-A-B-C\n"
    );

    return 0;
}
