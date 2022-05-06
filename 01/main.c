/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    min(|nums[x]+nums[y]|)
---------------------------------------------------------------------------------
    [i] 以空格分隔的有序数字字串
    [o] min(|nums[x]+nums[y]|) note: x!=y
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    long    nums[1000];
} solution_t;

solution_t  g_slt;

int in_parse(char *input)
{
    int     i = 0;
    char    * end;
    int     j = 0;

    if (!input) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    for (i = strtol(input, &end, 10); input != end; input = end, i = strtol(input, &end, 10)) {
        g_slt.nums[j++] = i;
    }

    return --j;
}

int solution(char *input)
{
    int i, j, min, tmp, cnt;

    cnt = in_parse(input);
    if (cnt <= 0) {
        return -1;
    }

    min = 0;
    for (i = 0; i < cnt - 1; i++) {
        for (j = i + 1; j < cnt; j++) {
            tmp = abs(g_slt.nums[i] + g_slt.nums[j]);
            if (!tmp) {
                return tmp;
            }
            if (tmp < min) {
                min = tmp;
            }
        }
    }
    return min;
}

int main(int argc, char const *argv[])
{
    printf("%d\n", solution("-3 -1 5 7 11"));
    return 0;
}
