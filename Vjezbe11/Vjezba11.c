/*
Prepraviti zadatak 10 na naèin da se formira hash tablica država.

Tablica ima 11 mjesta, a funkcija za preslikavanje kljuè raèuna da se zbraja ASCII vrijednost
prvih pet slova države zatim raèuna ostatak cjelobrojnog dijeljenja te vrijednosti s velièinom
tablice.

Države s istim kljuèem se pohranjuju u vezanu listu sortiranu po nazivu države.
Svaki èvor vezane liste sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -3
#define SCANF_ERROR -2
#define FILE_OPEN_ERROR -1

struct cityTree;
typedef struct cityTree* CityTreePosition;
typedef struct cityTree {
	char name[20];
	int population;
	CityTreePosition left;
	CityTreePosition right;
} CityTree;

struct countryList;
typedef struct countryList* CountryListPosition;
typedef struct countryList {
	char name[20];
	CityTreePosition cities;
	CountryListPosition next;
} CountryList;

typedef struct {
	CountryList hashItem[11];
	int size;
} HashTable;

int calculateHash(char*, int);
CityTreePosition insertCityIntoTree(CityTreePosition, CityTreePosition);
int displayCitiesAbovePopulation(CityTreePosition, int);
CountryListPosition searchHashTable(HashTable*, char*);
int insertCountryIntoList(CountryListPosition, CountryListPosition);

int main()
{
	HashTable hashTable = {
		.hashItem = {
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL },
			{.name = {0}, .cities = NULL, .next = NULL }
		},
		.size = 11
	};

	char countryFilePath[24] = { 0 }, country[20] = { 0 };
	char city[20] = { 0 }, countryNameChoice[20] = { 0 };
	int populationChoice = 0, population = 0;
	FILE* filePointer = NULL;
	FILE* countryFilePointer = NULL;

	filePointer = fopen("drzave.txt", "r");
	if (filePointer == NULL) {
		printf("Could not open file 'drzave.txt'\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(filePointer))
	{
		if (fscanf(filePointer, "%[^,], %s ", country, countryFilePath) != 2) {
			printf("fscanf error.\n");
			return SCANF_ERROR;
		}

		CountryListPosition newCountryList = (CountryListPosition)malloc(sizeof(CountryList));
		if (newCountryList == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}

		strcpy(newCountryList->name, country);
		newCountryList->next = NULL;
		newCountryList->cities = NULL;

		int countryHash = calculateHash(country, hashTable.size);
		if (hashTable.hashItem[countryHash].next == NULL) {
			hashTable.hashItem[countryHash].next = newCountryList;
		}
		else {
			CountryListPosition current = &hashTable.hashItem[countryHash];
			while (current->next != NULL && strcmp(current->next->name, country) < 0)
				current = current->next;
			newCountryList->next = current->next;
			current->next = newCountryList;
		}

		countryFilePointer = fopen(countryFilePath, "r");
		while (!feof(countryFilePointer))
		{
			if (fscanf(countryFilePointer, "%[^,], %d ", city, &population) != 2) {
				printf("fscanf error.\n");
				return SCANF_ERROR;
			}

			CityTreePosition newCityTree = (CityTreePosition)malloc(sizeof(CityTree));
			if (newCityTree == NULL) {
				printf("malloc error.\n");
				return MALLOC_ERROR;
			}

			strcpy(newCityTree->name, city);
			newCityTree->population = population;
			newCountryList->cities = insertCityIntoTree(newCountryList->cities, newCityTree);
		}

		fclose(countryFilePointer);
	}

	fclose(filePointer);

	while (1)
	{
		printf("Choose country: ");
		if (scanf(" %19[^\n]", countryNameChoice) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}

		if ((strlen(countryNameChoice) > 0) && (countryNameChoice[strlen(countryNameChoice) - 1] == '\n'))
			countryNameChoice[strlen(countryNameChoice) - 1] = '\0';

		if (!strcmp(countryNameChoice, "0"))
			break;

		CountryListPosition countryChoice = searchHashTable(&hashTable, countryNameChoice);
		if (countryChoice == NULL) {
			printf("No such country.\n");
			continue;
		}

		printf("Enter population: ");
		if (scanf(" %d", &populationChoice) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}

		printf("Cities in %s with population greater than %d:\n", countryNameChoice, populationChoice);
		displayCitiesAbovePopulation(countryChoice->cities, populationChoice);
	}

	return EXIT_SUCCESS;
}

int calculateHash(char* country, int tableSize)
{
	int sum = 0, i = 0;

	for (i = 0; i < 5; i++)
		sum += country[i];

	return sum % tableSize;
}

CityTreePosition insertCityIntoTree(CityTreePosition current, CityTreePosition newCity)
{
	if (current == NULL) {
		current = newCity;
		current->right = NULL;
		current->left = NULL;
	}

	if (newCity->population < current->population)
		current->left = insertCityIntoTree(current->left, newCity);
	else if (newCity->population > current->population)
		current->right = insertCityIntoTree(current->right, newCity);
	else {
		if (strcmp(newCity->name, current->name) < 0)
			current->left = insertCityIntoTree(current->left, newCity);
		else if (strcmp(newCity->name, current->name) > 0)
			current->right = insertCityIntoTree(current->right, newCity);
	}

	return current;
}

int displayCitiesAbovePopulation(CityTreePosition root, int population)
{
	if (root == NULL)
		return NULL;

	if (root->population > population)
		printf("\t%s, %d\n", root->name, root->population);

	displayCitiesAbovePopulation(root->left, population);
	displayCitiesAbovePopulation(root->right, population);
}

CountryListPosition searchHashTable(HashTable* hashTable, char* countryName)
{
	CountryListPosition current = hashTable->hashItem[calculateHash(countryName, hashTable->size)].next;

	while (current != NULL && strcmp(current->name, countryName))
		current = current->next;

	return current;
}

int insertCountryIntoList(CountryListPosition current, CountryListPosition newCountry)
{
	while (current->next != NULL && strcmp(newCountry->name, current->next->name) > 0)
		current = current->next;

	newCountry->next = current->next;
	current->next = newCountry;

	return EXIT_SUCCESS;
}