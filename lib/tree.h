/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    数据 - 红黑二叉树
---------------------------------------------------------------------------------
    性质1：节点非红即黑
    性质2：根节点是黑色
    性质3：叶节点是黑色
    性质4：红色节点的子节点是黑色节点
    性质5：指定节点的子路径的黑高相同
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_TREE
#define LSZ_TREE
#include <stdint.h>
#include <stddef.h>

typedef struct _lsz_tree_t  lsz_tree_t;

#define LSZ_TREE_SIG            ('t' << 24 | 'r' << 16 | 'e' << 8 | 'e')
#define LSZ_TREE_COLOUR_RED     0
#define LSZ_TREE_COLOUR_BLK     1

#pragma pack(1)
typedef struct _lsz_tree_t {
    uint32_t    signature;
    uint8_t     colour;
    uint8_t     _3;
    uint8_t     _2;
    uint8_t     _1;
    lsz_tree_t  *p;
    lsz_tree_t  *l;
    lsz_tree_t  *r;
    void        *handle;
    void        *data;
} lsz_tree_t;
#pragma pack(0)

//
//! callback(!lsz_compare_t): 0: 成功，!0: 失败
//
//
// 数据比较的回调函数：处理插入或查找时元素大小计算
//
typedef lsz_compare_t lsz_tree_handle_callback_t;
//
// 数据插入的回调函数：处理插入时如果元素存在的情况
//
typedef 
lsz_tree_t *(* lsz_tree_insert_callback_t) (
    void                *tree,
    void                *old_handle,
    const void          *new_handle
);
//
// 数据删除的回调函数：处理删除后元素依然存在的情况
//
typedef 
void (* lsz_tree_delete_callback_t) (
    void                *tree,
    const void          *next_handle
);
//
// 数据遍历的回调函数
//
typedef
void (* lsz_tree_for_each_callback_t) (
    lsz_tree_t          *node,
    void                *data
);

/*@fn: tree_new: 建立一棵树
--------------------------------------------------------------------------------
[I]:
    hsize               : 句柄单位大小
                             0: 代表字串
                            !0: 代表字串以外的其它数据类型
    dsize               : 数据单位大小
                             0: 代表字串
                            !0: 代表字串以外的其它数据类型
    data_compare_fn     : 句柄的比较函数

[O]:
    void *              : 此树（指针需要使用者释放）
--------------------------------------------------------------------------------
*/
void *tree_new (
    size_t                              hsize,
    size_t                              dsize,
    lsz_tree_handle_callback_t          handle_compare_fn
);
/*@fn: is_tree_empty
--------------------------------------------------------------------------------
[O]:
    int                 : 0 - 代表空树    1 - 代表非空
--------------------------------------------------------------------------------
*/
int is_tree_empty (
    const void          *tree
);
/*@fn: tree_insert：在树中插入一个节点
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
    handle              : 句柄
    data                : 数据
--------------------------------------------------------------------------------
*/
void tree_insert (
    void                *tree,
    const void          *handle,
    const void          *data
);
/*@fn: tree_insert：在树中删除一个节点
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
    handle              : 句柄
--------------------------------------------------------------------------------
*/
void tree_delete (
    void                *tree,
    const void          *handle
);
/*@fn: tree_min：获取最小句柄关联的数据
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
[O]:
    void *              : 最小句柄的数据
--------------------------------------------------------------------------------
*/
void *tree_min (
    const void          *tree
);
/*@fn: tree_min：获取最大句柄关联的数据
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
[O]:
    void *              : 最小句柄的数据
--------------------------------------------------------------------------------
*/
void *tree_max (
    const void          *tree
);
/*@fn: tree_search：查找指定句柄关联的数据
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
    handle              : 句柄
[O]:
    void *              : 最小句柄的数据
--------------------------------------------------------------------------------
*/
void *tree_search (
    const void          *tree,
    const void          *handle
);
/*@fn: tree_prev：获取当前的前驱关联的数据
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
    handle              : 当前句柄（NULL: 代表最小）
[O]:
    handle              : 前驱句柄
    void *              : 最小句柄的数据
--------------------------------------------------------------------------------
*/
void *tree_prev (
    const void          *tree,
    void               **handle
);
/*@fn: tree_prev：获取当前的后继关联的数据
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
    handle              : 当前句柄（NULL: 代表最小）
[O]:
    handle              : 前驱句柄
    void *              : 最小句柄的数据
--------------------------------------------------------------------------------
*/
void *tree_next (
    const void          *tree,
    void               **handle
);
/*@fn: tree_prev：获取当前的后继关联的数据
--------------------------------------------------------------------------------
[I]:
    tree                : 此树
    callback_func       : 回调函数
    callback_data       : 回调数据
[O]:
    handle              : 前驱句柄
    void *              : 最小句柄的数据
--------------------------------------------------------------------------------
*/
void tree_for_each (
    const void                          *tree,
    lsz_tree_for_each_callback_t         callback_func,
    void                                *callback_data
);

#endif
