#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum {
    HASH_MAP_TABLE_SIZE = 2,
    HASH_MAP_KEY_SIZE = 1024,
};

typedef struct _HashMapNode {
    char key[HASH_MAP_KEY_SIZE];
    void *data;
    struct _HashMapNode *next;
} HashMapNode;

typedef void (*HashMapDeleter)(void *);

void
HashMapNode_Del(
    HashMapNode *self,
    HashMapDeleter deleter
) {
    if (!self) {
        return;
    }

    for (HashMapNode *cur = self; cur; ) {
        HashMapNode *del = cur;
        cur = cur->next;
        deleter(del->data);
        free(del);
    }
}

HashMapNode *
HashMapNode_New(
    const char *key,
    void *data,  // move
    HashMapNode *next
) {
    HashMapNode *self = calloc(1, sizeof(*self));
    if (!self) {
        return NULL;
    }

    snprintf(self->key, sizeof self->key, "%s", key);
    self->data = data;
    self->next = next;

    return self;
}

typedef struct {
    HashMapNode *table[HASH_MAP_TABLE_SIZE];
    HashMapDeleter deleter;    
} HashMap;

void
HashMap_Destroy(HashMap *self) {
    for (int32_t i = 0; i < HASH_MAP_TABLE_SIZE; i += 1) {
        HashMapNode *node = self->table[i];
        if (!node) {
            continue;
        }
        HashMapNode_Del(node, self->deleter);
    }
}

void
HashMap_Init(HashMap *self, HashMapDeleter deleter) {
    *self = (HashMap) {0};    
    self->deleter = deleter;
}

int32_t
create_hash(const char *str) {
    int32_t hash = 0;
    int32_t weight = 1;

    for (const char *p = str; *p; p += 1) {
        hash += weight * (*p);
        weight += 1;
    }

    return hash;
}

HashMap *
HashMap_Set(
    HashMap *self,
    const char *key,
    void *data
) {
    int32_t hash = create_hash(key);
    hash %= HASH_MAP_TABLE_SIZE;

    HashMapNode *node = self->table[hash];
    if (!node) {
        node = HashMapNode_New(key, data, NULL);
        self->table[hash] = node;
        return self;
    }

    for (HashMapNode *cur = node; cur; cur = cur->next) {
        if (!strcmp(cur->key, key)) {
            // 同じキーのノード
            self->deleter(cur->data);
            cur->data = data;
            break;
        } else if (!cur->next) {
            // 末尾のノード
            cur->next = HashMapNode_New(key, data, NULL);
            if (!cur->next) {
                return NULL;
            }
            break;
        }
    }

    return self;
}

void *
HashMap_Get(const HashMap *self, const char *key) {
    int32_t hash = create_hash(key);
    hash %= HASH_MAP_TABLE_SIZE;

    HashMapNode *node = self->table[hash];
    if (!node) {
        return NULL;
    }

    for (HashMapNode *cur = node; cur; cur = cur->next) {
        if (!strcmp(cur->key, key)) {
            return cur->data;
        }
    }

    return NULL;
}

void
int_deleter(void *data) {
    free(data);
}

void
HashMap_Dump(HashMap *self) {
    for (int32_t i = 0; i < HASH_MAP_TABLE_SIZE; i += 1) {
        HashMapNode *node = self->table[i];
        if (!node) {
            continue;
        }

        int32_t j = 0;
        for (HashMapNode *cur = node; cur; cur = cur->next) {
            for (int32_t k = 0; k < j; k += 1) {
                printf("  ");
            }
            printf("%d: key[%s] data[%d]\n",
                i, cur->key, *(int *)cur->data);
            
            j += 1;
        }
    }
}

void
test(void) {
    HashMap hashmap;
    HashMap_Init(&hashmap, int_deleter);

    int *data;
    data = malloc(sizeof(int));
    *data = 1;
    HashMap_Set(&hashmap, "hige", data);

    data = malloc(sizeof(int));
    *data = 2;
    HashMap_Set(&hashmap, "hoge", data);

    data = malloc(sizeof(int));
    *data = 3;
    HashMap_Set(&hashmap, "hoge", data);

    data = malloc(sizeof(int));
    *data = 4;
    HashMap_Set(&hashmap, "a", data);

    data = malloc(sizeof(int));
    *data = 5;
    HashMap_Set(&hashmap, "b", data);

    data = malloc(sizeof(int));
    *data = 5;
    HashMap_Set(&hashmap, "d", data);

    data = malloc(sizeof(int));
    *data = 6;
    HashMap_Set(&hashmap, "c", data);

    HashMap_Dump(&hashmap);

    data = HashMap_Get(&hashmap, "hige");
    assert(*data == 1);

    data = HashMap_Get(&hashmap, "hoge");
    assert(*data == 3);

    data = HashMap_Get(&hashmap, "fuge");
    assert(data == NULL);

    HashMap_Destroy(&hashmap);
}

int main(void) {
    test();
    return 0;
}
