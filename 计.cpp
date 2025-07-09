#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_TREE_NODES 15
#define MAX_CODE_LENGTH 256
typedef struct TreeNode {
	char data;
	int freq;
	struct TreeNode *left, *right;
} TreeNode;
typedef struct {
	TreeNode **array;
	int size;
	int capacity;
} MinHeap;
TreeNode* newNode(char data, int freq) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->data = data;
	node->freq = freq;
	node->left = node->right = NULL;
	return node;
}
MinHeap* createMinHeap(int capacity) {
	MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (TreeNode**)malloc(capacity * sizeof(TreeNode*));
	return minHeap;
}
void swapTreeNode(TreeNode** a, TreeNode** b) {
	TreeNode* t = *a;
	*a = *b;
	*b = t;
}
void minHeapify(MinHeap* minHeap, int idx) {
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)smallest = left;
	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)smallest = right;
	if (smallest != idx) {
		swapTreeNode(&minHeap->array[smallest], &minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}
int isSizeOne(MinHeap* minHeap) {
	return (minHeap->size == 1);
}
TreeNode* extractMin(MinHeap* minHeap) {
	TreeNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];
	--minHeap->size;
	minHeapify(minHeap, 0);
	return temp;
}
void insertMinHeap(MinHeap* minHeap, TreeNode* node) {
	++minHeap->size;
	int i = minHeap->size - 1;
	while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	minHeap->array[i] = node;
}
int isLeaf(TreeNode* root) {
	return !(root->left) && !(root->right);
}
MinHeap* buildMinHeap(char data[], int freq[], int size) {
	MinHeap* minHeap = createMinHeap(size);
	for (int i = 0; i < size; ++i)minHeap->array[i] = newNode(data[i], freq[i]);
	minHeap->size = size;
	int i;
	for (i = (size - 2) / 2; i >= 0; --i)minHeapify(minHeap, i);
	return minHeap;
}
TreeNode* buildHuffmanTree(char data[], int freq[], int size) {
	TreeNode *left, *right, *top;
	MinHeap* minHeap = buildMinHeap(data, freq, size);
	while (!isSizeOne(minHeap)) {
		left = extractMin(minHeap);
		right = extractMin(minHeap);
		top = newNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		insertMinHeap(minHeap, top);
	}
	return extractMin(minHeap);
}
void printCodes(TreeNode* root, int arr[], int top) {
	if (root->left) {
		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}
	if (root->right) {
		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}
	if (isLeaf(root)) {
		printf("%c: ", root->data);
		for (int i = 0; i < top; ++i)printf("%d", arr[i]);
		printf("\n");
	}
}
void HuffmanCodes(char data[], int freq[], int size) {
	TreeNode* root = buildHuffmanTree(data, freq, size);
	int arr[MAX_CODE_LENGTH], top = 0;
	printCodes(root, arr, top);
}
void printHuffmanTree(TreeNode* root, int arr[], int top) {
	if (root == NULL)return;
	if (isLeaf(root)) {
		printf("Leaf Node: %c (freq: %d), Code: ", root->data, root->freq);
		for (int i = 0; i < top; i++) {
			printf("%d", arr[i]);
		}
		printf("\n");
	} else {
		printf("Internal Node (freq: %d)\n", root->freq);
	}
	if (root->left) {
		arr[top] = 0;
		printHuffmanTree(root->left, arr, top + 1);
	}
	if (root->right) {
		arr[top] = 1;
		printHuffmanTree(root->right, arr, top + 1);
	}
}
void freeHuffmanTree(TreeNode* root) {
	if (root == NULL)return;
	freeHuffmanTree(root->left);
	freeHuffmanTree(root->right);
	free(root);
}
int main() {
	char arr[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	int freq[] = {23, 5, 17, 4, 9, 31, 29, 18};
	int size = sizeof(arr) / sizeof(arr[0]);
	printf("Huffman编码结果:\n");
	HuffmanCodes(arr, freq, size);
	TreeNode* root = buildHuffmanTree(arr, freq, size);
	int arrPrint[MAX_CODE_LENGTH], top = 0;
	printf("\n哈夫曼树结构 (前序遍历):\n");
	printHuffmanTree(root, arrPrint, top);
	freeHuffmanTree(root);
	return 0;
}
