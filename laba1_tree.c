#include <stdio.h>
#include <math.h> 
#include <stdlib.h>

typedef struct Node
{
   int number;
   struct Node *right;
   struct Node *left;
} Node;

Node *create(int num){
    Node *node = malloc(sizeof(Node));
    if (node == NULL){
        free(node);
        printf("Memory error\n");
        return 0;
    }
    node->number = num;
    node->left = NULL;
    node->right = NULL;
    return node;

}

Node *min_node(Node* root){
    if (root->left == NULL){
        return root;
    }else{
        min_node(root->left);
    }
}

Node *min_parent(Node *root, Node *mimNode){
    if (root->left == mimNode){
        return root;
    }else{
        min_parent(root->left, mimNode);
    }
}

void delete(Node *root, Node *parent, int num){
    if (root == NULL){
        return;
    }
    if (root->number == num){
        if (root->right == NULL && root->left == NULL){
            if (parent->right == root){
                parent->right = NULL;
            }else{
                parent->left = NULL;
            }
            free(root);
            return;
        }else if(root->right == NULL || root->left == NULL){
            Node *child = (root->right != NULL) ? root->right : root->left; 
            if(root->right != NULL){
                if (parent == NULL){
                    Node* t = root->right;
                    root->number = root->right->number;
                    root->left = root->right->left;
                    root->right = root->right->right;
                    free(t);
                    return;
                }
                else if(parent->right == root){
                    parent->right = child;
                    free(root);
                    return;
                }else{
                    parent->left = child;
                    free(root);
                    return;
                }
            }else{
                if (parent == NULL){
                    Node* t = root->left;
                    root->number = root->left->number;
                    root->right = root->left->right;
                    root->left = root->left->left;
                    free(t);
                    return;
                }
                else if(parent->right == root){
                    parent->right = child;
                    free(root);
                    return;
                }else{
                    parent->left = child;
                    free(root);
                    return;
                }
            }
        }else{
            Node *minNode = min_node(root->right);
            Node *minParent = NULL;
            if (root->right == minNode){
                if (parent != NULL){
                    if (parent->right == root){
                        parent->right = minNode;
                        minNode->left = root->left;
                        free(root);
                        return;
                    }else{
                        parent->left = minNode;
                        minNode->left = root->left;
                        free(root);
                        return;
                    }
                }else{
                    Node *t = minNode;
                    root->number = minNode->number;
                    root->right = minNode->right;
                    free(t);
                    return;}
            }else{
                minParent = min_parent(root->right, minNode);
            }
            if (parent == NULL){
                minParent->left = minNode->right;
                root->number = minNode->number;
                free(minNode);
                return;
            }
            else if (parent->right == root){
                parent->right = minNode;
                minParent->left = minNode->right;
                minNode->right = root->right;
                minNode->left = root->left;
                free(root);
                return;
            }else{
                parent->left = minNode;
                minParent->left = minNode->right;
                minNode->right = root->right;
                minNode->left = root->left;
                free(root);
                return;
            }
        }
    }else{
        if (num > root->number){delete(root->right, root, num);}
        else{delete(root->left, root, num);}
    }

}

void add(Node *root, int num){
    if (num == root->number){
        printf("Error: indetical nods\n");
        return;
    }else if(num > root->number){
        if(root->right == NULL){
            root->right = create(num);
            return;
        }else{
            return add(root->right, num);
        }
    }else if(num < root->number){
        if(root->left == NULL){
            root->left = create(num);
            return;
        }else{
            return add(root->left, num);
        }
    }
}

void print(Node *root, int level, char side){
    if (root == NULL){
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    if (level > 0) {
        printf("%c: ", side);
    }
    printf("%d\n", root->number);

    print(root->right, level+1, 'R');
    print(root->left, level+1,'l');

}
int chek(Node *root, int *ok){
    if (root == NULL){
        return 0;
    }
    int max_right = chek(root->right, ok);
    int max_left = chek(root->left, ok);
    if (abs(max_left - max_right) > 1) {
        *ok = 0;  }
    if (max_left > max_right){
        return max_left+1;
    }else{
        return max_right+1;
    }
}

void chek_avl(Node *root){
    int ok = 1;
    chek(root, &ok);
    if (ok == 1){
        printf("This is AVL\n");
        return;
    }else{
        printf("This is not AVL\n");
        return;
    }
}

void delete_free(Node* root){
    if (root->left==NULL && root->right==NULL){
        printf("Free %d\n", root->number);
        free(root);
        return;
    }if(root->left != NULL){
        delete_free(root->left);
    }if(root->right != NULL){
        delete_free(root->right);
    }
    printf("Free %d\n", root->number);
    free(root);
}

void main(int){
    int num;
    printf("You have empty tree, please, enter root: ");
    scanf("%d", &num);
    Node *root = create(num);
    int f = 1;
    while (f != 0){
        int command;
        printf("Enter command : 0)Exit 1)Add nod  2)Delete nod  3)Print  4)Check AVL\n");
        scanf("%d", &command);
        switch (command){
            case 0:
                f = 0;
                break;

            case 1:
                int n;
                printf("Enter new node: ");
                scanf("%d", &n);
                add(root, n);
                break;

            case 2:
                int k;
                printf("Enter nod to delete: ");
                scanf("%d", &k);
                if (root->number == k && root->right == NULL && root->left == NULL){
                    free(root);
                    int num;
                    printf("You have empty tree, please, enter root: ");
                    scanf("%d", &num);
                    Node *root = create(num);
                }else{delete(root, NULL, k);}
                break;
            case 3:
                if (root == NULL){
                    printf("Empty tree\n");
                }else{print(root, 0, ' ');}
                break;
            
            case 4:
                chek_avl(root);
                break;
            
            default:
                break;
            
        }
    }
    delete_free(root);
    
}