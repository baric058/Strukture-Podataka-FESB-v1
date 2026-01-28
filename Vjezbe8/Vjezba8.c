/*
Napisati program koji omoguæava rad s binarnim stablom pretraživanja. Treba
omoguæiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1

struct treeNode;
typedef struct treeNode* TreePosition;
typedef struct treeNode {
	int data;
	TreePosition left;
	TreePosition right;
} TreeNode;

struct queueNode;
typedef struct queueNode* QueuePosition;
typedef struct queueNode {
	TreePosition treeNode;
	QueuePosition next;
} QueueNode;

QueuePosition rear = NULL;

TreePosition findMin(TreePosition root);
TreePosition deleteElement(TreePosition root, int x);
int deleteTree(TreePosition root);
TreePosition insert(int x, TreePosition root);
TreePosition search(int x, TreePosition root);
int printPreorder(TreePosition root, int level);
int printInorder(TreePosition root, int level);
int printPostorder(TreePosition root, int level);
QueuePosition enqueue(TreePosition levelItem);
TreePosition dequeue();
int printLevelorder(TreePosition root);

int main()
{
	TreePosition root = NULL;

	root = insert(4, root);
	root = insert(9, root);
	root = insert(2, root);
	root = insert(10, root);
	root = insert(6, root);
	root = insert(3, root);
	root = insert(1, root);
	root = insert(5, root);
	root = insert(7, root);

	search(6, root);
	search(11, root);

	printf("Preorder ispis:\n");
	printPreorder(root, 0);

	printf("Inorder ispis:\n");
	printInorder(root, 0);

	printf("Postorder ispis:\n");
	printPostorder(root, 0);

	printf("Levelorder ispis:\n");
	printLevelorder(root);

	root = deleteElement(root, 7);
	root = deleteElement(root, 6);
	printf("Preorder ispis nakon brisanja:\n");
	printPreorder(root, 0);

	deleteTree(root);

	return EXIT_SUCCESS;
}

int deleteTree(TreePosition root) {

	if (root == NULL) return EXIT_SUCCESS;

	deleteTree(root->left);
	deleteTree(root->right);
	free(root);

	return EXIT_SUCCESS;
}

TreePosition findMin(TreePosition root) {
	if (root->left == NULL) return root;
	return findMin(root->left);
}

TreePosition deleteElement(TreePosition root, int x)
{
	if (root == NULL) {
		printf("Element not found.\n");
		return NULL;
	}

	if (x < root->data) root->left = deleteElement(root->left, x);
	else if (x > root->data) root->right = deleteElement(root->right, x);

	// Element found
	// Has two children
	else if (root->left != NULL && root->right != NULL) {
		TreePosition minInRight = findMin(root->right);
		root->data = minInRight->data;
		root->right = deleteElement(root->right, root->data);
	}

	// Has 0 or 1 child
	else {
		TreePosition temp = root;

		if (root->left == NULL) root = root->right; // Doesn't have left child (has only right)
		else root = root->left; // Has only left child

		free(temp);
	}

	return root;
}

TreePosition search(int x, TreePosition root)
{
	if (root == NULL) {
		printf("%d not found.\n", x);
		return NULL;
	}

	if (x < root->data)
		return search(x, root->left);

	else if (x > root->data)
		return search(x, root->right);

	printf("%d found!\n", x);
	return root;
}

TreePosition insert(int x, TreePosition root)
{
	// Empty tree => set as root element
	if (root == NULL)
	{
		root = (TreePosition)malloc(sizeof(TreeNode));
		if (root == NULL) {
			printf("Malloc error.\n");
			return NULL;
		}

		root->data = x;
		root->left = NULL;
		root->right = NULL;
	}

	// Element is smaller than root element => add it to left child
	else if (x < root->data)
		root->left = insert(x, root->left);

	// Element is larger than root element => add it to right child
	else if (x > root->data)
		root->right = insert(x, root->right);

	return root;
}

// opposite of directory print
// 1. recursive print of left child, 2. recursive print of right child, 3. print node
int printPostorder(TreePosition root, int level)
{
	int i = 0;

	if (root->left != NULL) printPostorder(root->left, level + 1);
	if (root->right != NULL) printPostorder(root->right, level + 1);

	for (i; i < level; i++) printf("   ");
	printf("%d\n", root->data);

	return 0;
}

// 1. recursive print of left child, 2. print node, 3. recursive print of right child
int printInorder(TreePosition root, int level)
{
	int i = 0;

	if (root->left != NULL) printInorder(root->left, level + 1);

	for (i; i < level; i++) printf("   ");
	printf("%d\n", root->data);

	if (root->right != NULL) printInorder(root->right, level + 1);

	return 0;
}

// 1. print node, 2. recursive print of left child, 3. recursive print of right child
int printPreorder(TreePosition root, int level)
{
	int i = 0;

	for (i; i < level; i++) printf("   ");
	printf("%d\n", root->data);

	if (root->left != NULL) printPreorder(root->left, level + 1);
	if (root->right != NULL) printPreorder(root->right, level + 1);

	return 0;
}

TreePosition dequeue()
{
	if (rear == NULL) {
		return NULL;
	}

	TreePosition levelItem;
	if (rear == rear->next) {
		levelItem = rear->treeNode;
		free(rear);
		rear = NULL;
	}
	else {
		QueuePosition front = rear->next;
		levelItem = front->treeNode;
		rear->next = front->next;
		free(front);
	}

	return levelItem;
}

QueuePosition enqueue(TreePosition treeNode)
{
	QueuePosition queueNode;
	queueNode = (QueuePosition)malloc(sizeof(QueueNode));
	if (queueNode == NULL) {
		printf("Malloc error.\n");
		return NULL;
	}
	queueNode->treeNode = treeNode;

	if (rear == NULL) {
		queueNode->next = queueNode;
		return queueNode;
	}

	queueNode->next = rear->next;
	rear->next = queueNode;
	rear = queueNode;

	return queueNode;
}

int printLevelorder(TreePosition root)
{
	rear = enqueue(root);

	if (rear == NULL) return MALLOC_ERROR;

	while (rear != NULL) {
		TreePosition treeNode = dequeue();
		printf("%d ", treeNode->data);
		if (treeNode->left != NULL) rear = enqueue(treeNode->left);
		if (treeNode->right != NULL) rear = enqueue(treeNode->right);
	}

	printf("\n");

	return 0;
}
