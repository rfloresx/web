#include "wshtp_avl.h"
#include <stdlib.h>

typedef struct avlnode_s avlnode_t;

struct avlnode_s {
     int64_t key;
     void *value;

     avlnode_t *left;
     avlnode_t *right;

     int height;
};

struct avlmap_s {
    avlnode_t *root;
};

/* *******************************************
 * Private Methods:
 * ********************************************/

//avl util
static avlnode_t* avlnode_new(void);
static void avlnode_rfree(avlnode_t *node);
static int _avlnode_height(avlnode_t *node);
static void _avlmap_update_height(avlnode_t *node);

static avlnode_t* _avlmap_remove_min(avlnode_t **ptr);

//avl rotations
static void _avlmap_lrotation(avlnode_t **ptr);
static void _avlmap_rrotation(avlnode_t **ptr);
static void _avlmap_rotate(avlnode_t **ptr);
//avl internal api
static void _avlmap_insert(avlnode_t **ptr, int64_t key, void *value);
static void* _avlmap_remove(avlnode_t **ptr, int64_t key);
static void* _avlmap_find(avlnode_t **ptr, int64_t key);


/* ***********
 * Public Methods:
 * ***********/

avlmap_t* avlmap_new() {
    avlmap_t *map = malloc(sizeof(avlmap_t));
    if (map == NULL) {
        return NULL;
    }
    map->root = NULL;
    return map;
}

void avlmap_free(avlmap_t *map) {
    if (map) {
        avlnode_rfree(map->root);
        free(map);
    }
}

void avlmap_insert(avlmap_t *map, int64_t key, void *value) {
    _avlmap_insert(&map->root, key, value);
}

void* avlmap_remove(avlmap_t *map, int64_t key) {
    return _avlmap_remove(&map->root, key);
}

void* avlmap_find(avlmap_t *map, int64_t key) {
    return _avlmap_find(&map->root, key);
}

/* ***********
 * Private methods
 * ***********/

static avlnode_t* avlnode_new() {
    avlnode_t *node = malloc(sizeof(avlnode_t));
    if (node == NULL) {
        return NULL;
    }
    node->key = 0;
    node->value = NULL;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return node;
}

static void avlnode_rfree(avlnode_t *node){
    if (node != NULL) {
        avlnode_rfree(node->left);
        avlnode_rfree(node->right);
        free(node);
    }
}

static int _avlnode_height(avlnode_t *node) {
    if (node == NULL) {
        return -1;
    }
    return node->height;
}

static void _avlmap_update_height(avlnode_t *node) {
    int hleft = _avlnode_height(node->left);
    int hright = _avlnode_height(node->right);
    if (hleft > hright) {
        node->height = hleft + 1;
    } else {
        node->height = hright + 1;
    }
}

static avlnode_t* _avlmap_remove_min(avlnode_t **ptr) {
    avlnode_t *node = *ptr;
    if (node == NULL) {
        return NULL;
    }
    if (node->left == NULL) {
        *ptr = node->right;
        node->right = NULL;
        return node;
    }
    avlnode_t *ret = _avlmap_remove_min(&node->left);
    _avlmap_update_height(node);
    _avlmap_rotate(ptr);
    return ret;
}

//rotations
static void _avlmap_lrotation(avlnode_t **ptr) {
    avlnode_t *node = *ptr;
    if (node == NULL || node->right == NULL) {
        return;
    }
    avlnode_t *a = node;
    avlnode_t *b = node->right;
    a->right = b->left;
    b->left = a;
    *ptr = b;

    a->height = (a->height - 2);
    if (a->height < 0) {
        a->height = 0;
    }
}

static void _avlmap_rrotation(avlnode_t **ptr) {
    avlnode_t *node = *ptr;
    if (node == NULL || node->left == NULL) {
        return;
    }
    avlnode_t *a = node;
    avlnode_t *b = node->left;
    a->left = b->right;
    b->right = a;
    *ptr = b;
    a->height = (a->height - 2);
    if (a->height < 0) {
        a->height = 0;
    }
}

static void _avlmap_rotate(avlnode_t **ptr) {
    if (*ptr == NULL) {
        return;
    }
    avlnode_t *node = *ptr;

    int hleft = _avlnode_height(node->left);
    int hright = _avlnode_height(node->right);

    if (hleft > (hright + 1)) {
        avlnode_t *left = node->left;
        hleft = _avlnode_height(left->left);
        hright = _avlnode_height(left->right);
        if (hright > (hleft + 1)) {
            _avlmap_lrotation(&node->left);
        }
        _avlmap_rrotation(ptr);
    } else if (hright > (hleft + 1)) {
        avlnode_t *right = node->right;
        hleft = _avlnode_height(right->left);
        hright = _avlnode_height(right->right);
        if (hright > (hleft + 1)) {
            _avlmap_rrotation(&node->right);
        }
        _avlmap_lrotation(ptr);
    }
}


//internal api
static void _avlmap_insert(avlnode_t **ptr, int64_t key, void *value) {
    avlnode_t *node = *ptr;
    if (node == NULL) {
        node = avlnode_new();
        node->key = key;
        node->value = value;
        *ptr = node;
    } else {
        if (key < node->key){
            _avlmap_insert(&node->left, key, value);
        }else {
            _avlmap_insert(&node->right, key, value);
        }
        _avlmap_update_height(node);
        _avlmap_rotate(ptr);
    }
}

static void* _avlmap_remove(avlnode_t **ptr, int64_t key) {
    void *data = NULL;
    if (*ptr == NULL) {
        return NULL;
    }
    avlnode_t *root = *ptr;
    if (key == root->key) {
        data = root->value;
        if (root->left == NULL || root->right == NULL) {
            avlnode_t *temp = NULL;
            if (root->left != NULL) {
                temp = root->left;
            } else {
                temp = root->right;
            }
            if (temp == NULL) {
                temp = *ptr;
                *ptr = NULL;
            } else {
                **ptr = *temp;
            }
            free(temp);
            return data;
        } else {
            avlnode_t * temp = _avlmap_remove_min(&root->right);
            root->key = temp->key;
            root->value = temp->value;
            free(temp);
        }
    } else if (key < root->key ) {
        data = _avlmap_remove(&root->left, key);
    } else {
        data = _avlmap_remove(&root->right, key);
    }
    _avlmap_update_height(root);
    _avlmap_rotate(ptr);
    return data;
}

static void* _avlmap_find(avlnode_t **ptr, int64_t key) {
    avlnode_t *node = *ptr;
    if (node == NULL) {
        return NULL;
    }
    if (node->key == key) {
        return node->value;
    }
    if (key < node->key) {
        return _avlmap_find(&node->left, key);
    }
    return _avlmap_find(&node->right, key);
}
