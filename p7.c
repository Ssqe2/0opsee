#include "p7.h"  // Include the header file "p7.h"


// 创建一个新的跳表节点
MySkipNode* createNewNode(int level, int value) {
    MySkipNode *newNode = (MySkipNode *)malloc(sizeof(MySkipNode));  // Allocate memory for a new skip list node
    newNode->value = value;  // Set the value of the new node
    newNode->forward = (MySkipNode **)malloc(sizeof(MySkipNode *) * (level + 1));  // Allocate memory for the forward pointers
    for (int i = 0; i <= level; i++) {
        newNode->forward[i] = NULL;  // Initialize forward pointers to NULL
    }
    return newNode;  // Return the new node
}

// 创建一个新的跳表
MySkipList* createNewList(int MAX_LEVEL) {
    MySkipList *list = (MySkipList *)malloc(sizeof(MySkipList));  // Allocate memory for a new skip list
    list->level = 0;  // Initialize the level of the skip list to 0
    list->header = createNewNode(MAX_LEVEL, INT_MIN);  // Initialize the header node with the maximum level and minimum integer value
    return list;  // Return the new skip list
}

// 生成一个随机层数
int getRandomLevel(int MAX_LEVEL) {
    int level = 0;  // Initialize the level to 0
    while (rand() % 2 && level < MAX_LEVEL) {
        level++;  // Increment the level while a random condition is met and the level is less than the maximum level
    }
    return level;  // Return the generated level
}

// 插入一个元素到跳表中
void insertNewElement(MySkipList *list, int value,int MAX_LEVEL) {
    MySkipNode *update[MAX_LEVEL + 1];  // Create an array to store update pointers
    MySkipNode *x = list->header;  // Start from the header node
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->value < value) {
            x = x->forward[i];  // Move forward while the next node's value is less than the value to be inserted
        }
        update[i] = x;  // Store the pointer at the current level
    }
    x = x->forward[0];  // Move to the next node at level 0

    if (x == NULL || x->value != value) {
        int lvl = getRandomLevel(MAX_LEVEL);  // Generate a random level for the new node
        if (lvl > list->level) {
            for (int i = list->level + 1; i <= lvl; i++) {
                update[i] = list->header;  // Update the header pointers for the new levels
            }
            list->level = lvl;  // Update the list's level
        }

        x = createNewNode(lvl, value);  // Create a new node with the generated level and value
        for (int i = 0; i <= lvl; i++) {
            x->forward[i] = update[i]->forward[i];  // Update the forward pointers of the new node
            update[i]->forward[i] = x;  // Update the forward pointers of the previous nodes
        }
    }
}

// 删除跳表中的一个元素
void deleteNewElement(MySkipList *list, int value,int MAX_LEVEL) {
    MySkipNode *update[MAX_LEVEL + 1];  // Create an array to store update pointers
    MySkipNode *x = list->header;  // Start from the header node
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->value < value) {
            x = x->forward[i];  // Move forward while the next node's value is less than the value to be deleted
        }
        update[i] = x;  // Store the pointer at the current level
    }
    x = x->forward[0];  // Move to the next node at level 0

    if (x != NULL && x->value == value) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->forward[i] != x) break;  // Break if the node to be deleted is not at the current level
            update[i]->forward[i] = x->forward[i];  // Update the forward pointers to skip the node to be deleted
        }
        free(x);  // Free the memory of the deleted node
        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;  // Decrease the level of the list if the highest level is empty
        }
    }
}

// 搜索跳表中的一个元素
MySkipNode* searchNewElement(MySkipList *list, int value) {
  //  printNewList(list);
    MySkipNode *x = list->header;  // Start from the header node
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->value < value) {
            x = x->forward[i];  // Move forward while the next node's value is less than the value to be searched
           // printf("%d\n",x->value);
        }
    }
    x = x->forward[0];  // Move to the next node at level 0
 //   printf("%d\n",x->value);
    if (x != NULL && x->value == value) {
        return x;  // Return the node if found
    } else {
        printf("not found\n");
        return NULL;  // Return NULL if not found
    }
}

// 打印跳表的内容
void printNewList(MySkipList *list) {
    printf("Skip List Levels:\n");  // Print the header for the skip list levels
    for (int i = list->level; i >= 0; i--) {
        MySkipNode *node = list->header->forward[i];  // Start from the header node at the current level
        printf("Level %d: ", i);  // Print the current level
        while (node != NULL) {
            printf("%d ", node->value);  // Print the value of the node
            node = node->forward[i];  // Move to the next node at the current level
        }
        printf("\n");  // Print a newline after each level
    }
}

// 释放跳表所占的内存
void freeNewList(MySkipList *list) {
    MySkipNode *node = list->header;  // Start from the header node
    while (node != NULL) {
        MySkipNode *next = node->forward[0];  // Store the next node
        free(node->forward);  // Free the forward pointers
        free(node);  // Free the node
        node = next;  // Move to the next node
    }
    free(list);  // Free the skip list
}
