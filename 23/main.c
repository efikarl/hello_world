/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    h   [i].v                   b i t
    1 = [0].1                   0 0 1
    2 = [1].2                   0 1 0
    3 = [2].3                   1 0 0
    4 = [0].1, [1].2            0 1 1
    5 = [0].1, [2].3            1 0 1
    6 = [1].2, [2].3            1 1 0
    7 = [0].1, [1].2, [2].3     1 1 1
---------------------------------------------------------------------------------
    [i]
    [o]
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <lszlib.h>

// #define NDEBUG

#ifndef NDEBUG
#define lszin "lsz.in.testcase"
#endif

#define MAX_SET     5

typedef struct {
    int             index;
    int             value;
} set_ele_t;

typedef struct {
    int             cnt;
    set_ele_t      *ele;
    int             sum;
} sub_set_t;

struct {
    int             set[MAX_SET];
    int             n;
    int             subset_v[MAX_SET];
    int             subset_i[MAX_SET];
    int             m;
    int             i;
    void           *subset_tree;
    int             subset_base;
    sub_set_t       subset_data;
    void           *subcmb_tree;
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
    g_slt.subset_tree = tree_new(sizeof(int), sizeof(sub_set_t), lsz_icmp);
    return 0;
}

void build_subset()
{
    g_slt.subset_base++;

    g_slt.subset_data.cnt = g_slt.m;
    g_slt.subset_data.ele = calloc(g_slt.m, sizeof(set_ele_t));
    g_slt.subset_data.sum = 0;
    for (int i = 0; i < g_slt.m; i++) {
        g_slt.subset_data.ele[i].index = g_slt.subset_i[i];
        g_slt.subset_data.ele[i].value = g_slt.subset_v[i];
        g_slt.subset_data.sum         += g_slt.subset_v[i];
    }

    tree_insert(g_slt.subset_tree, &g_slt.subset_base, &g_slt.subset_data);
}

void subs(int n)
{
    if (g_slt.i + g_slt.n - n < g_slt.m) {
        return;
    }
    if (g_slt.i >= g_slt.m) {
        build_subset();
        return;
    } else {
        g_slt.subset_i[g_slt.i] = n;
        g_slt.subset_v[g_slt.i] = g_slt.set[n];
        g_slt.i++;
        subs(n + 1);
        g_slt.i--;
        subs(n + 1);
    }
}

void tree_for_each_callback(lsz_tree_t *node, void *data)
{
    sub_set_t *subset = node->data;

#ifndef NDEBUG
    for (int i = 0; i < subset->cnt; i++) {
        if (i == 0) {
            printf("(");
        }

        printf("[%d].%d", subset->ele[i].index, subset->ele[i].value);

        if (i != subset->cnt - 1) {
            printf(",");
        } else {
            printf(")\n");
        }
    }
#endif
}

int solution()
{
    // code
    for (int i = 1; i < g_slt.n; i++) {
        g_slt.m = i;
        g_slt.i = 0;
        subs(0);
    }
    tree_for_each(g_slt.subset_tree, tree_for_each_callback, NULL);

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
