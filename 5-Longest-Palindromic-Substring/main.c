#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void longestPalindromeB(char *s, int slen, int l, int r, int *ptr){
    while((l >= 0 && r <= slen - 1) && (s[l] == s[r])){
        l--;
        r++;
    }
    if(r == l+1) return;

    ptr[0] = l+1;
    ptr[1] = r-1;
}

char *getSubstring(int *t, char *s){
    if(t == NULL){
        return NULL;
    } else {
        char *res = malloc((t[1] - t[0] + 1 + 1) * sizeof(char));
        memcpy(res, s + t[0], t[1] - t[0] + 1);
        res[t[1] - t[0] + 1] = '\0';
        return res;
    }
}

int len(int *t){
    if(t == NULL){
        return 0;
    } else {
        return t[1] - t[0] + 1;
    }
}

char* longestPalindrome(char* s) {
    int slen = strlen(s);

    int *max = malloc(2 * sizeof(int));
    max[0] = 0;
    max[1] = 0;

    int lenMax = len(max);

    int b = 1;
    int c = 0;
    int i = (slen - 1) / 2;

    int *p1 = malloc(2 * sizeof(int));
    int *p2 = malloc(2 * sizeof(int));

    while(1){
        longestPalindromeB(s, slen, i, i, p1);
        longestPalindromeB(s, slen, i, i+1, p2);
        int len1 = len(p1);
        int len2 = len(p2);

        if(len1 > lenMax){
            max[0] = p1[0];
            max[1] = p1[1];
            lenMax = len1;
        }
        if(len2 > lenMax){
            max[0] = p2[0];
            max[1] = p2[1];
            lenMax = len2;
        }

        if(b){ b = 0; c++; i += c; } else { b = 1; c++; i -= c; }
        if(i + lenMax/2 >= slen || i - lenMax/2 < 0){
            break;
        }
    }

    char *res = getSubstring(max, s);
    if(max != NULL) free(max);
    return res;
}

int main(){
    return 0;
}