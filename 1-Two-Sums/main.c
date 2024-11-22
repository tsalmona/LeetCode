/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

#include <assert.h>
#include <stdlib.h>

#define TABLE_SIZE 1024
#define BUCKET_BLOCK_SIZE 128

/* 
 * On va créer une hashtable un peu particulière. Elle contient un champ refList qui contient la liste
 * que l'on veut hasher. t->table[i] contient un tableau des indices j tels que t->refList[j] == i %   
 * TABLE_SIZE.
 * Ainsi, si l'on veut savoir si la table contient une valeur p, on vérifie t->refList[i] pour i dans 
 * t->table[p % TABLE_SIZE].
 */

struct DynList {
    int *list;
    size_t len;
};

struct HashTable {
    int *refList;
    struct DynList *table;
};

struct HashTable *create(int *refList){
    struct HashTable *res = malloc(sizeof(struct HashTable));
    assert(res != NULL);

    res->refList = refList;
    res->table = malloc(TABLE_SIZE * sizeof(struct DynList));
    assert(res->table != NULL);

    for(size_t i = 0; i < TABLE_SIZE; i++){
        res->table[i] = NULL;
    }

    return res;
}

void free_dynlist(struct DynList *l){
    free(l->list);
    free(l);
}

void free_hashtable(struct HashTable *t){
    for(size_t i = 0; i < TABLE_SIZE; i++){
        if(t->table[i] != NULL){
            free_dynlist(t->table[i]);
        }
    }
    free(t->table);
    free(t);
}

/* Ajoute dynamiquement l'entier e (qui est en pratique un indice dans un tableau) au tableau t */
void append(struct DynList *l, int e){
    if(l == NULL){
        l = malloc(sizeof(struct DynList));
        assert(l != NULL);

        l->len = 0;
        l->list = malloc(BUCKET_BLOCK_SIZE * sizeof(int));
        assert(l->list != NULL);

        l->list[l->len] = -1;
    }
    if((l->len > 0) && ((l->len + 1) % BLOCK_BUCKET_SIZE < sizeof(int))){
        int *tmp = realloc(l->list, sizeof(int) * (((l->len + 1) / BLOCK_BUCKET_SIZE) + 1) * BLOCK_BUCKET_SIZE;
        assert(tmp != NULL);

        l->list = tmp;
    }
    l->list[l->len] = e;
    l->list[l->len + 1] = -1;

    (l->len)++;
}

/* Ajoute l'indice e à la table de hashage à la position t->table[t->refList[e] % TABLE_SIZE] */
void appendIndex(struct HashTable *t, int e){
    int v = t->refList[e];
    append(t->table[v % TABLE_SIZE], e);
}

/* Renvoie l'indice si la table de hashage contient e en valeur,
 * c'est à dire que t->table[e % TABLE_SIZE] contient l'indice de e dans t->refList
 */
int contains(struct HashTable *t, int e){
    if(t == NULL || t->table[e % TABLE_SIZE] == NULL){
        return 0;
    }
    size_t i = 0;
    int *l = t->table[e % TABLE_SIZE]->list;
    while(l[i] != -1){
        if(e == t->refList[l[i]]){
            return i;
        }
    }
    return -1;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int *res;
    struct HashTable *t = create(nums);

    for(size_t i = 0; i < numsSize; i++){
        int index = contains(t, target - nums[i]);
        if(index != -1){
            *returnSize = 2;
            res = malloc(2 * sizeof(int));
            assert(res != NULL);

            res[0] = i;
            res[1] = index;

            return res;
        }
        appendIndex(t, i);
    }
    *returnSize = 0;
    return NULL;
}