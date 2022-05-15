/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    叠积木 => 将集合划分为和相等的最大数量子集问题
---------------------------------------------------------------------------------
    假如： (1, 2, 3) => (1, 2) + (3):
    n   [i].v                   b i t
    1 = [0].1                   0 0 1
    2 = [1].2                   0 1 0
    3 = [2].3                   1 0 0
    4 = [0].1, [1].2            0 1 1
    5 = [0].1, [2].3            1 0 1
    6 = [1].2, [2].3            1 1 0
    7 = [0].1, [1].2, [2].3     1 1 1
---------------------------------------------------------------------------------
    [i] 空格分隔的积木长度
    [o] 长度相等的最高高度
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// #define NDEBUG

#ifndef NDEBUG
#define lszin "lsz.in.testcase"
#endif

#define MAX_SET     5
#define MAX_LEN     5

struct {
    int             set[MAX_SET];
    int             n;
    int             sub[MAX_SET];
    int             has[MAX_SET];
    int             m;
    int             r;
} g_slt;


int in_parse()
{
    // code
    while (scanf("%d", &g_slt.set[g_slt.n]) != EOF) {
        g_slt.n++;
        if (g_slt.n > MAX_SET) {
            break;
        }
    }

    return 0;
}

// 将N元素装M桶问题
void subs(int next, int i)
{
    if (next >= g_slt.n) {
        // 所有球都放完了，看看是否每个桶都相等
        i = 1;
        while ((i < g_slt.m) && (g_slt.sub[i-1] == g_slt.sub[i])) {
            if (!g_slt.has[i-1] || !g_slt.has[i]) {
                break;
            } else {
                i++;
            }
        }
        if (i >= g_slt.m) {
            if (g_slt.sub[0] <= MAX_LEN) {
                g_slt.r = g_slt.m;
            }
        }
        return;
    }

    // 每个元素都有M种选择
    for (i = 0; i < g_slt.m; i++) {

        // 对每个桶：

        // （1）就是它了~
        g_slt.sub[i] += g_slt.set[next];
        g_slt.has[i] ++;
        subs(next + 1, i);
        // （2）再考虑考虑
        g_slt.sub[i] -= g_slt.set[next];
        g_slt.has[i] --;
    }
}

int solution()
{
    // code
    for (int i = g_slt.n; i > 0; i--) {
        g_slt.m = i;
        g_slt.r = 0;
        memset(g_slt.sub, 0, g_slt.m);
        memset(g_slt.has, 0, g_slt.m);

        subs(0, 0);
        if (g_slt.r) {
            break;
        }
    }
    if (!g_slt.r) {
        g_slt.r = -1;
    }
    printf("%d\n", g_slt.r);

    return 0;
}

int main(int argc, char const *argv[])
{
#ifndef NDEBUG
    if (!freopen(lszin, "r", stdin)) {
        printf("%s: not found, please create test case in it\n", lszin);
        fclose(fopen(lszin, "w"));
        goto _end;
    }
#endif

    in_parse();
    solution();

#ifndef NDEBUG
    fclose(stdin);
_end:
#endif
    return 0;
}
