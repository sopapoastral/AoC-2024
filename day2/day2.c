#include <assert.h>
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
	Node* tail;
	size_t size;
} intList;

void initList(intList* list)
{
	list->head = NULL;
	list->tail = NULL;
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
	if(!list->tail)
	{
		list->tail = newNode;
	}
	list->size++;
}

void append(intList* list, int data)
{
	Node* newNode = createNode(data);

	newNode->next = NULL;
	if(!list->head)
	{
		list->head = newNode;
	}
	if(list->tail)
	{
		list->tail->next = newNode;
	}
	list->tail = newNode;
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

void printfile(FILE* fp, int lines)
{
	char current;
	int cont = 0;

	while((current = getc(fp)) && cont < lines)
	{
		if(current == '\n')
		{
			cont++;
		}
		printf("%c", current);
	}
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

int occurrences(char* string, char c)
{
	int res = 0;
	for(int i = 0; i < strlen(string); i++)
	{
		res = string[i] == c ? res + 1 : res;
	}

	return res;
}

char** split(char* string, char delimiter, int* size)
{
	char** result = 0;
	size_t count = 0;
	char* tmp = string;
	char* last_comma = 0;
	char delim[2];
	delim[0] = delimiter;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while(*tmp)
	{
		if(delimiter == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	count += last_comma < (string + strlen(string) - 1);

	count++;

	result = malloc(sizeof(char*) * count);

	if(result)
	{
		size_t idx = 0;
		char* token = strtok(string, delim);

		while(token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	*size = count;

	return result;
}

int parsearInt(const char* string)
{
	int res = 0;
	for(int i = 0; string[i] != '\0'; i++)
	{
		if(string[i] != '\n')
			res = 10 * res + (string[i] - '0');
	}

	return res;
}

intList** getData(FILE* fp, size_t size)
{
	intList** res = (intList**)malloc(sizeof(intList*) * size);
	if(!res)
	{
		fprintf(stderr, "Error al reservar memoria para el arreglo de listas");
		exit(EXIT_FAILURE);
	}

	rewind(fp);
	char line[100];
	int idx = 0;

	while(fgets(line, sizeof(line), fp))
	{
		int arr_sz;
		char** arr = split(line, ' ', &arr_sz);

		res[idx] = (intList*)malloc(sizeof(intList));
		if(!res[idx])
		{
			fprintf(stderr, "Error al reservar memoria para una de las listas");

			for(int i = 0; i < idx; i++)
			{
				free(res[i]);
			}
			free(res);
			exit(EXIT_FAILURE);
		}
		initList(res[idx]);

		for(int i = 0; *(arr + i); i++)
		{
			int data = parsearInt(arr[i]);
			append(res[idx], data);
			free(*(arr + i));
		}

		free(arr);

		idx++;
	}

	return res;
}

/*
* Determinates whether the numbers in the list are safe or unsafe.
* Returns 1 if safe, 0 otherwise.
*/
int isSafe(intList* list)
{
	Node* current = list->head;
	Node* next = current->next;
	if(list->size < 2)
		return 1;
	int size = list->size - 1;
	int* difs = (int*)malloc(sizeof(int) * size);
	if(!difs)
	{
		fprintf(stderr, "Error al reservar memoria para el array difs.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < size; i++)
	{
		if(!next)
		{
			free(difs);
			return 0;
		}
		difs[i] = current->data - next->data;

		current = current->next;
		next = current->next;
	}

	int sign = difs[0] > 0 ? 1 : difs[0] < 0 ? -1 : 0;
	int count = 0;

	for(int i = 0; i < size; i++)
	{
		int prev_sign = sign;
		sign = difs[i] > 0 ? 1 : difs[i] < 0 ? -1 : 0;
		if(difs[i] == 0 || abs(difs[i]) > 3 || prev_sign != sign)
			count++;

		if(count > 1)
		{
			free(difs);
			return 0;
		}
	}

	free(difs);
	return 1;
}

int main(void)
{
	FILE* finput;
	finput = fopen("./input.in", "r");

	if(!finput)
	{
		fprintf(stderr, "Error al abrir el archivo './input.in'");
		exit(2);
	}

	size_t arr_sz = filesz(finput);
	rewind(finput);

	intList** datos = getData(finput, arr_sz);

	int res = 0;

	for(int i = 0; i < arr_sz; i++)
	{
		res += isSafe(datos[i]);
	}

	for(int i = 0; i < arr_sz; i++)
	{
		freeList(datos[i]);
	}
	free(datos);

	printf("Output: %d\n", res);

	FILE* fp;
	fp = fopen("output.txt", "w");
	fprintf(fp, "Output: %d", res);
	fclose(fp);

	fclose(finput);

	return 0;
}
