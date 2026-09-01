#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key; //id pasien
    int height;
    struct Node *left, *right;
} Node;

Node *createNode(int v){
    Node *nn = (Node *)malloc(sizeof(Node));
    nn->left = nn->right = NULL;
    nn->key = v;
    nn->height = 1;
    return nn;
}

int max(int a, int b){
    return (a>b)?a:b;
}

int height(Node *x){
    if(!x) return 0;
    else return x->height;
}

int balance(Node *x){
    return height(x->left) - height(x->right);
}

Node *rotate_r(Node *x){
    Node *y = x->left;
    Node *t = y->right;
    y->right = x;
    x->left = t;
    x->height = 1+max(height(x->left),height(x->right));
    y->height = 1+max(height(y->left),height(y->right));
    return y;
}

Node *rotate_l(Node *x){
    Node *y = x->right;
    Node *t = y->left;
    y->left = x;
    x->right = t;
    x->height = 1+max(height(x->left),height(x->right));
    y->height = 1+max(height(y->left),height(y->right));
    return y;
}

Node *successor(Node *r){
    Node *x = r->right;
    while(x -> left){
        x = x->left;
    }
    return x;
}

Node *insertNode(Node *root, int k){
    if(!root) return createNode(k);
    else if(k < root->key) root->left = insertNode(root->left, k);
    else if(k > root->key) root->right = insertNode(root->right, k);
    else return root;
    root->height = 1+max(height(root->left),height(root->right));
    //balance
    int bf = balance(root);
    if(bf > 1 && k < root->left->key) return rotate_r(root);
    if(bf < -1 && k > root->right->key) return rotate_l(root);
    if(bf > 1 && k > root->left->key){
        root->left = rotate_l(root->left);
        return rotate_r(root);
    }
    if(bf < -1 && k < root->right->key){
        root->right = rotate_r(root->right);
        return rotate_l(root);
    }
    return root;
}

Node *removeNode(Node *root, int k){
    if(!root) return root;
    else if(k < root->key) root->left = removeNode(root->left, k);
    else if(k > root->key) root->right = removeNode(root->right, k);
    else{
        if(!root->left || !root->right){
            Node *t = (root->left)?root->left : root->right;
            if(!t){
                t = root;
                root = NULL;
            }else{
                *root = *t;
            }
            free(t);
        }else{
            Node *t = successor(root);
            root->key = t->key;
            root->right = removeNode(root->right,t->key);
        }
    }
    if(!root)return root;
    root->height = 1+max(height(root->left), height(root->right));
    int bf = balance(root);
    if(bf > 1 && balance(root->left) >= 0) return rotate_r(root);
    if(bf > 1 && balance(root->left) < 0) {
        root->left = rotate_l(root->left);
        return rotate_r(root);
    }
    if(bf < -1 && balance(root->right) <= 0) return rotate_l(root);
    if(bf < -1 && balance(root->right) > 0){
        root->right = rotate_r(root->right);
        return rotate_l(root);
    }
    return root;
}

void PrintTree(Node *root, int idx){
    if(!root) return;
    PrintTree(root->right, idx+1);
    for(int i = 0; i < idx; i++){
        printf("   ");
    }
    printf("%d [bf:%d, h:%d]\n", root->key, balance(root), height(root));
    PrintTree(root->left, idx+1);
}

Node *searchNode(Node *root, int x){
    if(!root) return NULL;
    else if(x < root->key) return searchNode(root->left, x);
    else if (x > root->key) return searchNode(root->right, x);
    else return root;
}

int main(void){
    Node *root = NULL;
    root = insertNode(root, 5);
    root = insertNode(root, 6);
    root = insertNode(root, 7);
    root = insertNode(root, 8);
    root = removeNode(root, 6);
    PrintTree(root, 0);
    return 0;
}