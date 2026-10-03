/* vector.c —— 你要实现的地方 */

#include "vector.h"
#include <stdlib.h>
#include <assert.h>

int vector_init(vector *v, size_t capacity) {
    if(capacity==0){
        v->cap=v->data=v->end=NULL;
        return 0;
    }
    if(capacity>SIZE_MAX/sizeof(int)){
        v->cap=v->data=v->end=NULL;
        return -1;
    }
    v->data=malloc(capacity*sizeof(int));
    if(v->data==NULL){
        v->cap=v->end=NULL;  
        return -1;
    }
    v->end=v->data;
    v->cap=v->data+capacity;
    return 0;
}

void vector_destroy(vector *v) {
    if(v==NULL)return;
    free(v->data);
    v->data=NULL;v->cap=NULL;v->end=NULL;
}

size_t size(const vector *v) {
    assert(v!=NULL);
    if(v->data==NULL)return 0;
    size_t a=(v->end-v->data);
    return a;
}

size_t capacity(const vector *v) {
    assert(v!=NULL);
    if(v->data==NULL)return 0;
    size_t a=(v->cap-v->data);
    return a;
}

int empty(const vector *v) {
    assert(v!=NULL);
    if(size(v)==0)return 1;
    return 0;
}

int get(const vector *v, size_t index, int *out) {
    if (index>=size(v))
    {
        return -1;
    }
    *out=*(v->data+index);
    return 0;
}

int set(vector *v, size_t index, int value) {
    if(index>=size(v))return -1;
    *(v->data+index)=value;
    return 0;
}

int front(const vector *v, int *out) {
    if(empty(v))return -1;
    *out=*(v->data);
    return 0;
}

int back(const vector *v, int *out) {
    if(empty(v))return -1;
    *out=*(v->end-1);
    return 0;
}

int push_back(vector *v, int value) {
    if(v==NULL)return -1;
    if(size(v)<capacity(v)){
        *(v->end)=value;
        v->end++;
        return 0;
    }
    if(capacity(v)==0){
        int *p;
        p=realloc(v->data,sizeof(int));
        if(p==NULL)return -1;
        v->data=p;
        v->cap=v->data+1;
        v->end=v->data;
        *(v->end)=value;
        v->end++;
        return 0;
    }
    if(size(v)==capacity(v)){
        int x=reserve(v,2*capacity(v));
        if(x==-1){return -1;}
        *(v->end)=value;
        v->end++;
    }
    return 0;
}

int pop_back(vector *v, int *out) {
    if(v==NULL)return -1;
    if(empty(v))return -1;
    v->end--;
    *out=*(v->end);
    return 0;
}

int reserve(vector *v, size_t new_capacity) {
    if(v==NULL)return -1;
    size_t x=size(v);
    if(new_capacity>SIZE_MAX/sizeof(int))return -1;
    if(new_capacity<=capacity(v))return 0;
    //防止扩容失败
    int *p; 
    p=realloc(v->data,new_capacity*sizeof(int));
    if(p==NULL)return -1;
    v->data=p;
    p=NULL;
    v->cap=v->data+new_capacity;
    v->end=v->data+x;
    return 0;
}

int shrink_to_fit(vector *v) {
    if(v==NULL)return -1;
    size_t x=size(v);
    //下面这个检查有何用？
    if(size(v)==0){
        free(v->data);
        v->data=v->end=v->cap=NULL;
        return 0;
    }
    int* p;
    p=realloc(v->data,(v->end-v->data)*sizeof(int));
    if(p==NULL)return -1;
    v->data=p;
    v->end=v->data+x;
    v->cap=v->end;
    return 0;
}

void clear(vector *v) {
    v->end=v->data;
}
