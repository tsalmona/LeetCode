/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define TABLE_SIZE 512
#define BUCKET_BLOCK_SIZE 512

/* 
 * On va créer une hashtable un peu particulière. Elle contient un champ refList qui contient la liste
 * que l'on veut hasher. t->table[i] contient un tableau des indices j tels que t->refList[j] == i %   
 * TABLE_SIZE.
 * Ainsi, si l'on veut savoir si la table contient une valeur p, on vérifie t->refList[i] pour i dans 
 * t->table[p % TABLE_SIZE].
 */

int mod(int a, int b){
    if(b < 0){
        return -1;
    } 
    if(a < 0){
        int v = (-1 * a) % b;
        return b - v;
    }
    return a % b;
}

struct DynList {
    int *list;
    size_t len;
};

struct HashTable {
    int *refList;
    struct DynList **table;
};

struct HashTable *create(int *refList){
    struct HashTable *res = malloc(sizeof(struct HashTable));
    assert(res != NULL);

    res->refList = refList;
    res->table = malloc(TABLE_SIZE * sizeof(struct DynList *));
    assert(res->table != NULL);

    for(size_t i = 0; i < TABLE_SIZE; i++){
        res->table[i] = NULL;
    }

    return res;
}

void print_hashtable(struct HashTable *t){
    for(size_t i = 0; i < TABLE_SIZE; i++){
        printf("%zu : ", i);
        if(t->table[i] == NULL){
            printf("null\n");
        } else {
            for(size_t j = 0; j < t->table[i]->len; j++){
                printf("%d, ", t->table[i]->list[j]);
            }
            printf("\n");
        }
    }
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
    if((l->len > 0) && ((l->len + 1) % BUCKET_BLOCK_SIZE < sizeof(int))){
        int *tmp = realloc(l->list, sizeof(int) * (((l->len + 1) / BUCKET_BLOCK_SIZE) + 1) * BUCKET_BLOCK_SIZE);
        assert(tmp != NULL);

        l->list = tmp;
    }
    l->list[l->len] = e;
    l->list[l->len + 1] = -1;

    (l->len)++;
}

/* Ajoute l'indice e à la table de hashage à la position t->table[t->refList[e] % TABLE_SIZE] */
void appendIndex(struct HashTable *t, int e) {
    int v = t->refList[e];
    size_t bucket_index = mod(v, TABLE_SIZE);

    if (t->table[bucket_index] == NULL) {
        t->table[bucket_index] = malloc(sizeof(struct DynList));
        assert(t->table[bucket_index] != NULL);

        t->table[bucket_index]->len = 0;
        t->table[bucket_index]->list = malloc(BUCKET_BLOCK_SIZE * sizeof(int));
        assert(t->table[bucket_index]->list != NULL);

        t->table[bucket_index]->list[0] = -1;
    }

    append(t->table[bucket_index], e);
}

/* Renvoie l'indice si la table de hashage contient e en valeur,
 * c'est à dire que t->table[e % TABLE_SIZE] contient l'indice de e dans t->refList
 */
int contains(struct HashTable *t, int e){
    if(t == NULL || t->table[mod(e, TABLE_SIZE)] == NULL){
        return -1;
    }
    size_t i = 0;
    int *l = t->table[mod(e, TABLE_SIZE)]->list;
    for(i = 0; i < t->table[mod(e, TABLE_SIZE)]->len; i++){
        if(e == t->refList[l[i]]){
            return l[i];
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

            free_hashtable(t);

            return res;
        }
        appendIndex(t, i);
    }
    *returnSize = 0;

    free_hashtable(t);
    return NULL;
}

int main(){
    int returnSize;

    int t1[5] = {0, 4, 3, 0};
    int *r1 = twoSum(t1, 4, 0, &returnSize);
    printf("Test 1 : %d, %d\n", r1[0], r1[1]);
    free(r1);

    returnSize = 0;

    int t2[1] = {1};
    int *r2 = twoSum(t2, 1, 2, &returnSize);
    if(r2 == NULL){
        printf("Test 2 : ok\n");
    } else {
        printf("Test 2 : failed");
        free(r2);
    }

    return 0;
}