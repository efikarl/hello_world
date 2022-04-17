/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    双向循环链表
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_LIST
#define LSZ_LIST
#include <stdint.h>
#include <stddef.h>

#define LSZ_MAX_LINK            UINT16_MAX
#define LSZ_LINK_SIG            ('l' << 24 | 'i' << 16 | 's' << 8 | 't')
#define LSZ_LINK_VLD            0

#define LSZ_RET_OK              0
#define LSZ_RET_E_ARG           (-1)
#define LSZ_RET_E_SIG           (-2)
#define LSZ_RET_E_MAX           (-3)

#define base_of(record, type_t, field) \
        ((type_t *) ((char *) (record) - offsetof(type_t, field)))

typedef struct _lsz_list_t  lsz_list_t;

typedef struct _lsz_list_t {
#if LSZ_LINK_VLD
    uint32_t    signature;
    uint32_t    _;
#endif
    lsz_list_t  *prev;
    lsz_list_t  *next;
} lsz_list_t;

//! callback return value, please define positive one
typedef int (* lsz_list_callback_t) (lsz_list_t *link, void *data);

int list_init (
    lsz_list_t          *list
);
int is_list_valid (
    lsz_list_t          *list
);
int is_list_empty (
    lsz_list_t          *list
);
int list_insert_tail (
    lsz_list_t          *list,
    lsz_list_t          *link
);
int list_insert_head (
    lsz_list_t          *list,
    lsz_list_t          *link
);
int list_delete_link (
    lsz_list_t          *link
);
int list_for_each_link (
    lsz_list_t          *list,
    lsz_list_callback_t callback,
    void                *data
);

#endif
