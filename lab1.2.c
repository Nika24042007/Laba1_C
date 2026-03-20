#include <stdio.h>
#include <math.h> 
#include <stdlib.h>

typedef struct Node
{
   int number;
   struct Node *right;
   struct Node *left;
   struct Node *last;
   int level;
} Node;

Node *create(int n, Node *last, int level){
    Node *tmp = malloc(sizeof(Node));
    if (tmp == NULL){
        free(tmp);
        printf("Memory error\n");
        return 0;
    }
    tmp->last = last;
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->number = n;
    tmp->level = level+1;
    return tmp;
}

Node **add_node(int *len, Node **tree, Node *node, int n){
    if ((node->right == NULL && n > node->number) || (node->left == NULL && n < node->number)){
        for (int i = 0; i < *len; i++){
            if (tree[i]->number == n){
                printf("Error: indetical nods\n");
                return tree;
            }
        }
        Node *new_node = create(n, NULL, 0);
        new_node->last = node;
        new_node->level = node->level+1;
        if (node->right == NULL && n > node->number){
            node->right = new_node;
        }
        else if (node->left == NULL && n <node->number){
            node->left = new_node;
        }
        (*len)++;
        Node **new_tree = realloc(tree, *len*sizeof(Node*));
        if(new_tree == NULL){
            free(new_tree);
            printf("Memory error\n");
            return tree;
        }
        new_tree[*len-1] = new_node;
        tree = new_tree;
        return tree;
        }
        else if(n > node->number){
            return add_node(len, tree, node->right, n);
        }
        else if (n < node->number){
            return add_node(len, tree, node->left, n);
        }
}

Node **add(int *len, Node **tree, int n){
    
    
    if (*len == 0) {
        Node *new_node = create(n, NULL, 0);
        Node **new_tree = malloc(sizeof(Node*));
        if (new_tree == NULL){
            free(new_tree);
            printf("Memory error\n");
            return tree;
        }
        new_tree[0] = new_node;
        *len = 1;
        return new_tree;
    }
    else{
        return add_node(len, tree, tree[0], n);
    }
}

Node **delete(int *len, Node **tree, int n){
    if (*len == 0) {
        printf("Error: tree is empty. Please add nodes.\n");
        return tree;
    }

    int idx = -1;
    for (int i = 0; i < *len; i++) {
        if (tree[i]->number == n) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Error: no such node.\n");
        return tree;
    }

    Node *del = tree[idx];
    Node *parent = del->last;
    
    if (del->left == NULL && del->right == NULL) {
        if (parent != NULL) {
            if (parent->left == del){
                parent->left = NULL;}
            else{
                parent->right = NULL;}
        }
        free(del);
        Node **new_tree = malloc((*len - 1) * sizeof(Node*));
        if (new_tree == NULL) {
            free(new_tree);
            printf("Memory error\n");
            return tree;
        }
        int j = 0;
        for (int i = 0; i < *len; i++) {
            if (i != idx)
                new_tree[j++] = tree[i];
        }
        *len = *len - 1;
        return new_tree;
    }
    else if (del->left == NULL || del->right == NULL) {
        Node *child;
        if (del->left != NULL) {
            child = del->left;   
        } else {
            child = del->right;
        }
        if (parent != NULL) {
            if (parent->left == del){
                parent->left = child;
            }else{
                parent->right = child;
            }
        }
        child->last = parent;
        free(del);
        Node **new_tree = malloc((*len - 1) * sizeof(Node*));
        if (new_tree == NULL) {
            free(new_tree);
            printf("Memory error.\n");
            return tree;
        }
        int j = 0;
        for (int i = 0; i < *len; i++) {
            if (i != idx)
                new_tree[j++] = tree[i];
        }
        *len = *len - 1;
        return new_tree;
    }
    else{
        Node *minNode = del->right;
        while (minNode->left != NULL)
            minNode = minNode->left;
        int minIdx = -1;
        for (int i = 0; i < *len; i++) {
            if (tree[i] == minNode) {
                minIdx = i;
                break;
            }
        }
        if (minIdx == -1) {
            printf("Error\n");
            return tree;
        }

        Node **new_tree = malloc((*len - 1) * sizeof(Node*));
        if (new_tree == NULL) {
            free(new_tree);
            printf("Memory error.\n");
            return tree;
        }
        int j = 0;
        for (int i = 0; i < *len; i++) {
            if (i != minIdx)
                new_tree[j++] = tree[i];
        }

        del->number = minNode->number;

        Node *minParent = minNode->last;
        if (minParent->left == minNode){
            minParent->left = minNode->right;}
        else{
            minParent->right = minNode->right;}
        if (minNode->right != NULL){
            minNode->right->last = minParent;}
        free(minNode);

        *len = *len - 1;
        free(tree);
        return new_tree;
    }
}
void print_node(Node *node, int level, char side) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    if (level > 0) {
        printf("%c: ", side);
    }
    printf("%d\n", node->number);

    print_node(node->left, level + 1, 'L');
    print_node(node->right, level + 1, 'R');
}

void print(int *len, Node **tree){
    if (*len == 0) {
        printf("Empty\n");
        return;
    }
    Node *root = NULL;
    for (int i = 0; i < *len; i++) {
        if (tree[i]->last == NULL) {
            root = tree[i];
            break;
        }
    }
    if (root == NULL) {
        printf("Error: can not find root\n");
        return;
    }
    printf("Tree\n");
    print_node(root, 0, ' ');
}

int check_avl(Node *node, int *ok) {
    if (node == NULL) {
        return 0; 
    }                             
    int left_h = check_avl(node->left, ok);                  
    int right_h = check_avl(node->right, ok);                
    if (abs(left_h - right_h) > 1) {
        *ok = 0;  }                
    int max_h;
    if (left_h > right_h){
        max_h = left_h;}
    else{
        max_h = right_h;}
    return 1 + max_h;      
}

void check(int *len, Node **tree){
    if (*len == 0) { 
        printf("Empty\n"); 
        return; 
    } 
    Node *root = NULL;
    for (int i = 0; i < *len; i++){                            
        if (tree[i]->last == NULL) {
            root = tree[i];
            break; 
        }
    }
    int ok = 1; 
    check_avl(root, &ok);
    if (ok == 1){
        printf("This is AVL\n");
        return;
    }else{
        printf("This is not AVL\n");
        return;
    }
}

int main(void){
    int flag = 1;
    int len = 0;
    Node **tree = malloc(len*sizeof(struct Node*));
    if (tree == NULL){
        free(tree);
        printf("Memory error");
        return 0;
    }
    while (flag == 1){
        printf("Enter command : 0)Exit 1)Add nod  2)Delete nod  3)Print  4)Check AVL\n");
        int command;
        scanf("%d", &command);
        switch (command){
        case 0:
            flag = 0;
            break;

        case 1:
            int n;
            printf("Enter new node: ");
            scanf("%d", &n);
            tree = add(&len, tree, n);
            break;

        case 2:
            int k;
            printf("Enter nod to delete: ");
            scanf("%d", &k);
            tree = delete(&len, tree, k);
            break;

        case 3:
            print(&len, tree);
            break;

        case 4:
            check(&len, tree);
            break;
    
        default:
            break;
        }
    }
    free(tree);
    return 0;
}