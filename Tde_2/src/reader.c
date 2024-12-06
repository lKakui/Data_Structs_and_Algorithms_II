#include <stdlib.h>
#include <string.h>

#include "../include/reader.h"

// Function to create a new node
struct BTreeNode *createNode(bool is_leaf) {
    struct BTreeNode *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i < N; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a full child node
void splitChild(struct BTreeNode *parent, int index) {
    struct BTreeNode *child = parent->children[index];
    struct BTreeNode *newNode = createNode(child->is_leaf);
    
    newNode->num_keys = N/2 - 1;
    
    // Move keys and children to the new node
    for (int i = 0; i < N/2 - 1; i++) {
        newNode->keys[i] = child->keys[i + N/2];
        newNode->address[i] = child->address[i + N/2];
    }
    
    if (!child->is_leaf) {
        for (int i = 0; i < N/2; i++) {
            newNode->children[i] = child->children[i + N/2];
     
        }
    }
    
    child->num_keys = N/2 - 1;

    // Shift parent's children to make space for the new node
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    } 
    
    parent->children[index + 1] = newNode; 
    
    // Shift parent's keys to insert the middle key from the child
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
        parent->address[i + 1] = parent->address[i];
    } 
    
    parent->keys[index] = child->keys[N/2 - 1];
    parent->address[index] = child->address[N/2 - 1];
    parent->num_keys++; 
}

// Function to insert a key into a non-full node
void insertNonFull(struct BTreeNode *node, PRODUCT *origin, int adress) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // Insert key into the sorted order
        while (i >= 0 && node->keys[i] > origin->product_id) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = origin->product_id;
        node->address[i+1] = adress;
        node->num_keys++;
    } else {
        // Find the child to insert the key
        while (i >= 0 && node->keys[i] > origin->product_id) {
            i--;
        }
        i++;
        
        if (node->children[i]->num_keys == N - 1) {
            // Split child if it's full
            splitChild(node, i);
            
            // Determine which of the two children is the new one
            if (node->keys[i] < origin->product_id) {
                i++;
            }
        }
        insertNonFull(node->children[i], origin, adress);
    }
}

// Function to insert a key into the B-tree
void insert(struct BTreeNode **root, PRODUCT *origin, int adress) {
    struct BTreeNode *node = *root;

    if (node == NULL) {
        // Create a new root node
        *root = createNode(true);
        (*root)->keys[0] = origin->product_id;
        (*root)->address[0] = adress;
        (*root)->num_keys = 1;
    } else {
        if (node->num_keys == N - 1) {
            // Split the root if it's full
            struct BTreeNode *new_root = createNode(false);
            new_root->children[0] = node;
            splitChild(new_root, 0);
            *root = new_root;
        }
        insertNonFull(*root, origin,adress);
    }
}

// Function to traverse and print the B-tree in-order
void traverse(struct BTreeNode *root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->num_keys; i++) {
            traverse(root->children[i]);
            printf("\nposicao %d", i);
            printf("\nNode:");
            printf("\nid: %lld ", root->keys[i]);
            printf("\nendereco: %d", root->address[i]);
            printf("\n------------------------");

        }
        traverse(root->children[i]);
    }
}

int read_file(FILE *input, size_t model){
    BTREE *root = NULL;

    int position = 0;

    void *buffer = malloc(model);

    while ((fread(buffer, model, 1, input) == 1) && (position < 5)){

        PRODUCT *read = (PRODUCT*)buffer;

        insert(&root, read, position);

        position++;

        fseek(input,-4,SEEK_CUR);
    }

    printf("\nIn-order traversal of the B-tree: ");
    traverse(root);

    return 1;
}