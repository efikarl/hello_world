/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    最长元音子串
---------------------------------------------------------------------------------
    [i]
        字串：长度(0, 0xffff) && 英文字母组成
    [o]
        最长元音子串的长度
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STR_LEN 0xffff

typedef struct {
    int             len;
} solution_t;

solution_t          g_slt;

bool is_vowel(char c)
{
    switch (c)
    {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
    case 'A':
    case 'E':
    case 'I':
    case 'O':
    case 'U':
        return true;
    default:
        return false;
        break;
    }
}

int solution(char *input)
{
    int   len;
    char *end;

    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }
    len = strlen(input);
    if (len <= 0 || len >= 0xffff) {
        return -1;
    }

    for (end = input; end[0] != '\0'; end++) {
        len = 0;
        while (is_vowel(end[0])) {
            end++;
            len++;
        }
        if (len > g_slt.len) {
            g_slt.len = len;
        }
    }
    
    printf("%d\n", g_slt);
}

int main(int argc, char const *argv[])
{
    solution("aeiouAEIOUasdbuiodevauufg");
    return 0;
}
