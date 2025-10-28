/*
1. Napisati program koji prvo proèita koliko redaka ima datoteka, tj.koliko ima studenata
zapisanih u datoteci.Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata(ime, prezime, bodovi) i uèitati iz datoteke sve zapise.Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova / max_br_bodova * 100
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 50
#define MAX_POINTS 20
#define BUF_SIZE 256




typedef struct{
	char Name [MAX_SIZE];
	char Surrname[MAX_SIZE];
	double Points; 
} Student;




int main() {
	int no_Rows=0;
	double Relative_Points;

	FILE* pF = fopen("students.txt", "r");
	char buffer[BUF_SIZE];

	if (pF == NULL) {
		printf("Unable to open file!\n ");
	}
	else {
		while (fgets(buffer, BUF_SIZE, pF) != NULL) {
			//printf("%s", buffer);
			no_Rows++;
		}
		rewind(pF);
	}

	//printf("%d", no_Rows);

	Student* Stud = NULL; 
	Stud = (Student*)malloc(no_Rows * sizeof(Student)); 
	
	for(int i = 0; i < no_Rows; i++) {
		fscanf(pF, " %s %s %lf ", Stud[i].Name, Stud[i].Surrname, &Stud[i].Points); 
	}

	for (int i = 0; i < no_Rows; i++) {
		Relative_Points = (Stud[i].Points / MAX_POINTS) * 100; 
		printf(" %s %s %lf %f\n", Stud[i].Name, Stud[i].Surrname, Stud[i].Points, Relative_Points);
	}


	fclose(pF);
	free(Stud);

	return 0;
}