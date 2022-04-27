/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    排列
---------------------------------------------------------------------------------
    [i] 待排数组
    [o] 完全排列
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_PERM
#define LSZ_PERM

typedef int (*perm_callback_t)(int *sub, int m);

void init(int *vec, int n, perm_callback_t fn);
void perm(int next);

#endif
