/*
Napisati program koji èita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki èvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo država sortirano po nazivu. Svaki èvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati države i gradove te omoguæiti korisniku putem
tastature pretragu gradova odreðene države koji imaju broj stanovnika veæi od unosa na
tastaturi.
*/


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define SCANF_ERROR -2
#define MALLOC_ERROR -3

struct cityList;
typedef struct cityList* CityListPosition;
typedef struct cityList {
	char name[20];
	int population;
	CityListPosition next;
} CityList;

struct cityTree;
typedef struct cityTree* CityTreePosition;
typedef struct cityTree {
	char name[20];
	int population;
	CityTreePosition left;
	CityTreePosition right;
} CityTree;

struct countryTree;
typedef struct countryTree* CountryTreePosition;
typedef struct countryTree {
	char name[20];
	CityList cityHead;
	CountryTreePosition left;
	CountryTreePosition right;
} CountryTree;

struct countryList;
typedef struct countryList* CountryListPosition;
typedef struct countryList {
	char name[20];
	CityTreePosition cities;
	CountryListPosition next;
} CountryList;

int insertCityIntoList(CityListPosition current, CityListPosition newCity);
CityTreePosition insertCityIntoTree(CityTreePosition current, CityTreePosition newCity);
int insertCountryIntoList(CountryListPosition current, CountryListPosition newCountry);
CountryTreePosition insertCountryIntoTree(CountryTreePosition current, CountryTreePosition newCountry);
int displayCityList(CityListPosition current);
int displayCityTree(CityTreePosition current);
int displayCountryList(CountryListPosition current);
int displayCountryTree(CountryTreePosition current);
CountryTreePosition searchCountryByName(CountryTreePosition current, char* name);

int main()
{
	CountryList countryListHead = { .name = { 0 }, .cities = NULL, .next = NULL };
	CountryTreePosition countryTree = NULL;
	CountryTreePosition selectedCountry = NULL;
	CityListPosition filteredCity = NULL;

	char countryName[20] = { 0 };
	char cityFileName[24] = { 0 };
	char cityName[20] = { 0 };
	char userCountryChoice[20] = { 0 };
	int cityPopulation = 0;
	int userPopulationThreshold = 0;

	FILE* mainFilePointer = NULL;
	FILE* cityFilePointer = NULL;

	mainFilePointer = fopen("drzave.txt", "r");
	if (mainFilePointer == NULL) {
		printf("Could not open file 'drzave.txt'\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(mainFilePointer))
	{
		if (fscanf(mainFilePointer, "%[^,], %s ", countryName, cityFileName) != 2) {
			printf("fscanf error.\n");
			return SCANF_ERROR;
		}

		CountryListPosition newCountryList = NULL;
		newCountryList = (CountryListPosition)malloc(sizeof(CountryList));
		if (newCountryList == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}
		strcpy(newCountryList->name, countryName);
		newCountryList->cities = NULL;

		CountryTreePosition newCountryTree = NULL;
		newCountryTree = (CountryTreePosition)malloc(sizeof(CountryTree));
		if (newCountryTree == NULL) {
			printf("malloc error.\n");
			return MALLOC_ERROR;
		}
		strcpy(newCountryTree->name, countryName);
		newCountryTree->cityHead = (CityList){ .name = { 0 }, .population = 0, .next = NULL };

		insertCountryIntoList(&countryListHead, newCountryList);
		countryTree = insertCountryIntoTree(countryTree, newCountryTree);

		cityFilePointer = fopen(cityFileName, "r");
		while (!feof(cityFilePointer)) {
			if (fscanf(cityFilePointer, "%[^,], %d ", cityName, &cityPopulation) != 2) {
				printf("fscanf error.\n");
				return SCANF_ERROR;
			}

			CityListPosition newCityList = NULL;
			newCityList = (CityListPosition)malloc(sizeof(CityList));
			if (newCityList == NULL) {
				printf("malloc error.\n");
				return MALLOC_ERROR;
			}
			strcpy(newCityList->name, cityName);
			newCityList->population = cityPopulation;

			CityTreePosition newCityTree = NULL;
			newCityTree = (CityTreePosition)malloc(sizeof(CityTree));
			if (newCityTree == NULL) {
				printf("malloc error.\n");
				return MALLOC_ERROR;
			}
			strcpy(newCityTree->name, cityName);
			newCityTree->population = cityPopulation;

			insertCityIntoList(&newCountryTree->cityHead, newCityList);
			newCountryList->cities = insertCityIntoTree(newCountryList->cities, newCityTree);
		}

		fclose(cityFilePointer);
	}

	fclose(mainFilePointer);

	printf("Country list:\n---------------\n");
	displayCountryList(countryListHead.next);
	printf("\nCountry tree:\n---------------\n");
	displayCountryTree(countryTree);

	while (1)
	{
		printf("\nChoose country: ");
		if (scanf(" %19[^\n]", userCountryChoice) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}
		if ((strlen(userCountryChoice) > 0) && (userCountryChoice[strlen(userCountryChoice) - 1] == '\n'))
			userCountryChoice[strlen(userCountryChoice) - 1] = '\0';
		if (!strcmp(userCountryChoice, "0")) break;

		selectedCountry = searchCountryByName(countryTree, userCountryChoice);
		if (selectedCountry == NULL) {
			printf("No such country.\n");
			continue;
		}

		printf("Enter some population to find cities with greater population: ");
		if (scanf(" %d", &userPopulationThreshold) != 1) {
			printf("scanf error.\n");
			return SCANF_ERROR;
		}

		filteredCity = selectedCountry->cityHead.next;
		while (filteredCity != NULL && filteredCity->population <= userPopulationThreshold)
			filteredCity = filteredCity->next;

		if (filteredCity == NULL) printf("No such city.\n");
		else {
			printf("Cities in %s with population greater than %d:\n", userCountryChoice, userPopulationThreshold);
			displayCityList(filteredCity);
		}
	}

	return EXIT_SUCCESS;
}

int insertCityIntoList(CityListPosition current, CityListPosition newCity)
{
	while (current->next != NULL && newCity->population > current->next->population)
		current = current->next;

	if (current->next != NULL && current->next->population == newCity->population) {
		while (current->next != NULL && strcmp(newCity->name, current->next->name) > 0)
			current = current->next;
	}

	newCity->next = current->next;
	current->next = newCity;

	return EXIT_SUCCESS;
}

CityTreePosition insertCityIntoTree(CityTreePosition current, CityTreePosition newCity)
{
	if (current == NULL) {
		current = newCity;
		current->left = NULL;
		current->right = NULL;
		return current;
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

int insertCountryIntoList(CountryListPosition current, CountryListPosition newCountry)
{
	while (current->next != NULL && strcmp(newCountry->name, current->next->name) > 0)
		current = current->next;

	newCountry->next = current->next;
	current->next = newCountry;

	return EXIT_SUCCESS;
}

CountryTreePosition insertCountryIntoTree(CountryTreePosition current, CountryTreePosition newCountry)
{
	if (current == NULL) {
		current = newCountry;
		current->left = NULL;
		current->right = NULL;
		return current;
	}

	if (strcmp(newCountry->name, current->name) < 0)
		current->left = insertCountryIntoTree(current->left, newCountry);
	else if (strcmp(newCountry->name, current->name) > 0)
		current->right = insertCountryIntoTree(current->right, newCountry);

	return current;
}

int displayCityList(CityListPosition current)
{
	while (current != NULL) {
		printf("\t%s, %d\n", current->name, current->population);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int displayCityTree(CityTreePosition current)
{
	if (current == NULL) return EXIT_SUCCESS;

	displayCityTree(current->left);
	printf("\t%s, %d\n", current->name, current->population);
	displayCityTree(current->right);

	return EXIT_SUCCESS;
}

int displayCountryList(CountryListPosition current)
{
	while (current != NULL) {
		printf("%s\n", current->name);
		displayCityTree(current->cities);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int displayCountryTree(CountryTreePosition current)
{
	if (current == NULL) return EXIT_SUCCESS;

	displayCountryTree(current->left);
	printf("%s\n", current->name);
	displayCityList(current->cityHead.next);
	displayCountryTree(current->right);

	return EXIT_SUCCESS;
}

CountryTreePosition searchCountryByName(CountryTreePosition current, char* name)
{
	if (current == NULL) return NULL;

	if (strcmp(name, current->name) > 0)
		return searchCountryByName(current->right, name);
	else if (strcmp(name, current->name) < 0)
		return searchCountryByName(current->left, name);

	return current;
}