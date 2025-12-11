/* Napisati program koji iz datoteke èita postfiks
izraz i zatim korištenjem stoga raèuna rezultat.
Stog je potrebno realizirati preko vezane liste.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define INVALID_INPUT 2
#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define FILE_OPEN_ERROR -2
#define SCANF_ERROR -3

typedef struct stackNode {
	float number;              
	struct stackNode* nextNode;
} Node;

typedef struct {
	Node* topElement;              
} Stack;

int pushToStack(Stack* s, float val);
int popFromStack(Stack* s, float* result);

int main()
{
	int charsRead = 0;
	float operand1 = 0, operand2 = 0, finalResult = 0;
	char token[10] = { 0 };                    
	char expression[MAX_LINE] = { 0 };         
	char* exprPtr = expression;                
	FILE* fp = NULL;
	Stack myStack = { .topElement = NULL };    

	fp = fopen("postfix.txt", "r");
	if (fp == NULL) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	fgets(expression, MAX_LINE, fp);
	printf("Postfix: %s\n", expression);

	// Process each token in postfix expression
	while (strlen(exprPtr) > 0)
	{
		if (sscanf(exprPtr, " %s %n ", token, &charsRead) != 1) {
			printf("sscanf error.\n");
			return SCANF_ERROR;
		}

		if (token[0] >= '0' && token[0] <= '9') {
			if (pushToStack(&myStack, atof(token))) return MALLOC_ERROR;
		}

		// Check if token is an operator
		else if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')
		{
			if (popFromStack(&myStack, &operand2) || popFromStack(&myStack, &operand1)) {
				printf("Trying to read from empty stack.\n");
				return EMPTY_STACK;
			}

			if (token[0] == '+') {
				if (pushToStack(&myStack, operand1 + operand2)) return MALLOC_ERROR;
			}

			else if (token[0] == '-') {
				if (pushToStack(&myStack, operand1 - operand2)) return MALLOC_ERROR;
			}

			else if (token[0] == '*') {
				if (pushToStack(&myStack, operand1 * operand2)) return MALLOC_ERROR;
			}

			else {
				if (pushToStack(&myStack, operand1 / operand2)) return MALLOC_ERROR;
			}
		}

		else {
			printf("Invalid character in postfix.\n");
			return INVALID_INPUT;
		}

		exprPtr += charsRead;  // Move pointer forward
	}

	if (popFromStack(&myStack, &finalResult)) return EMPTY_STACK;

	printf("Result: %g\n", finalResult);

	return 0;
}

int pushToStack(Stack* s, float val)
{
	Node* newItem = NULL;
	newItem = (Node*)malloc(sizeof(Node));
	if (newItem == NULL) {
		printf("Malloc error!\n");
		return MALLOC_ERROR;
	}

	newItem->number = val;           

	newItem->nextNode = s->topElement;
	s->topElement = newItem;          

	return EXIT_SUCCESS;
}

int popFromStack(Stack* s, float* result)
{
	Node* topNode;
	topNode = s->topElement;        
	if (topNode == NULL) {
		printf("Trying to read from empty stack.\n");
		return EMPTY_STACK;
	}

	*result = topNode->number;      

	s->topElement = topNode->nextNode; 
	free(topNode);                      

	return EXIT_SUCCESS;
}