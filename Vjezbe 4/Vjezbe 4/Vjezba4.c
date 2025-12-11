/* Napisati program za zbrajanje i množenje polinoma.
Koeficijenti i eksponenti se èitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani. */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SIZE 100

#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct monomial {
	int coef;              
	int exp;               
	struct monomial* next; 
} Mono;

int strToPoly(Mono*, char*);
int addMonoToPoly(Mono*, Mono*);
int printMono(Mono*);
int printPoly(Mono*);
int deletePoly(Mono*);
int polySum(Mono*, Mono*, Mono*);
int polyProduct(Mono*, Mono*, Mono*);

int main()
{
	Mono poly1Head = { .coef = 0, .exp = 0, .next = NULL };
	Mono poly2Head = { .coef = 0, .exp = 0, .next = NULL };
	Mono sumHead = { .coef = 0, .exp = 0, .next = NULL };
	Mono productHead = { .coef = 0, .exp = 0, .next = NULL };

	int j = 0, bytesRead = 0;
	char line[MAX_LINE] = { 0 };

	FILE* fp = NULL;
	fp = fopen("polynomials.txt", "r");

	if (fp == NULL) {
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	// read line 1 and convert into poly1
	fgets(line, MAX_LINE, fp);
	strToPoly(&poly1Head, line);

	// read line 2 and covert into poly2
	fgets(line, MAX_LINE, fp);
	strToPoly(&poly2Head, line);

	printf("Polynomial 1: ");
	printPoly(poly1Head.next);
	printf("Polynomial 2: ");
	printPoly(poly2Head.next);

	polySum(&sumHead, poly1Head.next, poly2Head.next);
	printf("Sum: ");
	printPoly(sumHead.next);

	polyProduct(&productHead, poly1Head.next, poly2Head.next);
	printf("Product: ");
	printPoly(productHead.next);

	fclose(fp);

	deletePoly(&poly1Head);
	deletePoly(&poly2Head);
	deletePoly(&sumHead);
	deletePoly(&productHead);

	return EXIT_SUCCESS;
}

int addMonoToPoly(Mono* head, Mono* mono)
{
	Mono* predecessor = head;
	Mono* nodeToRemove = NULL;

	// ax^0 = 0
	if (mono->coef == 0) {
		free(mono);  
		return EXIT_SUCCESS;
	}

	while (predecessor->next != NULL && predecessor->next->exp > mono->exp)
		predecessor = predecessor->next;

	// ax^n + bx^n = (a+b)x^n
	if (predecessor->next != NULL && mono->exp == predecessor->next->exp)
	{
		predecessor->next->coef += mono->coef;
		free(mono); 

		
		if (predecessor->next->coef == 0)
		{
			nodeToRemove = predecessor->next;
			predecessor->next = predecessor->next->next;
			free(nodeToRemove);
		}
	}

	// Insert at correct position
	else {
		mono->next = predecessor->next;
		predecessor->next = mono;
	}

	return EXIT_SUCCESS;
}

int strToPoly(Mono* head, char* str)
{
	int k = 0, charsProcessed = 0;
	Mono* term = NULL;

	while (strlen(str) > 0)
	{
		term = (Mono*)malloc(sizeof(Mono));
		if (term == NULL) return MALLOC_ERROR;

		if (sscanf(str, " %dx^%d %n", &term->coef, &term->exp, &charsProcessed) != 2)
			return SCANF_ERROR;

		addMonoToPoly(head, term);
		str += charsProcessed;
	}

	return EXIT_SUCCESS;
}

int printMono(Mono* mono)
{
	mono->coef > 0 ? printf("%d", mono->coef) : printf("- %d", -mono->coef);

	if (mono->exp == 1) printf("x ");
	else if (mono->exp != 0) printf("x^%d ", mono->exp);

	return EXIT_SUCCESS;
}

int printPoly(Mono* current)
{
	if (current != NULL) {
		printMono(current);
		current = current->next;
	}

	while (current != NULL)
	{
		if (current->coef > 0) printf("+ ");
		printMono(current);
		current = current->next;
	}

	printf("\n");

	return EXIT_SUCCESS;
}

int deletePoly(Mono* head)
{
	Mono* toDelete = NULL;

	while (head->next != NULL)
	{
		toDelete = head->next;
		head->next = toDelete->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

int polySum(Mono* sum, Mono* poly1, Mono* poly2)
{
	Mono* newNode = NULL;

	while (poly1 != NULL)
	{
		newNode = (Mono*)malloc(sizeof(Mono));
		if (newNode == NULL) return MALLOC_ERROR;

		newNode->coef = poly1->coef;
		newNode->exp = poly1->exp;

		addMonoToPoly(sum, newNode);
		poly1 = poly1->next;
	}

	while (poly2 != NULL)
	{
		newNode = (Mono*)malloc(sizeof(Mono));
		if (newNode == NULL) return MALLOC_ERROR;

		newNode->coef = poly2->coef;
		newNode->exp = poly2->exp;

		addMonoToPoly(sum, newNode);  
		poly2 = poly2->next;
	}

	return EXIT_SUCCESS;
}

int polyProduct(Mono* product, Mono* poly1, Mono* poly2)
{
	Mono* productTerm = NULL;
	Mono* secondPolyStart = poly2;  

	while (poly1 != NULL)
	{
		poly2 = secondPolyStart; // Reset poly2 for each term of poly1
		while (poly2 != NULL)
		{
			productTerm = (Mono*)malloc(sizeof(Mono));  
			if (productTerm == NULL) return MALLOC_ERROR;

			productTerm->coef = poly1->coef * poly2->coef;  
			productTerm->exp = poly1->exp + poly2->exp;

			addMonoToPoly(product, productTerm);
			poly2 = poly2->next;
		}
		poly1 = poly1->next;
	}

	return EXIT_SUCCESS;
}