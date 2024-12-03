#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define num_sz 10

typedef struct Node
{
	int data;
	struct Node* next;
} Node;

typedef struct intList
{
	Node* head;
	size_t size;
} intList;

void initList(intList* list)
{
	list->head = NULL;
	list->size = 0;
}

Node* createNode(int data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if(!newNode)
	{
		fprintf(stderr, "Memory error: Could not append element '%d'", data);
		exit(EXIT_FAILURE);
	}

	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

void prepend(intList* list, int data)
{
	Node* newNode = createNode(data);

	newNode->next = list->head;
	list->head = newNode;
	list->size++;
}

void freeList(intList* list)
{
	if(list)
	{
		Node* current = list->head;
		Node* nextNode;

		while(current)
		{
			nextNode = current->next;
			free(current);
			current = nextNode;
		}
		free(list);
	}
	else
	{
		fprintf(stderr, "Error al acceder a la lista.");
		exit(EXIT_FAILURE);
	}
}

/*
* Gets and deletes an item from a list;
*/

int getItem(intList* list)
{
	int res = -1;
	if(list && list->head)
	{
		Node* target = list->head;
		list->head = target->next;
		res = target->data;
		free(target);
	}
	else if(!list->head)
	{
		freeList(list);
	}

	return res;
}

size_t filesz(FILE* fp)
{
	size_t lines = 0;
	char current;

	while((current = fgetc(fp)) != EOF)
	{
		if(current == '\n')
			lines++;
	}

	return lines;
}

int parsearInt(char input[num_sz])
{
	int output = 0;

	for(int i = 0; i < num_sz; i++)
	{
		if(input[i] != '\0')
		{
			output = output * 10 + (input[i] - '0');
			input[i] = '\0';
		}
	}

	return output;
}

void populateArrays(int* arr1, int* arr2, size_t arr_sz, char* name)
{
	FILE* fp;
	fp = fopen(name, "r");
	if(!fp)
	{
		fprintf(stderr, "Error al abrir el archivo '%s'", name);
		exit(2);
	}

	char current;
	char num[num_sz];
	int line = 0;
	int idx = 0;
	int res = 0;

	for(int i = 0; i < num_sz; i++)
	{
		num[i] = '\0';
	}

	while((current = fgetc(fp)) != EOF)
	{
		if(current == '\n')
		{
			res = parsearInt(num);
			arr2[line] = res;
			line++;
			res = 0;
			idx = 0;
		}
		else if(current == ' ')
		{
			current = fgetc(fp);
			// printf("%c", current);
			current = fgetc(fp);
			// printf("%c\n", current);
			res = parsearInt(num);
			arr1[line] = res;
			res = 0;
			idx = 0;
		}
		else
		{
			if(idx < num_sz - 1)
			{
				num[idx++] = current;
			}
		}
	}

	fclose(fp);

	return;
}

int main(int argc, char* argv[])
{
	intList* list1[100000];
	intList* list2[100000];

	memset(list1, 0, sizeof(list1));
	memset(list2, 0, sizeof(list2));

	FILE* finput;
	char* name = "./input.txt";
	finput = fopen("./input.txt", "r");
	size_t arr_sz = filesz(finput);
	rewind(finput);
	fclose(finput);

	int arr1[arr_sz];
	int arr2[arr_sz];
	int difs[arr_sz];

	populateArrays(arr1, arr2, arr_sz, name);

	for(int i = 0; i < arr_sz; i++)
	{
		if(list1[arr1[i]] == NULL)
		{
			list1[arr1[i]] = (intList*)malloc(sizeof(intList));
			initList(list1[arr1[i]]);
		}
		if(list2[arr2[i]] == NULL)
		{
			list2[arr2[i]] = (intList*)malloc(sizeof(intList));
			initList(list2[arr2[i]]);
		}
		prepend(list1[arr1[i]], i);
		prepend(list2[arr2[i]], i);
	}

	int res = 0;

	int idx1 = 0;
	int idx2 = 0;

	int cont = 0;

	for(int i = 0; i < arr_sz; i++)
	{
		if(list1[idx1] != NULL && list1[idx1]->head == NULL)
		{
			freeList(list1[idx1]);
			list1[idx1] = NULL;
		}

		if(list2[idx2] != NULL && list2[idx2]->head == NULL)
		{
			freeList(list2[idx2]);
			list2[idx2] = NULL;
		}

		// Encontrar el menor elemento en el map.
		while(list1[idx1] == NULL)
			idx1++;
		while(list2[idx2] == NULL)
			idx2++;

		int item1 = arr1[getItem(list1[idx1])];
		int item2 = arr2[getItem(list2[idx2])];

		if(cont < 20)
		{
			printf("%d %d\n", item1, item2);
			cont++;
		}

		res += abs(item1 - item2);
		difs[i] = abs(item1 - item2);
	}

	for(int i = 0; i < 20; i++)
	{
		printf("%d\n", difs[i]);
	}

	FILE* fp;
	fp = fopen("output.txt", "w");
	fprintf(fp, "Output: %d", res);
	fclose(fp);

	for(int i = 0; i < 100000; i++)
	{
		if(list1[i])
			freeList(list1[i]);
		if(list2[i])
			freeList(list2[i]);
	}

	return 0;
}
