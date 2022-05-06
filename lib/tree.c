/** @file
--------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
--------------------------------------------------------------------------------
    数据 - 红黑二叉树
--------------------------------------------------------------------------------
    性质1：节点非红即黑
    性质2：根节点是黑色
    性质3：叶节点是黑色
    性质4：红色节点的子节点是黑色节点
    性质5：指定节点的子路径的黑高相同
--------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "base.h"
#include "tree.h"

// #define NDEBUG
#ifdef  LIB_DEBUG
#undef  LIB_DEBUG
#define LIB_DEBUG   0
#else
#define LIB_DEBUG   0
#endif

lsz_tree_t __lsz_tree_nil__ = { .colour = LSZ_TREE_COLOUR_BLK };

typedef struct _lsz_tree_private_t {
    lsz_tree_t                 *root;
    lsz_tree_t                 *null;
    size_t                     hsize;
    size_t                     dsize;
    lsz_tree_handle_callback_t  fcmp;
    lsz_tree_insert_callback_t  fins;
    lsz_tree_delete_callback_t  fdel;
    pthread_mutex_t             lock;
} lsz_tree_private_t;

#define lsz_tree_this(tree, field)  (((lsz_tree_private_t *)tree)->field)

/*
--------------------------------------------------------------------------------
*/

//
// 旋转操作：只改变树的指针，不改变树的次序属性
//
/*@fn: _tree_rotate_l
--------------------------------------------------------------------------------
     lp                    lp
      |                     |
      l                     r
     / \                   / \
    ll  r      =左旋=>     l  rr
       / \               /  \
      rl  rr            ll  rl
--------------------------------------------------------------------------------
*/
void _tree_rotate_l(lsz_tree_t **root, lsz_tree_t *l)
{
    lsz_tree_t *r = l->r;

    if (!l || !r) {
        assert(l);
        assert(r);
        return;
    }
    // r的父亲
    r->p = l->p;
    if (l->p == &__lsz_tree_nil__) {
        *root = r;
    } else {
        if (l == l->p->l) {
            l->p->l = r;
        } else {
            l->p->r = r;
        }
    }
    // l的右子
    l->r = r->l;
    if (r->l != &__lsz_tree_nil__) {
        r->l->p = l;
    }
    // r的左子
    r->l = l;
    l->p = r;
}
/*@fn: _tree_rotate_r
--------------------------------------------------------------------------------
     rp                    rp
      |                     |
      l                     r
     / \                   / \
    ll  r      <=右旋=     l  rr
       / \               /  \
      lr  rr            ll  lr
--------------------------------------------------------------------------------
*/
void _tree_rotate_r(lsz_tree_t **root, lsz_tree_t *r)
{
    lsz_tree_t *l = r->l;

    if (!r || !l) {
        assert(r);
        assert(l);
        return;
    }
    // l的父亲
    l->p = r->p;
    if (r->p == &__lsz_tree_nil__) {
        *root = l;
    } else {
        if (r == r->p->r) {
            r->p->r = l;
        } else {
            r->p->l = l;
        }
    }
    // r的左子
    r->l = l->r;
    if (l->r != &__lsz_tree_nil__) {
        l->r->p = r;
    }
    // l的右子
    l->r = r;
    r->p = l;
}
/*@fn: _tree_transplant
--------------------------------------------------------------------------------
    目的：子树替换（以v替换u）
--------------------------------------------------------------------------------
*/
void _tree_transplant(lsz_tree_t **root, lsz_tree_t *u, lsz_tree_t *v)
{
    assert(u);
    assert(v);

    assert(u != &__lsz_tree_nil__);
    if (u->p == &__lsz_tree_nil__) {
        *root = v;
    } else if (u == u->p->l) {
        u->p->l = v;
    } else if (u == u->p->r) {
        u->p->r = v;
    }
    v->p = u->p;
}
/*@fn: _tree_insert_fixup
--------------------------------------------------------------------------------
    目的：不删减节点，保持红黑树性质
--------------------------------------------------------------------------------
*/
void _tree_insert_fixup(lsz_tree_t **root, lsz_tree_t *i)
{
    //
    // 插入操作可能破坏：性质2，性质4
    //
    lsz_tree_t *uofi = NULL;
    //
    // 红父条件下，i祖父必然存在
    //
    while (i->p->colour == LSZ_TREE_COLOUR_RED) {
        //
        // 参考：算法导论
        //
        if (i->p == i->p->p->l) {
            uofi = i->p->p->r;
            if (uofi->colour == LSZ_TREE_COLOUR_RED) {
                i->p->colour    = LSZ_TREE_COLOUR_BLK;
                uofi->colour    = LSZ_TREE_COLOUR_BLK;
                i->p->p->colour = LSZ_TREE_COLOUR_RED;
                i = i->p->p;
            } else {
                if (i == i->p->r) {
                    i = i->p;
                    _tree_rotate_l(root, i);
                }
                i->p->colour    = LSZ_TREE_COLOUR_BLK;
                i->p->p->colour = LSZ_TREE_COLOUR_RED;
                _tree_rotate_r(root, i->p->p);
            }
        } else {
            uofi = i->p->p->l;
            if (uofi->colour == LSZ_TREE_COLOUR_RED) {
                i->p->colour    = LSZ_TREE_COLOUR_BLK;
                uofi->colour    = LSZ_TREE_COLOUR_BLK;
                i->p->p->colour = LSZ_TREE_COLOUR_RED;
                i = i->p->p;
            } else {
                if (i == i->p->l) {
                    i = i->p;
                    _tree_rotate_r(root, i);
                }
                i->p->colour    = LSZ_TREE_COLOUR_BLK;
                i->p->p->colour = LSZ_TREE_COLOUR_RED;
                _tree_rotate_l(root, i->p->p);
            }
        }
    }
    (*root)->colour = LSZ_TREE_COLOUR_BLK;
}
/*@fn: _tree_delete_fixup
--------------------------------------------------------------------------------
    目的：不删减节点，保持红黑树性质
--------------------------------------------------------------------------------
*/
void _tree_delete_fixup(lsz_tree_t **root, lsz_tree_t *i)
{
    //
    // 删除操作可能破坏：性质1，性质2，性质4
    //
    lsz_tree_t *bofi = NULL;
    //
    // 双黑非根时，i兄弟必然存在
    //
    while ((i->p != &__lsz_tree_nil__) && (i->colour == LSZ_TREE_COLOUR_BLK)) {
        //
        // 参考：算法导论
        //
        if (i == i->p->l) {
            bofi = i->p->r;
            if (bofi->colour == LSZ_TREE_COLOUR_RED) {
                bofi->colour = LSZ_TREE_COLOUR_BLK;
                i->p->colour = LSZ_TREE_COLOUR_RED;
                _tree_rotate_l(root, i->p); bofi = i->p->r;
            }
            if (((bofi->l->colour == LSZ_TREE_COLOUR_BLK)) &&
                ((bofi->r->colour == LSZ_TREE_COLOUR_BLK))) {
                bofi->colour = LSZ_TREE_COLOUR_RED;
                i = i->p;
            } else {
                if (((bofi->l->colour == LSZ_TREE_COLOUR_RED)) &&
                    ((bofi->r->colour == LSZ_TREE_COLOUR_BLK))) {
                    bofi->l->colour = LSZ_TREE_COLOUR_BLK;
                    bofi->colour    = LSZ_TREE_COLOUR_RED;
                    _tree_rotate_r(root, bofi); bofi = i->p->r;
                }
                if (bofi->r->colour == LSZ_TREE_COLOUR_RED) {
                    bofi->colour    = i->p->colour;
                    i->p->colour    = LSZ_TREE_COLOUR_BLK;
                    bofi->r->colour = LSZ_TREE_COLOUR_BLK;
                    _tree_rotate_l(root, i->p);
                    // 双黑解除，终止循环
                    i = *root;
                }
            }
        } else {
            bofi = i->p->l;
            if (bofi->colour == LSZ_TREE_COLOUR_RED) {
                bofi->colour = LSZ_TREE_COLOUR_BLK;
                i->p->colour = LSZ_TREE_COLOUR_RED;
                _tree_rotate_r(root, i->p); bofi = i->p->l;
            }
            if (((bofi->r->colour == LSZ_TREE_COLOUR_BLK)) &&
                ((bofi->l->colour == LSZ_TREE_COLOUR_BLK))) {
                bofi->colour = LSZ_TREE_COLOUR_RED;
                i = i->p;
            } else {
                if (((bofi->r->colour == LSZ_TREE_COLOUR_RED)) &&
                    ((bofi->l->colour == LSZ_TREE_COLOUR_BLK))) {
                    bofi->r->colour = LSZ_TREE_COLOUR_BLK;
                    bofi->colour    = LSZ_TREE_COLOUR_RED;
                    _tree_rotate_l(root, bofi); bofi = i->p->l;
                }
                if (bofi->l->colour == LSZ_TREE_COLOUR_RED) {
                    bofi->colour    = i->p->colour;
                    i->p->colour    = LSZ_TREE_COLOUR_BLK;
                    bofi->l->colour = LSZ_TREE_COLOUR_BLK;
                    _tree_rotate_r(root, i->p);
                    // 双黑解除，终止循环
                    i = *root;
                }
            }
        }
    }
    //
    // 情况1到情况2的补偿
    //
    if (i->colour == LSZ_TREE_COLOUR_RED) {
        i->colour =  LSZ_TREE_COLOUR_BLK;
    }
}

/*
--------------------------------------------------------------------------------
*/

lsz_tree_t *_tree_new(const void *tree, const void *handle, const void *data)
{
    assert(tree); assert(handle);

    lsz_tree_t *node = calloc(1, sizeof(lsz_tree_t));
    if (!node) {
        assert(node);
        return node;
    }

    node->signature = LSZ_TREE_SIG;

    if (!lsz_tree_this(tree, hsize)) {
        node->handle = strdup(handle);
    } else {
        node->handle = malloc(lsz_tree_this(tree, hsize)); if (node->handle) memcpy(node->handle, handle, lsz_tree_this(tree, hsize));
    }
    assert(node->handle);

    if (data) {
        if (!lsz_tree_this(tree, dsize)) {
            node->data = strdup(data);
        } else {
            node->data = malloc(lsz_tree_this(tree, dsize)); if (node->data) memcpy(node->data, data, lsz_tree_this(tree, dsize));
        }
        assert(node->data);
    }

    node->colour = LSZ_TREE_COLOUR_BLK;
    node->p = &__lsz_tree_nil__;
    node->l = &__lsz_tree_nil__;
    node->r = &__lsz_tree_nil__;

    return node;
}

lsz_tree_t *_tree_min(lsz_tree_t *node)
{
    assert(node);

    lsz_tree_t *find = node;
    while (node != &__lsz_tree_nil__) {
        find = node;
        node = node->l;
    }

    return find;
}

lsz_tree_t *_tree_max(lsz_tree_t *node)
{
    assert(node);

    lsz_tree_t *find = node;
    while (node != &__lsz_tree_nil__) {
        find = node;
        node = node->r;
    }
    return find;
}

lsz_tree_t *_tree_search(const void *tree, const void *handle)
{
    assert(tree); assert(handle);

    int i = 0;
    lsz_tree_t *node = lsz_tree_this(tree, root);
    while ((node != lsz_tree_this(tree, null)) && (i = lsz_tree_this(tree, fcmp)(handle, node->handle))) {
        if (i < 0) {
            node = node->l;
        } else {
            node = node->r;
        }
    }
    return node;
}

lsz_tree_t *_tree_prev(const void *tree, lsz_tree_t *node)
{
    assert(tree); assert(node);

    if (node == &__lsz_tree_nil__) {
        return node;
    }
    if (node->l != &__lsz_tree_nil__) {
        node = _tree_max(node->l);
    } else {
        while ((node->p != &__lsz_tree_nil__) && (node == node->p->l)) {
            node = node->p;
        }
        node = node->p;
    }

    return node;
}

lsz_tree_t *_tree_next(const void *tree, lsz_tree_t *node)
{
    assert(tree); assert(node);

    if (node == &__lsz_tree_nil__) {
        return node;
    }
    if (node->r != &__lsz_tree_nil__) {
        node = _tree_min(node->r);
    } else {
        while ((node->p != &__lsz_tree_nil__) && (node == node->p->r)) {
            node = node->p;
        }
        node = node->p;
    }

    return node;
}

void _tree_for_each(lsz_tree_t *node, lsz_tree_for_each_callback_t callback, void *data)
{
    assert(node);

    if (node != &__lsz_tree_nil__) {
        _tree_for_each(node->l, callback, data);
        if (callback) callback(node, data);
        _tree_for_each(node->r, callback, data);
    }
}

void _tree_reset_nil(void *tree)
{
    assert(lsz_tree_this(tree, null) ==  &__lsz_tree_nil__);
    memset(lsz_tree_this(tree, null), 0, sizeof(lsz_tree_t));
    lsz_tree_this(tree, null)->colour = LSZ_TREE_COLOUR_BLK;
}

/*
--------------------------------------------------------------------------------
*/

void *tree_new_ex(size_t hsize, size_t dsize, lsz_tree_handle_callback_t fcmp, lsz_tree_insert_callback_t fins, lsz_tree_delete_callback_t fdel)
{
    lsz_tree_private_t *this = calloc(1, sizeof(lsz_tree_private_t));
    if (!this) {
        assert(this);
        return this;
    }
    this->root  = &__lsz_tree_nil__;
    this->null  = &__lsz_tree_nil__;
    this->hsize = hsize;
    this->dsize = dsize;
    this->fcmp  = fcmp;
    this->fins  = fins;
    this->fdel  = fdel;
    pthread_mutex_init(&this->lock, NULL);

    return (void *) this;
}

void *tree_new(size_t hsize, size_t dsize, lsz_tree_handle_callback_t fcmp)
{
    assert(fcmp);
    return tree_new_ex(hsize, dsize, fcmp, NULL, NULL);
}

int is_tree_empty(const void *tree)
{
    assert(tree);

    if (lsz_tree_this(tree, root) == lsz_tree_this(tree, null)) {
        return 1;
    } else {
        return 0;
    }
}

void tree_insert(void *tree, const void *handle, const void *data)
{
    assert(tree);

    lsz_tree_t *node = NULL;
    //
    // 如果节点存在，返回节点（如果回调存在，由使用者自己解决冲突）
    //
    node = _tree_search(tree, handle);
    if (node != lsz_tree_this(tree, null)) {
        if (lsz_tree_this(tree, fins)) {
            lsz_tree_this(tree, fins)(tree, node->handle, handle);
        } else {
            return;
        }
    }

    pthread_mutex_lock(&lsz_tree_this(tree, lock));

    node = _tree_new(tree, handle, data);
    if (!node) {
        assert(node);
        return;
    }
    lsz_tree_t *n = lsz_tree_this(tree, root);
    lsz_tree_t *p = lsz_tree_this(tree, null);
    // 寻找其父
    while (n != lsz_tree_this(tree, null)) {
        p = n;
        if (lsz_tree_this(tree, fcmp)(node->handle, n->handle) < 0) {
            n = n->l;
        } else {
            n = n->r;
        }
    }
    node->p = p;
    // 插入节点
    if (p == lsz_tree_this(tree, null)) {
        lsz_tree_this(tree, root) = node;
    } else {
        if (lsz_tree_this(tree, fcmp)(node->handle, p->handle) < 0) {
            p->l = node;
        } else {
            p->r = node;
        }
    }
    node->colour = LSZ_TREE_COLOUR_RED;
    // 红黑平衡
    _tree_insert_fixup(&lsz_tree_this(tree, root), node);

    pthread_mutex_unlock(&lsz_tree_this(tree, lock));
}

void tree_delete(void *tree, const void *handle)
{
    assert(tree);

    lsz_tree_t *node = NULL;
    lsz_tree_t *succ = NULL;
    //
    // 如果没此节点，直接返回
    //
    node = _tree_search(tree, handle);
    if (node == lsz_tree_this(tree, null)) {
        return;
    }

    pthread_mutex_lock(&lsz_tree_this(tree, lock));

    lsz_tree_t *node_to_fixup = NULL;
    uint8_t     delete_colour = node->colour;

    if ((node->l == lsz_tree_this(tree, null)) && (node->r == lsz_tree_this(tree, null))) {
        //
        // [1] 叶子节点
        //
        node_to_fixup = lsz_tree_this(tree, null);
        _tree_transplant(&lsz_tree_this(tree, root), node, lsz_tree_this(tree, null));
    } else if ((node->l != lsz_tree_this(tree, null)) && (node->r == lsz_tree_this(tree, null))) {
        //
        // [2] 存在左子（必然红色）：直接移植
        //
        node->l->colour = node->colour;
        _tree_transplant(&lsz_tree_this(tree, root), node, node->l);
    } else if ((node->l == lsz_tree_this(tree, null)) && (node->r != lsz_tree_this(tree, null))) {
        //
        // [3] 存在右子（必然红色）：直接移植
        //
        node->r->colour = node->colour;
        _tree_transplant(&lsz_tree_this(tree, root), node, node->r);
    } else {
        //
        // [4] 存在双子：使用后继置换所删（+颜色），那么等效于删除了后继
        //
        // 寻找节点后继（节点右子最小者即为后继）| 存在右子即存在后继节点
        succ = _tree_min(node->r);
        node_to_fixup = succ->r;
        delete_colour = succ->colour;
        //
        // 移植后继右子到后继
        //
        if (node == succ->p) {
            succ->r->p = succ;
        } else {
            _tree_transplant(&lsz_tree_this(tree, root), succ, succ->r);
            succ->r = node->r;
            succ->r->p = succ;
        }
        //
        // 移植后继到所删节点
        //
        succ->colour = node->colour;
        _tree_transplant(&lsz_tree_this(tree, root), node, succ);
        succ->l = node->l;
        succ->l->p = succ;
    }

    if (node_to_fixup && (delete_colour == LSZ_TREE_COLOUR_BLK)) {
        // 红黑平衡
        _tree_delete_fixup(&lsz_tree_this(tree, root), node_to_fixup);
        _tree_reset_nil(tree);
    }
    //
    // 释放资源
    //
    free(node->handle); free(node->data);
    free(node);
    //
    // 如果删除后节点依然存在
    //
    if (lsz_tree_this(tree, fins) && lsz_tree_this(tree, fdel)) {
        node = _tree_search(tree, handle);
        if (node == lsz_tree_this(tree, null)) {
            lsz_tree_this(tree, fdel)(tree, node);
            return;
        }
    }

    pthread_mutex_unlock(&lsz_tree_this(tree, lock));
}

void *tree_min(const void *tree)
{
    assert(tree);

    lsz_tree_t *node = _tree_min(lsz_tree_this(tree, root));
    if (node == lsz_tree_this(tree, null)) {
        return NULL;
    } else {
        return node->data;
    }
}

void *tree_max(const void *tree)
{
    assert(tree);

    lsz_tree_t *node = _tree_max(lsz_tree_this(tree, root));
    if (node == lsz_tree_this(tree, null)) {
        return NULL;
    } else {
        return node->data;
    }
}

void *tree_search(const void *tree, const void *handle)
{
    assert(tree); assert(handle);

    lsz_tree_t *node = _tree_search(tree, handle);
    if (node == lsz_tree_this(tree, null)) {
        return NULL;
    } else {
        return node->data;
    }
}

void *tree_prev(const void *tree, void **handle)
{
    assert(tree); assert(handle);

    lsz_tree_t *node;
    if (!*handle) {
        node = _tree_max(lsz_tree_this(tree, root));
    } else {
        node = _tree_prev(tree, _tree_search(tree, *handle));
    }
    *handle = node->handle;

    return node->data;
}

void *tree_next(const void *tree, void **handle)
{
    assert(tree); assert(handle);

    lsz_tree_t *node;
    if (!*handle) {
        node = _tree_min(lsz_tree_this(tree, root));
    } else {
        node = _tree_next(tree, _tree_search(tree, *handle));
    }
    *handle = node->handle;

    return node->data;
}

void tree_for_each(const void *tree, lsz_tree_for_each_callback_t callback, void *data)
{
    assert(tree);
    _tree_for_each(lsz_tree_this(tree, root), callback, data);
}

/*
--------------------------------------------------------------------------------
*/

#if LIB_DEBUG

int is_tree_balance(lsz_tree_t *node)
{
    assert(node);

    if (node == &__lsz_tree_nil__) {
        return 1;
    }
    //
    // 对每个节点，只检最左最右黑高
    //
    lsz_tree_t *next = NULL;
    int lbh = 0;
    int rbh = 0;

    next = node;
    while(next != &__lsz_tree_nil__) {
        next = next->l;
        if (next && next->colour == LSZ_TREE_COLOUR_BLK) {
            lbh++;
        }
    }
    lbh++;
    next = node;
    while(next != &__lsz_tree_nil__) {
        next = next->r;
        if (next->colour == LSZ_TREE_COLOUR_BLK) {
            rbh++;
        }
    }
    rbh++;
    if (lbh != rbh) {
        printf("%02d(%02d): lbh:rbh = %02d:%02d\n", *(int *) node->handle, *(int *) node->data, lbh, rbh);
    }

    return lbh == rbh;
}

int is_tree_redsblk(lsz_tree_t *node)
{
    assert(node);
    //
    // 对每个红色节点，双子必须黑色
    //
    if (node == &__lsz_tree_nil__) {
        return 1;
    }
    if (node->colour == LSZ_TREE_COLOUR_BLK) {
        return 1;
    }
    if (node->colour == LSZ_TREE_COLOUR_RED) {
        if (node->l->colour != LSZ_TREE_COLOUR_BLK) {
            return 0;
        }
        if (node->r->colour != LSZ_TREE_COLOUR_BLK) {
            return 0;
        }
        return 1;
    }
    return 0;
}

static int is_balance = 1;
static int is_redsblk = 1;
#define MAX_TEST_HANDLE 32

int icompare(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) - (*(int *)a < *(int *)b);
}

void callback(lsz_tree_t *node, void *data)
{
    int handle = *(int *) node->handle;

    if (!is_tree_balance(node)) {
        is_balance = 0;
        printf("handle: %02d, is_tree_balance: fail\n", handle);
    }
    if (!is_tree_redsblk(node)) {
        printf("handle: %02d, is_tree_redsblk: fail\n", handle);
        is_redsblk = 0;
    }
}

int main(int argc, char const *argv[])
{
    void *tree = tree_new(sizeof(int), sizeof(int), icompare);

    srand(time(0));

    int hint = 0;
    int dint = 0;
    int dget = 0;
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机插入
            dint = i;
            do {
                hint = rand()%MAX_TEST_HANDLE;
            } while (tree_search(tree, &hint));
            tree_insert(tree, &hint, &dint);
            printf("tree_insert: %02d(%02d)\n", hint, dget = *(int *) tree_search(tree, &hint));
            if (dget != dint) {
                printf("tree_search: %02d(%02d)\n", hint, dget);
            }
        }
        printf("+++++++++++++++++++++++++++++++++\n");
        printf("result:\n");
        tree_for_each(tree, callback, NULL);
        printf("---------------------------------\n");
        printf("  is_balance: %s\n", is_balance?"pass":"fail");
        printf("  is_redsblk: %s\n", is_redsblk?"pass":"fail");
    }
    {
        printf("+++++++++++++++++++++++++++++++++\n");
        int *min   = NULL;
        int *max   = NULL;
        int *hptr  = NULL;
        int *dptr  = NULL;
        printf("tree_min: %02d\n", *(min = tree_min(tree)));
        printf("tree_max: %02d\n", *(max = tree_max(tree)));
        hptr = NULL;
        while((dptr = tree_next(tree, (void **) &hptr))) {
            printf("tree_next: %02d(%02d)\n", *hptr, *dptr);
        }
        hptr = NULL;
        while((dptr = tree_prev(tree, (void **) &hptr))) {
            printf("tree_prev: %02d(%02d)\n", *hptr, *dptr);
        }
        printf("+++++++++++++++++++++++++++++++++\n");
    }
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机删除
            do {
                hint = rand()%MAX_TEST_HANDLE;
            } while (!tree_search(tree, &hint));
            printf("tree_delete: %02d(%02d)\n", hint, dint = *(int *) tree_search(tree, &hint));
            tree_delete(tree, &hint);
            if (tree_search(tree, &hint)) {
                printf("tree_search: %02d(%02d)\n", hint, dint);
            }
        }
        printf("+++++++++++++++++++++++++++++++++\n");
        printf("result:\n");
        tree_for_each(tree, callback, NULL);
        printf("---------------------------------\n");
        printf("  is_balance: %s\n", is_balance?"pass":"fail");
        printf("  is_redsblk: %s\n", is_redsblk?"pass":"fail");
        printf("+++++++++++++++++++++++++++++++++\n");
    }

    return 0;
}

#endif
