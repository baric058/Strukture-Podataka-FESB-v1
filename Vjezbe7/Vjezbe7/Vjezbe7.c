#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

struct directory;
typedef struct directory* DirectoryPosition;
typedef struct directory {
	char name[MAX_LENGTH];
	DirectoryPosition subdirectoryPosition;
	DirectoryPosition next;
} Directory;

struct levelStack;
typedef struct levelStack* LevelStackPosition;
typedef struct levelStack {
	DirectoryPosition directoryLevel;
	LevelStackPosition next;
} LevelStack;

DirectoryPosition createDirectory(char* directoryName);
int push(LevelStackPosition, DirectoryPosition);
int pop(LevelStackPosition);
int addDirectory(DirectoryPosition, DirectoryPosition);
int changeDirectory(LevelStackPosition, char*);
int printDirectory(DirectoryPosition, int);
int deleteDirectory(DirectoryPosition);
int deleteStack(LevelStackPosition);

int main()
{
	char command = 0;
	char folderName[MAX_LENGTH] = { 0 };
	Directory headDirectory = { .name = {0}, .subdirectoryPosition = NULL, .next = NULL };
	DirectoryPosition rootDirectory = NULL;
	LevelStack headLevelStack = { .directoryLevel = NULL, .next = NULL };

	rootDirectory = createDirectory("C:");
	if (rootDirectory == NULL) return MALLOC_ERROR;

	headDirectory.next = rootDirectory;
	push(&headLevelStack, rootDirectory);

	printf("Commands:\nm - create directory\nc - navigate to directory\nb - return to parent\nd - display contents\ne - exit\n");

	do {
		printf("\nEnter command: ");
		if (scanf(" %c", &command) != 1) return SCANF_ERROR;

		switch (command) {
		case 'm':
		case 'M':
			printf("Enter directory name: ");
			if (scanf(" %s", folderName) != 1) return SCANF_ERROR;

			DirectoryPosition newDirectory = createDirectory(folderName);
			if (newDirectory == NULL) return MALLOC_ERROR;
			addDirectory(headLevelStack.next->directoryLevel, newDirectory);
			printf("Directory '%s' created successfully\n", folderName);

			break;
		case 'c':
		case 'C':
			printf("Enter directory name: ");
			if (scanf(" %s", folderName) != 1) return SCANF_ERROR;
			changeDirectory(&headLevelStack, folderName);
			break;
		case 'b':
		case 'B':
			if (pop(&headLevelStack)) printf("Cannot go back - already at root\n");
			else printf("Moved to parent directory\n");
			break;
		case 'd':
		case 'D':
			printf("Contents of '%s':\n", headLevelStack.next->directoryLevel->name);
			printDirectory(headLevelStack.next->directoryLevel, 0);
			break;
		case 'e':
		case 'E':
			printf("\nProgram terminated\n");
			break;
		default:
			printf("Unknown command\n");
			break;
		}

	} while (command != 'e' && command != 'E');

	deleteDirectory(&headDirectory);
	deleteStack(&headLevelStack);

	return EXIT_SUCCESS;
}

DirectoryPosition createDirectory(char* directoryName) {

	DirectoryPosition newDirectory = NULL;
	newDirectory = (DirectoryPosition)malloc(sizeof(Directory));

	if (newDirectory == NULL) return NULL;

	strcpy(newDirectory->name, directoryName);
	newDirectory->subdirectoryPosition = NULL;
	newDirectory->next = NULL;

	return newDirectory;
}

int push(LevelStackPosition headLevelStack, DirectoryPosition currentDirectory) {

	LevelStackPosition newLevelStack = NULL;
	newLevelStack = (LevelStackPosition)malloc(sizeof(LevelStack));

	if (newLevelStack == NULL) return MALLOC_ERROR;

	newLevelStack->directoryLevel = currentDirectory;

	newLevelStack->next = headLevelStack->next;
	headLevelStack->next = newLevelStack;

	return EXIT_SUCCESS;
}

int pop(LevelStackPosition headLevelStack) {

	LevelStackPosition temp;

	if (headLevelStack->next == NULL) return EMPTY_STACK;

	temp = headLevelStack->next;
	headLevelStack->next = headLevelStack->next->next;
	free(temp);

	return EXIT_SUCCESS;
}

int addDirectory(DirectoryPosition superdirectory, DirectoryPosition subdirectory) {

	subdirectory->next = superdirectory->subdirectoryPosition;
	superdirectory->subdirectoryPosition = subdirectory;

	return EXIT_SUCCESS;
}

int changeDirectory(LevelStackPosition headLevelStack, char* directoryName) {

	DirectoryPosition current = headLevelStack->next->directoryLevel->subdirectoryPosition;

	while (current != NULL && strcmp(current->name, directoryName) != 0)
		current = current->next;

	if (current != NULL) {
		printf("Navigated to '%s'\n", directoryName);
		push(headLevelStack, current);
	}
	else
		printf("Directory not found\n");

	return EXIT_SUCCESS;
}

int printDirectory(DirectoryPosition current, int depth) {

	int i = 0;
	DirectoryPosition currentSubdirectory = NULL;

	if (current->subdirectoryPosition != NULL) {
		currentSubdirectory = current->subdirectoryPosition;
		while (currentSubdirectory != NULL) {
			for (i = 0; i < depth; i++)
				printf("  ");
			printf("%s\n", currentSubdirectory->name);
			printDirectory(currentSubdirectory, depth + 1);
			currentSubdirectory = currentSubdirectory->next;
		}
	}

	return EXIT_SUCCESS;
}

int deleteDirectory(DirectoryPosition headDirectory) {

	DirectoryPosition temp = NULL;

	while (headDirectory->next != NULL) {
		temp = headDirectory->next;
		headDirectory->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int deleteStack(LevelStackPosition headLevelStack) {

	LevelStackPosition temp = NULL;

	while (headLevelStack->next != NULL) {
		temp = headLevelStack->next;
		headLevelStack->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}