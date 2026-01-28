#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

struct node;
typedef struct node* Position;
typedef struct node {
	int data;
	Position left;
	Position right;
} Node;

Position insert(Position root, int x);
int replace(Position root);
int writeInorderToFile(Position root, int level, FILE* filePointer);
int deleteTree(Position root);

int main()
{
	srand(time(NULL));

	Position root = NULL;
	FILE* filePointer = NULL;

	filePointer = fopen("inorder.txt", "w");
	if (filePointer == NULL) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	for (int counter = 0; counter < 10; counter++) {
		int randomValue = rand() % (91 - 10) + 10;
		root = insert(root, randomValue);
		if (root == NULL) {
			printf("Malloc error.\n");
			return MALLOC_ERROR;
		}
	}

	fprintf(filePointer, "Original tree:\n");
	writeInorderToFile(root, 0, filePointer);

	replace(root);

	fprintf(filePointer, "\nTree after replace():\n");
	writeInorderToFile(root, 0, filePointer);

	deleteTree(root);
	fclose(filePointer);

	return 0;
}

Position insert(Position root, int x)
{
	if (root == NULL) {

		root = (Position)malloc(sizeof(Node));
		if (root == NULL) return NULL;

		root->left = NULL;
		root->right = NULL;
		root->data = x;
	}

	else if (x < root->data) {
		root->left = insert(root->left, x);
	}
	else /* x >= root->data */ {
		root->right = insert(root->right, x);
	}

	return root;
}

int replace(Position root)
{
	if (root == NULL) return 0;

	int originalData = root->data;
	int leftSum = replace(root->left);
	int rightSum = replace(root->right);
	root->data = leftSum + rightSum;

	return originalData + root->data;
}

int writeInorderToFile(Position root, int level, FILE* filePointer)
{
	if (root->left != NULL) writeInorderToFile(root->left, level + 1, filePointer);

	for (int indent = 0; indent < level; indent++) {
		fprintf(filePointer, "   ");
	}
	fprintf(filePointer, "%d\n", root->data);

	if (root->right != NULL) writeInorderToFile(root->right, level + 1, filePointer);

	return 0;
}

int deleteTree(Position root) {

	if (root == NULL) return EXIT_SUCCESS;

	deleteTree(root->left);
	deleteTree(root->right);
	free(root);

	return EXIT_SUCCESS;
}