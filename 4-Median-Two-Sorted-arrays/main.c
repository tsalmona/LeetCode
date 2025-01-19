#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct easyList{
    int *arr;
    int left; // bord gauche de la fenêtre, inclus dans la fenêtre
    int right; // bord droit de la fenêtre, pas inclus dans la fenêtre
    int maxsize; // pour l'affichage uniquement
} easyList;

struct easyList *create(int *t, int tsize){
    struct easyList *l = malloc(sizeof(easyList));
    assert(l != NULL);
    if(tsize > 0) {
        l->arr = malloc(tsize * sizeof(int));
        assert(l->arr != NULL);
    } else {
        l->arr = NULL;
    }

    for(int i = 0; i < tsize; i++){
        l->arr[i] = t[i];
    }
    l->left = 0;
    l->right = tsize;
    l->maxsize = tsize;

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
    for(int i = 0; i < l->maxsize; i++){
        if(i >= l->left && i < l->right) printf("\033[1;31m%d\033[0m", l->arr[i]);
        else printf("%d", l->arr[i]);
        if(i != l->maxsize-1) printf(", ");
    }
    printf(" ]\n");
}

// La taille de la fenêtre s'exprime donc facilement.
int size(struct easyList *l){
    assert(l != NULL);
    return l->right - l->left;
}

// Modifie le bord droit de la fenêtre et le place en position i
void keepLeftPart(struct easyList *l, int i){
    assert(l != NULL);
    if(i >= l->left && i <= l->right){
        l->right = i;
    }
}

// Modifie le bord gauche de la fenêtre et le place en position i
void keepRightPart(struct easyList *l, int i){
    assert(l != NULL);
    if(i >= l->left && i <= l->right){
        l->left = i;
    }
}

int mid(int a, int b){
    return (a + b) / 2;
}
int max(int a, int b){
    return a > b ? a : b;
}
int min(int a, int b){
    return a < b ? a : b;
}

int *findTargets(struct easyList *l1, struct easyList *l2, int t1, int t2);

int *findTargets(struct easyList *l1, struct easyList *l2, int t1, int t2){
    assert(l1 != NULL && l2 != NULL);
    assert((t1 <= t2) && (t1 >= 0) && (t2 <= (size(l1) + size(l2))));
    assert((t1 == t2) || (t2 = t1 + 1));

    if(size(l1) == 0){
        int *res = malloc(2 * sizeof(int));
        assert(res != NULL);

        res[0] = l2->arr[l2->left + t1 - 1];
        res[1] = l2->arr[l2->left + t2 - 1];

        return res;
    } else if(size(l2) == 0){
        return findTargets(l2, l1, t1, t2);
    } else if(size(l1) == 1 && size(l2) == 1){
        int *res = malloc(2 * sizeof(int));
        assert(res != NULL);

        res[0] = min(l1->arr[l1->left], l2->arr[l2->left]);
        res[1] = max(l1->arr[l1->left], l2->arr[l2->left]);

        return res;
    } else if(size(l1) == 1 && size(l2) == 2){
        int *res = malloc(2 * sizeof(int));
        assert(res != NULL);
        int *sorted = malloc(3 * sizeof(int));
        assert(sorted != NULL);

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
        res[0] = sorted[t1-1];
        res[1] = sorted[t2-1];

        free(sorted);
        return res;
    } else if(size(l1) == 2 && size(l2) == 1){
        return findTargets(l2, l1, t1, t2);
    }
    
    int i1 = mid(l1->left, l1->right);
    int m1 = l1->arr[i1];
    int i2 = mid(l2->left, l2->right);
    int m2 = l2->arr[i2];

    if(m1 < m2){
        int sup_rank1 = size(l1) + size(l2) - (l2->right - i2) - (l1->right - i1 - 1); // le rang de m1 est inférieur à sup_rank1
        int inf_rank2 = (i2 - l2->left) + (i1 + 1 - l1->left) + 1; // le rang de m2 est supérieur à inf_rank2
        int nt1 = t1; int nt2 = t2;

        if(sup_rank1 < t1) { nt1 -= i1+1 - l1->left; nt2 -= i1+1 - l1->left; keepRightPart(l1, i1+1); }
        if(sup_rank1 == t1) { nt1 -= i1 - l1->left; nt2 -= i1 - l1->left; keepRightPart(l1, i1); }
        if(inf_rank2 > t2) { keepLeftPart(l2, i2); }
        if(inf_rank2 == t2) { keepLeftPart(l2, i2+1); }

        return findTargets(l1, l2, nt1, nt2);
    } else if(m1 > m2){
        return findTargets(l2, l1, t1, t2);
    } else {
        // Dans ce cas, m1 == m2, on va supposer que le rang de ma < mb
        int sup_rank1 = size(l1) + size(l2) - (l2->right - i2) - (l1->right - i1 - 1); // le rang de m1 est inférieur à sup_rank1
        int inf_rank2 = (i2 - l2->left) + (i1 + 1 - l1->left) + 1; // le rang de m2 est supérieur à inf_rank2
        int nt1 = t1; int nt2 = t2;

        if(sup_rank1 < t1) { nt1 -= i1+1 - l1->left; nt2 -= i1+1 - l1->left; keepRightPart(l1, i1+1); }
        if(sup_rank1 == t1) { nt1 -= i1 - l1->left; nt2 -= i1 - l1->left; keepRightPart(l1, i1); }
        if(inf_rank2 > t2) { keepLeftPart(l2, i2); }
        if(inf_rank2 == t2) { keepLeftPart(l2, i2+1); }

        return findTargets(l1, l2, nt1, nt2);
    }
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    if(nums1Size + nums2Size > 1){
        struct easyList *l1 = create(nums1, nums1Size);
        struct easyList *l2 = create(nums2, nums2Size);

        int *res = findTargets(l1, l2, (nums1Size + nums2Size) / 2, (nums1Size + nums2Size) / 2 + 1);
        double median = (nums1Size + nums2Size) % 2 == 0 ? (res[0] + res[1]) / 2.0 : res[1];

        free(res);
        return median;
    } else {
        if(nums1Size == 1) return nums1[0];
        else return nums2[0];
    }
}

int main(){
    int t1[] = {3};
    int t2[] = {1, 2, 4, 5, 6, 7};
    struct easyList *l1 = create(t1, 1);
    struct easyList *l2 = create(t2, 6);

    int *res = findTargets(l1, l2, 3, 4);
    printf("res[0] = %d, res[1] = %d\n", res[0], res[1]);

    if(res != NULL) free(res);
    freeList(l1);
    freeList(l2);
    return 0;
}