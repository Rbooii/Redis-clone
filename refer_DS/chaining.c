#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct n {
    int value;
    struct n *next;
} n;

n *create(int val){
    n *newNode = (n*)malloc(sizeof(n));
    newNode->value = val;
    newNode->next = NULL;
    
    return newNode;
}

void pushTail(n **chain, int val){
    n *newnode = create(val);
    if(!(*chain)){
        (*chain) = newnode;
        return;
    }
    
    n *curr = (*chain);
    while(curr->next){
        curr = curr->next;
    }
    curr->next = newnode;
    return;
}

n *table[MAX];

void init(){
    for(int i = 0; i < MAX; i++){
        table[i] = NULL;
    }
}

int hash(int val){
    return val%MAX;
}

void insert(int val){
    int key = hash(val);
    pushTail(&table[key], val);
}

void search(int val){
    int key = hash(val);
    if(!table[key]){
        printf("NODATA\n");
        return;
    }
    
    int indx = 0;
    n *curr = table[key];
    while(curr && curr->value != val){
        curr = curr->next;
        indx++;
    }
    if(!curr){
        printf("NODATA\n");
        return;
    }else{
        printf("FOUND INDEX : %d, LL-INDEX : %d\n", key, indx);
        return;
    }
}

void delete(int val){
    int key = hash(val);
    if(!table[key]){
        printf("NODATA\n");
        return;
    }
    
    if(table[key]->value == val){
        n *del = table[key];
        table[key] = del->next;
        free(del);
        printf("DELETED %d\n", val);
        return;
    }
    
    n *curr = table[key];
    n *prev = NULL;
    while(curr && curr->value != val){
        prev = curr;
        curr = curr->next;
    }
    
    if(!curr){
        printf("NODATA\n");
        return;
    }
    
    prev->next = curr->next;
    free(curr);
    printf("DELETED %d\n", val);
    return;
}

void display(){
    printf("=====================\n");
    for(int i = 0; i < MAX; i++){
        printf("index : %d => ", i);
        n *curr = table[i];
        while(curr){
            printf("%d%s", curr->value, curr->next ? "->" : "->NULL");
            curr = curr->next;
        }
        printf("\n");
    }
    printf("=====================\n");
}

int main(){
    init();
    
    insert(1);
    insert(31);
    
    display();
    search(31);
    delete(1);
    display();
    return 0;
}