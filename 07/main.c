/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    报数问题：N人围成一圈，从1到3报数，报到3的人退出，直到剩1个人为止
---------------------------------------------------------------------------------
    [i] 
        n: 人数
    [o]
        最后的人的最初编号
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "list.h" // this line introduce macro LSZ_LIST
#include "benchmark.h"

#ifndef APP_DEBUG
#define APP_DEBUG 0
#endif

#define MAX_PEOPLE 1000

#ifndef LSZ_LIST

typedef struct {
    int         num;
    bool        out;
} people_t;

typedef struct {
    people_t    who[MAX_PEOPLE];
    int         out;
    int         cnt;
    int         last_person;
} solution_t;

solution_t      g_slt;

void count(int n) {
    if (n - g_slt.out <= 1) {
        return;
    }
    for (int i = 0; i < n; i++) {
        if (g_slt.who[i].out) {
            continue;
        }
        g_slt.cnt = g_slt.cnt % 3 + 1;
        if (g_slt.cnt == 3) {
            g_slt.who[i].out = true;
            g_slt.out++;
        }
#if APP_DEBUG
        printf("who: num=%-3d, out=%d; out = %-3d, cnt=%d\n", g_slt.who[i].num, g_slt.who[i].out, g_slt.out, g_slt.cnt);
#endif
    }
    count(n);
}

int solution(int n)
{
    if (n <= 0 || n >= MAX_PEOPLE) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
    }

    for (int i = 0; i < n; i++) {
        g_slt.who[i].num = i+1;
    }

    count(n);

    for (int i = 0; i < n; i++) {
        if (!g_slt.who[i].out) {
            g_slt.last_person = g_slt.who[i].num;
            break;
        }
    }

    printf("%d\n", g_slt.last_person);
}

#else   // #ifndef LSZ_LIST

typedef struct {
    lsz_list_t  link;
    int         num;
} people_t;

typedef struct {
    lsz_list_t  list;
    int         cnt;
#if APP_DEBUG
    int         out;
#endif
    int         last_person;
} solution_t;

solution_t      g_slt;

void count(lsz_list_t *list)
{

    lsz_list_t *this = NULL;
    lsz_list_t *next = NULL;

    if (!list) {
        return;
    }
    if (is_list_empty(list)) {
        return;
    }
    for (this = list->next, next = this->next; this != list; this = next, next = this->next) {
        people_t *people = base_of(this, people_t, link);

        if (next == list) {
            g_slt.last_person = people->num;
        }
        g_slt.cnt = g_slt.cnt % 3 + 1;
#if APP_DEBUG
            printf("who: num=%-3d, out = %-3d, cnt=%d\n", people->num, g_slt.out, g_slt.cnt);
#endif
        if (g_slt.cnt == 3) {
#if APP_DEBUG
            g_slt.out++;
            printf("who: num=%-3d, this person is out\n", people->num);
#endif
            list_delete_link(this);
            if (people) {
                free(people);
            }
        }
    }

    count(list);
}

int solution(int n)
{
    people_t *people;

    if (n <= 0 || n >= MAX_PEOPLE) {
        return -1;
    } else {
        memset(&g_slt, 0, sizeof(g_slt));
        list_init(&g_slt.list);
    }

    for (int i = 0; i < n; i++) {
        people = calloc(1, sizeof(people_t));
        people->num = i+1;
        list_insert_tail(&g_slt.list, &people->link);
    }

    count(&g_slt.list);

    lsz_list_t *this = NULL;
    for (this = g_slt.list.next; this != &g_slt.list; this = this->next) {
        people = base_of(this, people_t, link);
        list_delete_link(this);
        if (people) {
            free(people);
        }
    }

    printf("%d\n", g_slt.last_person);
}

#endif  // #ifndef LSZ_LIST

int main(int argc, char const *argv[])
{
    lsz_benchmark_do;

    printf("--------\n");
    solution(1);
    printf("--------\n");
    solution(2);
    printf("--------\n");
    solution(3);
    printf("--------\n");
    solution(997);
    printf("--------\n");
    solution(998);
    printf("--------\n");
    solution(999);
    printf("--------\n");

    lsz_benchmark_done;

/* 性能对比

    结论：递归深度越深，性能差异越显著；链表方式逐层降低了循环次数，优于数组方式

    * 数组方式
    efika@EFIKA-PC ~/f/h/07 (master)> make run
    -----------------------------------------------
    benchmark:
    --------------- --------------- ---------------
    min (us)        max (us)        avr (us)       
    --------------- --------------- ---------------
        118.0000        406.0000        130.0957
    -----------------------------------------------
    efika@EFIKA-PC ~/f/h/07 (master)> make run
    -----------------------------------------------
    benchmark:
    --------------- --------------- ---------------
    min (us)        max (us)        avr (us)       
    --------------- --------------- ---------------
        118.0000        326.0000        138.4521
    -----------------------------------------------
    efika@EFIKA-PC ~/f/h/07 (master)> make run
    -----------------------------------------------
    benchmark:
    --------------- --------------- ---------------
    min (us)        max (us)        avr (us)       
    --------------- --------------- ---------------
        118.0000        362.0000        132.6289
    -----------------------------------------------
    efika@EFIKA-PC ~/f/h/07 (master)> make run

    * 链表方式
    -----------------------------------------------
    benchmark:
    --------------- --------------- ---------------
    min (us)        max (us)        avr (us)       
    --------------- --------------- ---------------
            93.0000        264.0000         96.5732
    -----------------------------------------------
    efika@EFIKA-PC ~/f/h/07 (master)> make run
    -----------------------------------------------
    benchmark:
    --------------- --------------- ---------------
    min (us)        max (us)        avr (us)       
    --------------- --------------- ---------------
            93.0000        220.0000         97.1455
    -----------------------------------------------
    efika@EFIKA-PC ~/f/h/07 (master)> make run
    -----------------------------------------------
    benchmark:
    --------------- --------------- ---------------
    min (us)        max (us)        avr (us)       
    --------------- --------------- ---------------
            82.0000        256.0000         94.7822
    -----------------------------------------------
*/

    return 0;
}
