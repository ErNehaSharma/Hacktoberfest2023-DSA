#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_NODES 256

typedef struct Node {
    unsigned char data;
    int frequency;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct PriorityQueue {
    int size;
    Node *nodes[MAX_TREE_NODES];
} PriorityQueue;

Node* createNode(unsigned char data, int frequency) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

PriorityQueue* createPriorityQueue() {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    return pq;
}

void swap(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(PriorityQueue *pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->nodes[left]->frequency < pq->nodes[smallest]->frequency)
        smallest = left;

    if (right < pq->size && pq->nodes[right]->frequency < pq->nodes[smallest]->frequency)
        smallest = right;

    if (smallest != index) {
        swap(&pq->nodes[index], &pq->nodes[smallest]);
        minHeapify(pq, smallest);
    }
}

void buildMinHeap(PriorityQueue *pq) {
    int n = pq->size;
    for (int i = (n / 2) - 1; i >= 0; i--) {
        minHeapify(pq, i);
    }
}

void insert(PriorityQueue *pq, Node *node) {
    pq->size++;
    int i = pq->size - 1;
    while (i > 0 && node->frequency < pq->nodes[(i - 1) / 2]->frequency) {
        pq->nodes[i] = pq->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->nodes[i] = node;
}

Node* extractMin(PriorityQueue *pq) {
    Node *minNode = pq->nodes[0];
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;
    minHeapify(pq, 0);
    return minNode;
}

Node* buildHuffmanTree(char data[], int frequency[], int size) {
    PriorityQueue *pq = createPriorityQueue();

    for (int i = 0; i < size; i++) {
        insert(pq, createNode(data[i], frequency[i]));
    }

    buildMinHeap(pq);

    while (pq->size > 1) {
        Node *left = extractMin(pq);
        Node *right = extractMin(pq);

        Node *merged = createNode('$', left->frequency + right->frequency);
        merged->left = left;
        merged->right = right;

        insert(pq, merged);
    }

    return extractMin(pq);
}

void encodeHuffman(Node *root, char *code, int depth) {
    if (root->left == NULL && root->right == NULL) {
        printf("%c: %s\n", root->data, code);
        return;
    }

    char leftCode[256], rightCode[256];
    strcpy(leftCode, code);
    strcpy(rightCode, code);

    leftCode[depth] = '0';
    rightCode[depth] = '1';

    encodeHuffman(root->left, leftCode, depth + 1);
    encodeHuffman(root->right, rightCode, depth + 1);
}

void huffmanEncode(char data[], int frequency[], int size) {
    Node *root = buildHuffmanTree(data, frequency, size);
    char code[256];
    encodeHuffman(root, code, 0);
}

int main() {
    char data[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int frequency[] = {5, 9, 12, 13, 16, 45};
    int size = sizeof(data) / sizeof(data[0]);

    printf("Huffman Codes:\n");
    huffmanEncode(data, frequency, size);

    return 0;
}
