/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    最大不等式
---------------------------------------------------------------------------------
    [i] 各元素以逗号分隔，各参数以分号分隔
        [1] 不等式系数
        [2] 不等式变量
        [3] 不等式约束
        [4] 不等式目标
        形式：[1];[2];[4];[3]
    [o]
        不等式成立与否 不等式最大差值
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

#define MAX_VAR   1000
#define MAX_INE   1000

typedef struct {
    double  a[MAX_INE][MAX_VAR];    // 系数
    int     v[MAX_VAR];             // 变量
    int     c[MAX_INE];             // 约束: 1: > 2: >= -1: < -2 <=
    double  t[MAX_INE];             // 目标
    int     var_cnt;
    int     exp_cnt;
    double  l_value[MAX_INE];       // 左值
    bool    truth[MAX_INE];         // 真值
    double  d_value[MAX_INE];       // 差值
} solution_t;

solution_t  g_slt;

int ip_parse(char *input)
{
    char    *p, *end;
    double  v;
    int     input_len, i, j;

    // 不等式数量
    input_len = strlen(input);
    for (int i = input_len-1; input[i] != ';'; i--) {
        if (input[i] == ',') {
            g_slt.exp_cnt++;
        }
    }
    g_slt.exp_cnt++;
    // 其变量数量
    for (int i = 0; input[i] != ';'; i++) {
        if (input[i] == ',') {
            g_slt.var_cnt++;
        }
    }
    g_slt.var_cnt++;

    end = input, i = 0, j = 0;
    do {
        p = end;
        v = strtod(p, &end);
        if (i < g_slt.exp_cnt) {
            // 分析系数
            g_slt.a[i][j] = v;
        } else {
            if (i == g_slt.exp_cnt+0) {
                // 分析变量
                g_slt.v[j] = (int) v;
            } else
            if (i == g_slt.exp_cnt+1) {
                // 分析目标
                g_slt.t[j] = v;
            }
        }
        while(!(end[0] == ',' || end[0] == ';')) end++;
        switch (end[0]) {
        case ',':
            end++;
            j++;
            break;
        case ';':
            end++;
            i++;
            j=0;
            break;
        default:
            break;
        }
        if (i >= g_slt.exp_cnt+2) {
            break;
        }
    } while (p != end);
    // 分析约束
    p = end;
    for (i = 0, j = 0; i < input_len; i++, j++) {
        switch (p[i]) {
        case '>':
            if (p[i+1] != '=') {
                g_slt.c[j] = 1;
            } else {
                i++;
                g_slt.c[j] = 2;
            }
            i++; // 分隔
            break;
        case '<':
            if (p[i+1] != '=') {
                g_slt.c[j] = -1;
            } else {
                i++;
                g_slt.c[j] = -2;
            }
            i++; // 分隔
            break;
        default:
            break;
        }
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

    for (int i = 0; i < g_slt.exp_cnt; i++) {
        g_slt.l_value[i] = 0;
        // 计算不等式左值
        for (int j = 0; j < g_slt.var_cnt; j++) {
            g_slt.l_value[i] += g_slt.a[i][j] * g_slt.v[j];
        }
        // 计算不等式差值
        g_slt.d_value[i] = g_slt.l_value[i] - g_slt.t[i];
        // 分析不等式真假
        if (g_slt.d_value[i] == 0) {
            if (g_slt.c[i] == 2 || g_slt.c[i] == -2) {
                g_slt.truth[i] = true;
            } else {
                g_slt.truth[i] = false;
            }
        } else if (g_slt.d_value[i] > 0) {
            if (g_slt.c[i] > 0) {
                g_slt.truth[i] = true;
            } else {
                g_slt.truth[i] = false;
            }
        } else if (g_slt.d_value[i] < 0) {
            if (g_slt.c[i] < 0) {
                g_slt.truth[i] = true;
            } else {
                g_slt.truth[i] = false;
            }
        }
    }

#if APP_DEBUG
    for (int i = 0; i < g_slt.exp_cnt; i++) {
        for (int j = 0; j < g_slt.var_cnt; j++) {
            printf("%9.4f * %-4d", g_slt.a[i][j], g_slt.v[j]);
            if (j != g_slt.var_cnt - 1) {
                printf(" + ");
            }
        }
        printf("(l=%-9.4f)", g_slt.l_value[i]);
        char *p;
        switch (g_slt.c[i])
        {
        case 1:
            p = " >  ";
            break;
        case 2:
            p = " >= ";
            break;
        case -1:
            p = " <  ";
            break;
        case -2:
            p = " <= ";
            break;
        default:
            p = "    "; 
            break;
        }
        printf("%s%9.4f    ", p, g_slt.t[i]);
        printf("(-=%9.4f)\n", g_slt.d_value[i]);
    }
#endif

    bool truth = true;
    double max = g_slt.d_value[0];
    for (int i = 0; i < g_slt.exp_cnt; i++) {
        // 不等式整体真假
        truth  = truth && g_slt.truth[i];
        // 不等式最大差值
        for (int j = i + 1; j < g_slt.exp_cnt; j++) {
            if (max < g_slt.d_value[j]) {
                max = g_slt.d_value[j];
            }
        }
    }

    // 打印结果
    double  max_ipart;
    double  max_fpart = modf(max, &max_ipart);
    printf("%s %.0f\n", truth?"true":"false", max_ipart);
}

int main(int argc, char const *argv[])
{
    solution("1,2,3,4,5;5,4,3,2,1;1,2,1,2,1;1,2,3,4,5;12.3,34.5,56.7;>,>=,<");
    return 0;
}
