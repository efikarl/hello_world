/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    字串一次交换的最小字串 => 字串中最小字母与首字母交换
---------------------------------------------------------------------------------
    [i] 一串小写字母组成的字串 s
    [o] 最小字串
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
    char            *min;
} solution_t;

solution_t          g_slt;

void swap(char *a, char *b)
{
    char t = *a; *a = *b; *b = t;
}

int solution(char *input)
{
    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    // code
    char *dup = strdup(input);
    char *end = dup;
    g_slt.min = dup;
    while (end[0] != '\0') {
        if (end[0] <= g_slt.min[0]) {
            g_slt.min = end;
        }
        end++;
    }
    end = dup;
    while (end[0] == g_slt.min[0]) end++;

    swap(end, g_slt.min);
    printf("%s\n", dup);
    free(dup);

    return 0;
}

int main(int argc, char const *argv[])
{
    solution("aabcdefa");

    return 0;
}
