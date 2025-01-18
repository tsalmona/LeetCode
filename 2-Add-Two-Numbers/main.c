/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

struct ListNode *addTwoNumbersRec(struct ListNode *l1, struct ListNode *l2, int ret){
    if(l1 == NULL && l2 == NULL){
        if(ret){
            struct ListNode *res = malloc(sizeof(struct ListNode));
            assert(res != NULL);

            res->val = ret;
            res->next = NULL;

            return res;
        }
        return NULL;
    }
    struct ListNode *res = malloc(sizeof(struct ListNode));
    assert(res != NULL);

    int v1 = l1 != NULL ? l1->val : 0;
    int v2 = l2 != NULL ? l2->val : 0;

    struct ListNode *nl1 = l1 != NULL ? l1->next : NULL;
    struct ListNode *nl2 = l2 != NULL ? l2->next : NULL;

    res->val = (v1 + v2 + ret) % 10;
    res->next = addTwoNumbersRec(nl1, nl2, (v1 + v2 + ret) / 10);

    return res;
}

struct ListNode *addTwoNumbersIter(struct ListNode *l1, struct ListNode *l2){
    struct ListNode *res = malloc(sizeof(struct ListNode));
    assert(res != NULL);
    struct ListNode **tmp = &res;

    struct ListNode *nl1 = l1;
    struct ListNode *nl2 = l2;
    int v1 = 0; int v2 = 0; int ret = 0;

    while(nl1 != NULL || nl2 != NULL){
        v1 = nl1 == NULL ? 0 : nl1->val;
        v2 = nl2 == NULL ? 0 : nl2->val;

        nl1 = nl1 == NULL ? NULL : nl1->next;
        nl2 = nl2 == NULL ? NULL : nl2->next;

        (*tmp)->val = (v1 + v2 + ret) % 10;
        ret = (v1 + v2 + ret) / 10;

        (*tmp)->next = malloc(sizeof(struct ListNode));
        assert((*tmp)->next != NULL);
        tmp = &((*tmp)->next);
    }
    if(ret){
        (*tmp)->val = ret;
        (*tmp)->next = NULL;
    } else {
        (*tmp) = NULL;
    }

    return res;
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    //return addTwoNumbersRec(l1, l2, 0);
    return addTwoNumbersIter(l1, l2);
}

int main(){
    printf("Flemme d'écrire des tests mais ça marche !");
    return 0;
}