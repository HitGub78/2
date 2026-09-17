#include <stdio.h>
#include <stdlib.h>
#define CAP 100


typedef struct darray{
    int *ar;
    int size;
    int cap;
}darray;

darray* init(){
    darray* S = malloc(sizeof(darray));
    S->size = 0;
    S->cap = CAP;
    S->ar = malloc(CAP*sizeof(int));
    return S;
}

void delete(darray *S){
    free(S->ar);
    free(S);
}

void push(darray *S, int el){
    if (S->size == S->cap){
        S->cap *= 2;
        S->ar = realloc(S->ar,S->cap*sizeof(int));
    }
    S->ar[S->size] = el;
    S->size++;
}

void pop(darray *S){
    if (S->size == 0) return;
    else S->size --;
    if (S->cap > 100 && S->size < S->cap / 4){
        S->cap /= 2;
        S->ar = realloc(S->ar,S->cap*sizeof(int));
    }
    
}

int idex(darray *S, int i){
    return S->ar[i];
}

void change(darray *S, int i, int val){
    if (i >= S->size) return;
    S->ar[i] = val;
}

void clear(darray *S){
    S->cap = CAP;
    S->ar = realloc(S->ar,CAP*sizeof(int));
    S->size = 0;
}

void print(darray*S){
    for (int i = 0; i < S->size; i++){
        printf("%d ",S->ar[i]);
    }
    printf("\n");
}

int main(){
    darray *S = init();
    int input = -1, el = 0, idx=0;
    while(input){
        scanf("%d",&input);
        switch(input){
            case 1:
                scanf("%d",&el);
                push(S,el);
                break;
            case 2:
                pop(S);
                break;
            case 3:
                scanf("%d",&idx);
                if (idx < S->size) printf("%d\n", idex(S,idx));
                else printf("Index out of range\n");
                break;
            case 4:
                scanf("%d %d",&idx,&el);
                change(S,idx,el);
                break;
            case 5:
                clear(S);
                break;
            case 6:
                print(S);
                break;
            default:
                break;
        }
    }
    delete(S);
    return 0;
}