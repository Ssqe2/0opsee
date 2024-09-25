#ifndef P7_H
#define P7_H
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
// 跳表节点结构
typedef struct MySkipNode {
    int value;
    struct MySkipNode **forward;
} MySkipNode;

// 跳表结构
typedef struct MySkipList {
    int level;
    MySkipNode *header;
} MySkipList;

// 创建跳表
MySkipList* createNewList(int MAX_LEVEL);

// 插入元素到跳�?
void insertNewElement(MySkipList *list, int value,int MAX_LEVEL);

// 删除跳表中的元素
void deleteNewElement(MySkipList *list, int value,int MAX_LEVEL);

// 查找跳表中的元素
MySkipNode* searchNewElement(MySkipList *list, int value);

// 打印跳表内容（用于调试）
void printNewList(MySkipList *list);

// 释放跳表内存
void freeNewList(MySkipList *list);

#endif /* P7_H */
