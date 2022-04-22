/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    字段加密（经典）
---------------------------------------------------------------------------------
    [i] 
        line 1: 命令字索引K
        line 2: 命令字符串S
                1. 长度<128的字母+数字+下划线+双引号
                2. 命令字以下划线分隔
                3. 双引号标识：_命令字|空命令字("""")
    [o] 
        索引字段加密后的结果
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct {
    int         index;
    char        *head;
    char        *tail;
    char        *cmds;
    char        *cend;
} solution_t;

solution_t      g_slt;

int ip_parse(char *input)
{
    char *p, *end;

    p = end = input;
    g_slt.index = (int) strtol(p, &end, 10);

    while(end[0] != '\n') end++;
    end++;

    g_slt.cmds = end;
    while(end[0] != '\n') end++;
    g_slt.cend = end;

    return 0;
}

int next_cmd(char *p, char **n)
{
    char *end = p;

    if (end[0] == '"') {
        end++;
        if (end[0] == '"') {
            end++;
            end++;
        } else {
            while (isalnum(end[0])||(end[0] == '_')) end++;
        }
        end++;
    } else {
        while (isalnum(end[0])) end++;
    }
    *n = end;

    printf("p:n = %c:%c\n", *p, *(*n-1));
    if (end[0] == '_') {
        return 1;
    } else {
        return 0;
    }
}

int solution(char *input)
{
    char i, *p, *n;

    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }
    ip_parse(input);
    printf("%s", g_slt.cmds);

    p = g_slt.cmds;
    for (i = 0; next_cmd(p, &n); i++) {
        if (i == g_slt.index) {
            break;
        }
        p = ++n;
    }

    if (i != g_slt.index) {
        printf("ERROR\n");
    } else {
        g_slt.head = p;
        g_slt.tail = n;
        printf("%.*s******%.*s\n", g_slt.head - g_slt.cmds, g_slt.cmds, g_slt.cend - g_slt.tail, g_slt.tail);
    }
}

int main(int argc, char const *argv[])
{
    solution(
        "5\n"
        "password_a12345678_timeout_10_\"\"\"\"_\"un_kn0wn\"\n"
        );
    return 0;
}
