#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct easyList{
    int *arr;
    int left; // bord gauche de la fenêtre, inclus dans la fenêtre
    int right; // bord droit de la fenêtre, pas inclus dans la fenêtre
} easyList;

struct easyList *create(int *t, int tsize){
    struct easyList *l = malloc(sizeof(easyList));
    assert(l != NULL);
    l->arr = malloc(tsize * sizeof(int));
    assert(l->arr != NULL);

    for(int i = 0; i < tsize; i++){
        l->arr[i] = t[i];
    }
    l->left = 0;
    l->right = tsize;

    return l;
}

void freeList(struct easyList *l){
    assert(l != NULL);
    free(l->arr);
    free(l);
}

void printList(struct easyList *l){
    assert(l != NULL);
    printf("[ ");
    for(int i = l->left; i < l->right; i++){
        printf("%d", l->arr[i]);
        if(i != l->right-1) printf(", ");
    }
    printf(" ]\n ");
    for(int i = l->left; i < l->right; i++){
        if(i == l->left || i == l->right-1) printf(" ^ ");
        else printf("   ");
    }
    printf("\n");
}

// La taille de la fenêtre s'exprime donc facilement.
int size(struct easyList *l){
    assert(l != NULL);
    return l->right - l->left;
}

// Modifie le bord droit de la fenêtre et le place en position i
void keepLeftPart(struct easyList *l, int i){
    assert(l != NULL);
    if(i >= l->left && i < l->right){
        l->right = i;
    }
}

// Modifie le bord gauche de la fenêtre et le place en position i
void keepRightPart(struct easyList *l, int i){
    assert(l != NULL);
    if(i >= l->left && i < l->right){
        l->left = i;
    }
}

int mid(int a, int b){
    return (a + b) / 2;
}

int *findTargets(struct easyList *l1, struct easyList *l2, int t1, int t2);

int *findTargets(struct easyList *l1, struct easyList *l2, int t1, int t2){
    assert(l1 != NULL && l2 != NULL);

    printList(l1);
    printList(l2);
    printf("t1 = %d, t2 = %d\n\n", t1, t2);
    
    assert((t1 <= t2) && (t1 >= 0) && (t2 <= (size(l1) + size(l2))));
    assert((t1 == t2) || (t2 = t1 + 1));

    if(size(l1) == 1 && size(l2) == 2){
        int *res = malloc(2 * sizeof(int));
        int *sorted = malloc(3 * sizeof(int));

        if(l1->arr[l1->left] < l2->arr[l2->left]){
            sorted[0] = l1->arr[l1->left];
            sorted[1] = l2->arr[l2->left];
            sorted[2] = l2->arr[l2->left+1];
        } else if(l1->arr[l1->left] > l2->arr[l2->left+1]){
            sorted[0] = l2->arr[l2->left];
            sorted[1] = l2->arr[l2->left+1];
            sorted[2] = l1->arr[l1->left];
        } else {
            sorted[0] = l2->arr[l2->left];
            sorted[1] = l1->arr[l1->left];
            sorted[2] = l2->arr[l2->left+1];
        }
        res[0] = sorted[t1];
        res[1] = sorted[t2];

        free(sorted);
        return res;
    } else if(size(l1) == 2 && size(l2) == 1){
        return findTargets(l2, l1, t2, t1);
    } else if(size(l1) == 2 && size(l2) == 2){
        return NULL;
    }
    
    int i1 = mid(l1->left, l1->right);
    int m1 = l1->arr[i1];
    int i2 = mid(l2->left, l2->right);
    int m2 = l2->arr[i2];

    if(m1 < m2){
        int sup_rank1 = size(l1) + size(l2) - (l2->right - i2) - (l1->right - i1 - 1); // le rang de m1 est inférieur à sup_rank1
        int inf_rank2 = (i2 - l2->left) + (i1 + 1 - l1->left) + 1; // le rang de m2 est supérieur à inf_rank2

        if(sup_rank1 < t1) { t1 -= i1+1 - l1->left; t2 -= i1+1 - l1->left; keepRightPart(l1, i1+1); }
        if(sup_rank1 == t1) { t1 -= i1 - l1->left; t2 -= i1 - l1->left; keepRightPart(l1, i1); }
        if(sup_rank1 >= t2) { keepLeftPart(l1, i1+1); keepLeftPart(l2, i2); }
        if(inf_rank2 > t2) keepLeftPart(l2, i2);
        if(inf_rank2 == t2) keepLeftPart(l2, i2+1);
        if(inf_rank2 <= t1) { 
            t1 -= i2 - l2->left; t2 -= i2 - l2->left; keepRightPart(l2, i2); 
            t1 -= i2 - l2->left; t2 -= i2 - l2->left; keepRightPart(l1, i1+1); 
        }

        return findTargets(l1, l2, t1, t2);
    }
    if(m1 > m2){
        return findTargets(l2, l1, t1, t2);
    }
    //if(m1 == m2){
        int sup_rank = size(l1) + size(l2) - (l2->right - i2 - 1) - (l1->right - i1 - 1); // les rangs de m1 et m2 sont inférieurs à sup_rank
        int inf_rank = (i2 - l2->left) + (i1 - l1->left); // les rangs de m1 et m2 sont supérieurs à inf_rank

        if(sup_rank < t1) { 
            t1 -= i1+1 - l1->left; t2 -= i1+1 - l1->left; keepRightPart(l1, i1+1);
            t1 -= i2+1 - l2->left; t2 -= i2+1 - l2->left; keepRightPart(l2, i2+1);
        }
        if(sup_rank == t1) { 
            t1 -= i1+1 - l1->left; t2 -= i1+1 - l1->left; keepRightPart(l1, i1+1);
            t1 -= i2 - l2->left; t2 -= i2 - l2->left; keepRightPart(l2, i2);
        }
        if(inf_rank > t2) {
            keepLeftPart(l1, i1);
            keepLeftPart(l2, i2);
        }
        if(inf_rank == t2) {
            keepLeftPart(l1, i1+1);
            keepLeftPart(l2, i2);
        }
        return findTargets(l1, l2, t1, t2);
    //}
}

int main(){
    int t1[] = {1, 3, 5, 7, 9};
    int t2[] = {2, 4, 6, 8, 9};
    struct easyList *l1 = create(t1, 5);
    struct easyList *l2 = create(t2, 5);

    int *res = findTargets(l1, l2, 3, 4);
    printf("res[0] = %d, res[1] = %d\n", res[0], res[1]);

    if(res != NULL) free(res);
    freeList(l1);
    freeList(l2);
    return 0;
}