/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 快速排序
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_SORT
#define LSZ_SORT

typedef int (* lsz_compare_t) (const void *a, const void *b);
int lsz_icmp(const void *a, const void *b);
int lsz_scmp(const void *a, const void *b);

void lsz_qsort(void *data, size_t unit, int n, lsz_compare_t cmpfn);

#endif
