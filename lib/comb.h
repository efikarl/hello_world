/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    组合
---------------------------------------------------------------------------------
    [i] 待组数组
    [o] 部分组合
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_COMB
#define LSZ_COMB

typedef int (*comb_callback_t)(int *sub, int m);

void init(int *set, int n, int *sub, int m, comb_callback_t fn);
void comb(int next);

#endif
