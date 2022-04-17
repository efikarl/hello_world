/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    寻找戈多
---------------------------------------------------------------------------------
    [i] 
        line 1: n m （代表 n x m 矩阵）
        line 2: 戈多
        line 3(1): 1th (长度m的字串)
        ..
        line  (n): nth (长度m的字串)
    [o]
        戈多的位置
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_M   20
#define MAX_S   100

typedef struct {
    int     n;
    int     m;
    char*   search;     // 寻找的内容
    int     length;     // 寻找的长度
    char    **data;     // 指向数据的指针
    int     result[2];  // 结果（1-基）
} solution_t;

solution_t  g_slt;

int ip_parse(char *input)
{
    int   cnt;
    char *p, *end;

    end = input;
    p   = end;
    g_slt.n = (int) strtol(p, &end, 10);
    p   = end;
    g_slt.m = (int) strtol(p, &end, 10);

    while(end[0] != '\n') end++;
    end++; // '\n'
    g_slt.search  = end;
    while(end[0] != '\n') end++;
    g_slt.length  = (int) (end - g_slt.search) > MAX_S ? MAX_S : (int) (end - g_slt.search);
    end++; // '\n'
    // 注：此时必须使用*alloc分配空间
    g_slt.data = calloc(g_slt.n, sizeof(char*));
    for (int i = 0; i < g_slt.n; i++) {
        g_slt.data[i] = end;
        cnt = 0;
        while(end[0] != '\n') {
            cnt++;
            end++;
            if (cnt >= MAX_M) {
                break;
            }
        }
        end++;
    }

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

    int i, j, k, t, found;
    for (i = 0; i < g_slt.n; i++) {
        found = 0;
        for (j = 0; j < g_slt.m; j++) {
            if (g_slt.data[i][j] == '\n') {
                break;
            }
            found = 1;
            for (k = 0, t = 0; k < g_slt.length; k++, t++) {
                if ((&g_slt.data[i][j])[t] == '\n') {
                    t++;
                }
                if (g_slt.search[k] != (&g_slt.data[i][j])[t]) {
                    found = 0;
                }
            }
            if (found) {
                g_slt.result[0] = i+1;
                g_slt.result[1] = j+1;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    if (found) {
        printf("%d %d\n", g_slt.result[0], g_slt.result[1]);
    } else {
        printf("NO\n");
    }

    if (g_slt.data) {
        free(g_slt.data);
    }
}

int main(int argc, char const *argv[])
{
    solution(
        "5 5\n"
        "HELLOWORD\n"
        "CPUCY\n"
        "EKLQH\n"
        "CHELLO\n"
        "WORD\n"
        "DGRBC\n"
    );

    return 0;
}
