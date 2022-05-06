/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    基础定义
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_BASE
#define LSZ_BASE
#include <stddef.h>

#define LSZ_RET_OK                0
#define LSZ_RET_E_ARG           (-1)
#define LSZ_RET_E_SIG           (-2)
#define LSZ_RET_E_MAX           (-3)
#define LSZ_RET_E_FND           (-4)

#define void_of(ptr, index, _size)      \
            ((char *) ptr + index * _size)

#define base_of(ptr, _type, field)      \
            ((_type *) ((char *) (ptr) - offsetof(_type, field)))

typedef int (* lsz_compare_t) (const void *a, const void *b);

void lsz_swap(void *a, void *b, size_t unit);
int  lsz_icmp(const void *a, const void *b);
int  lsz_scmp(const void *a, const void *b);

#endif
