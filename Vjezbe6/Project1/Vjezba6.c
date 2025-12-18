/*
6. Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu
naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni
artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.
*/

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3


struct Article;
typedef struct Article* ArticlePosition;
typedef struct Article {
	char name[20];
	int quantity;
	double price;
	ArticlePosition next;
} Article;

struct Receipt;
typedef struct Receipt* ReceiptPosition;
typedef struct Receipt {
	char date[11];
	ArticlePosition headArticle; 
	ReceiptPosition next;
}Receipt;

ArticlePosition CreateArticle(const char* name, int quanity, double price);
ReceiptPosition CreateReceipt(const char* date);
int InsertArticleSorted(ArticlePosition head, ArticlePosition newArticle);
int InsertReceiptSorted(ReceiptPosition head, ReceiptPosition newReceipt);
ReceiptPosition ProcessReceipt(const char* filename);
int ReadAllReceipts(const char* mainFile, ReceiptPosition head);
int Query(ReceiptPosition);


int main() {
	Receipt receiptHead = { .next = NULL };

	ReadAllReceipts("racuni.txt", &receiptHead);

	ReceiptPosition currentReceipt = receiptHead.next;
	while (currentReceipt != NULL) {
		printf("\nReceipt Date: %s\n", currentReceipt->date);
		printf("  Articles:\n");

		ArticlePosition currentArticle = currentReceipt->headArticle;
		while (currentArticle != NULL) {
			printf("    %-20s Qty: %2d  Price: %6.2f  Total: %7.2f\n",
				currentArticle->name,
				currentArticle->quantity,
				currentArticle->price,
				currentArticle->quantity * currentArticle->price);
			currentArticle = currentArticle->next;
		}

		currentReceipt = currentReceipt->next;
	}

	char choice;
	do {
		Query(&receiptHead);

		printf("\nDo another query? (y/n): ");
		scanf(" %c", &choice);

	} while (choice == 'y' || choice == 'Y');


	currentReceipt = receiptHead.next;
	while (currentReceipt != NULL) {

		ArticlePosition currentArticle = currentReceipt->headArticle;

		while (currentArticle != NULL) {

			ArticlePosition tempArticle = currentArticle;
			currentArticle = currentArticle->next;

			free(tempArticle);
		}

		ReceiptPosition tempReceipt = currentReceipt;
		currentReceipt = currentReceipt->next;
		free(tempReceipt);
	}

	return EXIT_SUCCESS;
}
ArticlePosition CreateArticle(const char* name, int quanity, double price) {
	ArticlePosition newArticle = (ArticlePosition)malloc(sizeof(Article));
	if (!newArticle) {
		printf("Memory allocation failed! \n");
		return NULL; 
	}
	
	strcpy(newArticle->name, name);
	newArticle->quantity = quanity;
	newArticle->price = price;

	newArticle->next = NULL;

	return newArticle;
}

ReceiptPosition CreateReceipt(const char* date) {
	ReceiptPosition newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (!newReceipt) {
		printf("Memory allocation failed! \n");
		return NULL;
	}

	strcpy(newReceipt->date, date);
	newReceipt->headArticle = NULL;
	newReceipt->next = NULL;

	return newReceipt;
}

int InsertArticleSorted(ArticlePosition head, ArticlePosition newArticle) {

	if (!newArticle) {
		return MALLOC_ERROR; 
	}
	
	ArticlePosition current = head; 
	while (current->next != NULL && strcmp(current->next->name, newArticle->name) < 0) {
		current = current->next; 
	}

	newArticle->next = current->next;
	current->next = newArticle;

	return EXIT_SUCCESS;
}

int InsertReceiptSorted(ReceiptPosition head, ReceiptPosition newReceipt) {
	if (!newReceipt) {
		printf("Cannot insert NULL receipit! \n}");
		return MALLOC_ERROR;
	}

	ReceiptPosition current = head; 

	while (current->next != NULL && strcmp(current->next->date, newReceipt->date) < 0) {
		current = current->next; 
	}

	newReceipt->next = current->next; 
	current->next = newReceipt; 

	return EXIT_SUCCESS; 
}

ReceiptPosition ProcessReceipt(const char* filename) {
	FILE* file = fopen(filename, "r"); 
	if (!file) {
		printf("Error: Cannot open file! \n");
		return NULL; 
	}

	char date[11]; 
	
	if (fscanf(file, "%10s", date) != 1) {
		printf("Error: Cannot open file! \n");
		fclose(file);
		return NULL;
	}

	ReceiptPosition newReceipt = CreateReceipt(date);
	if (!newReceipt) {
		fclose(file);
		return NULL;
	}

	Article articleHead = { .next = NULL };

	char articleName[20];
	int quanity;
	double price;

	while (fscanf(file, " %19[^,],%d,%lf", articleName, &quanity, &price) == 3) {
		ArticlePosition newArticle = CreateArticle(articleName, quanity, price); 
		if (newArticle) {
			InsertArticleSorted(&articleHead, newArticle);
		}
	}

	newReceipt->headArticle = articleHead.next;

	fclose(file); 
	return newReceipt; 


}

int ReadAllReceipts(const char* mainFile, ReceiptPosition head) {
	FILE* file = fopen(mainFile, "r"); 
	if (!file) {
		printf("Error: Cannot open %s \n", mainFile);

		return FILE_OPEN_ERROR; 
	}

	char filename[MAX_LINE]; 

	while (fscanf(file, "%s", filename) == 1) {
		printf("Processing %s \n", filename); 
		
		ReceiptPosition newReceipt = ProcessReceipt(filename);
		if (newReceipt) {
			InsertReceiptSorted(head, newReceipt); 
		}
	}

	fclose(file);
	return EXIT_SUCCESS; 
}


int Query(ReceiptPosition head) {

	char articleName[20];
	char startDate[11];
	char endDate[11];

	printf("Enter article name:");
	scanf("%19s", articleName);

	printf("Enter start date (YYYY-MM-DD):");
	scanf("%10s", startDate);

	printf("Enter end date(YYYY-MM-DD):");
	scanf("%10s", endDate);

	int totalQty = 0;
	double totalSpent = 0.0;

	ReceiptPosition currentReceipt = head->next;

	while (currentReceipt != NULL) {
		if (strcmp(currentReceipt->date, startDate) >= 0 && strcmp(currentReceipt->date, endDate) <= 0) {

			ArticlePosition currentArticle = currentReceipt->headArticle;

			while (currentArticle != NULL) {
				if (strcmp(currentArticle->name, articleName) == 0) {
					int qty = currentArticle->quantity;
					double cost = qty * currentArticle->price;

					totalQty += qty;
					totalSpent += cost;
				}
				currentArticle = currentArticle->next;
			}

		}
		currentReceipt = currentReceipt->next;
	}

	printf("Article: %s\n", articleName);
	printf("Date range: %s to %s\n", startDate, endDate);
	printf("Total quantity purchased: %d\n", totalQty);
	printf("Total money spent: %.2f\n", totalSpent);

	return EXIT_SUCCESS;

}
