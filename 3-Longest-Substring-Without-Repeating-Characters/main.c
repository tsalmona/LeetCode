#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLOCKSIZE 64

typedef struct HashNode{
    char key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct HashTable{
    struct HashNode **nodes;
} HashTable;

int hash(char c){
    return c % BLOCKSIZE;
}

struct HashTable *create(){
    struct HashTable *res = malloc(sizeof(HashTable));
    assert(res != NULL);

    res->nodes = malloc(sizeof(HashNode *) * BLOCKSIZE);
    assert(res != NULL);

    for(int i = 0; i < BLOCKSIZE; i++){
        res->nodes[i] = NULL;
    }

    return res;
}

void freeNode(HashNode *n){
    if(n != NULL){
        freeNode(n->next);
    }
    free(n);
}

void freeTable(struct HashTable *t){
    if(t != NULL){
        for(int i = 0; i <  BLOCKSIZE; i++){
            if(t->nodes[i] != NULL) freeNode(t->nodes[i]);
        }
        free(t);
    }
}

void set(struct HashTable *t, char key, int value){
    assert(t != NULL);
    int h = hash(key);

    HashNode *n = t->nodes[h];
    while(n){
        if(n->key == key){
            n->value = value;
            return;
        }
        n = n->next;
    }
    n = malloc(sizeof(HashNode));
    n->key = key;
    n->value = value;
    n->next = t->nodes[h];
    t->nodes[h] = n;
}

int get(struct HashTable *t, char c){
    int h = hash(c);

    HashNode *n = t->nodes[h];
    while(n){
        if(n->key == c){
            return n->value;
        }
        n = n->next;
    }
    return -1;
}

int max(int a, int b){
    return a < b ? b : a;
}

int lengthOfLongestSubstring(char* s) {
    struct HashTable *t = create();
    int maximum = 0;
    int l = 0;

    int len = strlen(s);
    for(int i = 0; i < len; i++){
        char c = s[i];
        int n = get(t, c);
        if(n != -1 && n >= i - l){
            maximum = max(l, maximum);
            l = i - n;
        } else {
            l++;
        }
        set(t, c, i);
    }
    maximum = max(l, maximum);

    freeTable(t);
    return maximum;
}

int lengthOfLongestSubstring2(char* s) {
    int counts[256] = {0};
    int l = 0; int r = 0; int maximum = 0;

    int len = strlen(s);

    while(r < len){
        counts[s[r]]++;

        while(counts[s[r]] > 1){
            counts[s[l]]--;
            l++;
        }

        maximum = max(maximum, r - l + 1);
        r++;
    }

    return maximum;
}

int main(){
    char *s = "abcabcbb";
    printf("%s : %d\n", s, lengthOfLongestSubstring(s));

    return 0;
}