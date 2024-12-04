#include <stdio.h>
#include <stdlib.h>

#define input "input.in"

typedef enum directions
{
	ne = 0,
	no = 1,
	so = 2,
	se = 3,
} directions;

int cantCol(FILE* fp)
{
	if(!fp)
	{
		fprintf(stderr, "FILE* fp nulo en cantCol.\n");
		exit(EXIT_FAILURE);
	}
	rewind(fp);

	char current;
	int col_sz = 0;

	while((current = fgetc(fp)) != '\n')
	{
		col_sz++;
	}

	return col_sz;
}

int cantRow(FILE* fp)
{
	if(!fp)
	{
		fprintf(stderr, "FILE* fp nulo en cantRow.\n");
		exit(EXIT_FAILURE);
	}
	rewind(fp);

	char current;
	int row_sz = 0;

	while((current = fgetc(fp)) != EOF)
	{
		if(current == '\n')
			row_sz++;
	}

	return row_sz;
}

char** cargarDatos(FILE* fp, int cant_row, int cant_col)
{
	if(!fp)
	{
		fprintf(stderr, "FILE* fp nulo en cargarDatos.\n");
		exit(EXIT_FAILURE);
	}
	rewind(fp);

	char** res = (char**)malloc(sizeof(char*) * cant_row);
	if(!res)
	{
		fprintf(stderr, "Error al reservar memoria para res en cargarDatos.\n");
		exit(EXIT_FAILURE);
	}

	for(int j = 0; j < cant_col; j++)
	{
		res[j] = (char*)malloc(sizeof(char) * cant_col);
		if(!res[j])
		{
			fprintf(stderr, "Error al reservar memoria para res[%d] en cargarDatos.\n", j);

			for(int k = 0; k < j; k++)
			{
				free(res[k]);
			}
			free(res);
			exit(EXIT_FAILURE);
		}
	}

	char current;
	int i = 0;
	int j = 0;

	while((current = fgetc(fp)) != EOF)
	{
		res[i][j] = current;
		if(current == '\n')
		{
			i++;
			j = 0;
		}
		else
			j++;
	}

	return res;
}

int main(void)
{
	FILE* finput;
	finput = fopen(input, "r");
	if(!finput)
	{
		fprintf(stderr, "El archivo 'input.in' no fue encontrado.\n");
		exit(2);
	}

	int cant_row = cantRow(finput);
	int cant_col = cantCol(finput);

	char** sopa = cargarDatos(finput, cant_row, cant_col);
	int res = 0;

	for(int i = 0; i < cant_row - 2; i++)
	{
		for(int j = 0; j < cant_col - 2; j++)
		{
			char* search = "MAS";
			char* reverse = "SAM";
			int size = 3;

			/*
            * Buscar alguno:
            * M   S
            *  A   A
            *   S   M
            */
			int find = 1;
			for(int k = 0; k < size; k++)
			{
				if(sopa[i + k][j + k] != search[k])
					find = 0;
			}
			if(!find)
			{
				find = 1;
				for(int k = 0; k < size; k++)
				{
					if(sopa[i + k][j + k] != reverse[k])
						find = 0;
				}
			}

			/*
            * Si se encontro, buscar alguno:
            *   S  M
            *  A  A
            * M  S
            */
			if(find)
			{
				find = 1;
				for(int k = 0; k < size; k++)
				{
					if(sopa[i + size - 1 - k][j + k] != search[k])
						find = 0;
				}
				if(!find)
				{
					find = 1;
					for(int k = 0; k < size; k++)
					{
						if(sopa[i + size - 1 - k][j + k] != reverse[k])
							find = 0;
					}
				}
			}

			res += find;
		}
	}

	for(int i = 0; i < cant_col; i++)
	{
		free(sopa[i]);
	}
	free(sopa);

	FILE* foutput;
	foutput = fopen("output.out", "w");
	fprintf(foutput, "Output: %d", res);
	fclose(foutput);

	fclose(finput);
	return 0;
}
