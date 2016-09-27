#ifndef __WSHTP_AVL_H__
#define __WSHTP_AVL_H__
#include <ctype.h>
#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* *******************************************
* Public avlmap API:
* ********************************************/
typedef struct avlmap_s avlmap_t;

avlmap_t *avlmap_new(void);
void avlmap_free(avlmap_t *map);
void avlmap_insert(avlmap_t *map, int64_t key, void *value);
void* avlmap_remove(avlmap_t *map, int64_t key);
void* avlmap_find(avlmap_t *map, int64_t key);

#ifdef __cplusplus
}
#endif

#endif
