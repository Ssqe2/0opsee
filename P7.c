#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

// ������
#define MAX_LEVEL 6

// ����ڵ�ṹ
typedef struct SkipNode {
    int value;
    struct SkipNode **forward; // ָ�����飬ָ��ÿһ�����һ���ڵ�
} SkipNode;

// ����ṹ
typedef struct SkipList {
    int level; // ��ǰ����Ĳ���
    struct SkipNode *header; // �����ͷ�ڵ�
} SkipList;

// ����һ���µĽڵ�
SkipNode* createNode(int level, int value) {
    SkipNode *newNode = (SkipNode *)malloc(sizeof(SkipNode));
    newNode->value = value;
    newNode->forward = (SkipNode **)malloc(sizeof(SkipNode *) * (level + 1));
    for (int i = 0; i <= level; i++) {
        newNode->forward[i] = NULL;
    }
    return newNode;
}

// ����һ���µ�����
SkipList* createList() {
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = createNode(MAX_LEVEL, INT_MIN); // ͷ�ڵ�ֵ��Ϊ��Сֵ
    return list;
}

// ����һ���������
int randomLevel() {
    int level = 0;
    while (rand() % 2 && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

// ����һ��Ԫ�ص�������
void insertElement(SkipList *list, int value) {
    SkipNode *update[MAX_LEVEL + 1];
    SkipNode *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->value < value) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];

    if (x == NULL || x->value != value) {
        int lvl = randomLevel();
        if (lvl > list->level) {
            for (int i = list->level + 1; i <= lvl; i++) {
                update[i] = list->header;
            }
            list->level = lvl;
        }

        x = createNode(lvl, value);
        for (int i = 0; i <= lvl; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
}

// ɾ�������е�һ��Ԫ��
void deleteElement(SkipList *list, int value) {
    SkipNode *update[MAX_LEVEL + 1];
    SkipNode *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->value < value) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];

    if (x != NULL && x->value == value) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->forward[i] != x) break;
            update[i]->forward[i] = x->forward[i];
        }
        free(x);
        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }
    }
}

// ���������е�һ��Ԫ��
SkipNode* searchElement(SkipList *list, int value) {
    SkipNode *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->value < value) {
            x = x->forward[i];
        }
    }
    x = x->forward[0];
    if (x != NULL && x->value == value) {
        return x;
    } else {
        return NULL;
    }
}

// ��ӡ����
void printList(SkipList *list) {
    printf("Skip List Levels:\n");
    for (int i = list->level; i >= 0; i--) {
        SkipNode *node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node != NULL) {
            printf("%d ", node->value);
            node = node->forward[i];
        }
        printf("\n");
    }
}

// �ͷ�����
void freeList(SkipList *list) {
    SkipNode *node = list->header;
    while (node != NULL) {
        SkipNode *next = node->forward[0];
        free(node->forward);
        free(node);
        node = next;
    }
    free(list);
}

int main() {
    srand((unsigned)time(NULL));

    SkipList *list = createList();
    
    // �������
    insertElement(list, 3);
    insertElement(list, 6);
    insertElement(list, 7);
    insertElement(list, 9);
    insertElement(list, 12);
    insertElement(list, 19);
    insertElement(list, 17);
    insertElement(list, 26);
    insertElement(list, 21);
    insertElement(list, 25);

    printList(list);

    // ��������
    SkipNode *node = searchElement(list, 3);
    if (node != NULL) {
        printf("Found %d\n", node->value);
    } else {
        printf("Not Found \n");
    }

    // ɾ������
    deleteElement(list, 19);
    printf("After deletion \n");
    printList(list);

    freeList(list);
    return 0;
}

