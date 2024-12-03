#include <stdio.h>
#include <stdlib.h>

enum States
{
	basura = 0,
	m = 1,
	u = 2,
	l = 3,
	opar = 4,
	X = 5,
	comma = 6,
	Y = 7,
	cpar = 8,
	d = 9,
	o = 10,
	n = 11,
	apostrophe = 12,
	t = 13,
};

typedef struct
{
	enum States estado_actual;
	enum States build; // m, d or t (mul, do or don't);
	int valor_x;
	int valor_y;
	int able;
	long int valor_mul;
} finite_automaton;

int contChars(FILE* fp)
{
	if(!fp)
	{
		fprintf(stderr, "No se puede leer de un archivo nulo.");
		exit(EXIT_FAILURE);
	}

	rewind(fp);
	int res = 0;
	char current;

	while((current = fgetc(fp)) != EOF)
	{
		res++;
	}

	return res;
}

char* loadString(FILE* fp)
{
	int file_sz = contChars(fp);
	if(file_sz <= 0)
	{
		fprintf(stderr, "Size error: fp size non positive in loadString.\n");
		exit(EXIT_FAILURE);
	}

	rewind(fp);
	char* res = (char*)malloc(sizeof(char) * file_sz);
	if(!res)
	{
		fprintf(stderr, "Allocation error: cannot allocate memory for res in loadString.\n");
		exit(EXIT_FAILURE);
	}

	char current;
	int cont = 0;
	while((current = fgetc(fp)) != EOF)
	{
		if(cont < file_sz)
		{
			res[cont++] = current;
		}
		else
		{
			fprintf(stderr, "Index error: Index count out of bounds (file_sz) in loadString.\n");
			exit(EXIT_FAILURE);
		}
	}

	return res;
}

void init(finite_automaton* maquina)
{
	maquina->valor_mul = maquina->valor_x = maquina->valor_y = 0;
	maquina->able = 1;
	maquina->build = maquina->estado_actual = basura;
}

void cargarDatos(finite_automaton* maquina, char* string)
{
	for(int i = 0; *(string + i); i++)
	{
		char current = *(string + i);
		/*
		* printf("Estado : %d Build: %d  Able: %d Valor: %c\n",
		* 	   maquina->estado_actual,
		* 	   maquina->build,
		* 	   maquina->able,
		* 	   current);
		*/
		switch(maquina->estado_actual)
		{
		case basura:
			if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
			}
			// else seguir en el mismo estado
			break;
		case m:
			if(current == 'u')
			{
				maquina->estado_actual = u;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case u:
			if(current == 'l')
			{
				maquina->estado_actual = l;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case l:
			if(current == '(')
			{
				maquina->estado_actual = opar;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case opar:
			if(current >= '0' && current <= '9')
			{
				maquina->estado_actual = X;
				maquina->valor_x *= 10;
				maquina->valor_x += (current - '0');
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == ')')
			{
				maquina->estado_actual = cpar;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case X:
			if(current >= '0' && current <= '9')
			{
				maquina->estado_actual = X;
				maquina->valor_x *= 10;
				maquina->valor_x += (current - '0');
			}
			else if(current == ',')
			{
				maquina->estado_actual = comma;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case comma:
			if(current >= '0' && current <= '9')
			{
				maquina->estado_actual = Y;
				maquina->valor_y *= 10;
				maquina->valor_y += (current - '0');
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case Y:
			if(current >= '0' && current <= '9')
			{
				maquina->estado_actual = Y;
				maquina->valor_y *= 10;
				maquina->valor_y += (current - '0');
			}
			else if(current == ')')
			{
				maquina->estado_actual = cpar;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case cpar:
			if(maquina->build == m) // == mul(X,Y)
			{
				int val = maquina->valor_x * maquina->valor_y * maquina->able;
				maquina->valor_mul += val;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(maquina->build == d) // == do()
			{
				maquina->able = 1;
			}
			else if(maquina->build == t) // == don't()
			{
				maquina->able = 0;
			}
			if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
			}
			else
			{
				maquina->estado_actual = basura;
			}
			break;
		case d:
			if(current == 'o')
			{
				maquina->estado_actual = o;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case o:
			if(current == '(')
			{
				maquina->estado_actual = opar;
			}
			else if(current == 'n')
			{
				maquina->estado_actual = n;
				maquina->build = t;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case n:
			if(current == '\'')
			{
				maquina->estado_actual = apostrophe;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case apostrophe:
			if(current == 't')
			{
				maquina->estado_actual = t;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		case t:
			if(current == '(')
			{
				maquina->estado_actual = opar;
			}
			else if(current == 'm')
			{
				maquina->estado_actual = m;
				maquina->build = m;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else if(current == 'd')
			{
				maquina->estado_actual = d;
				maquina->build = d;
				maquina->valor_y = maquina->valor_x = 0;
			}
			else
			{
				maquina->estado_actual = basura;
				maquina->valor_y = maquina->valor_x = 0;
			}
			break;
		}
	}
}

int main(void)
{
	FILE* finput;
	finput = fopen("./example.in", "r");
	if(!finput)
	{
		fprintf(stderr, "Error al abrir el archivo 'input.in'");
		exit(2);
	}

	long int res = 0;

	char* string = loadString(finput);

	finite_automaton* maquina = (finite_automaton*)malloc(sizeof(finite_automaton));
	if(!maquina)
	{
		fprintf(stderr, "Error al reservar memoria para el automata.");
		exit(EXIT_FAILURE);
	}

	init(maquina);
	cargarDatos(maquina, string);

	res = maquina->valor_mul;

	free(string);
	free(maquina);

	FILE* foutput;
	foutput = fopen("./output.out", "w");
	if(!foutput)
	{
		fprintf(stderr, "Error al crear el archivo 'output.out'");
		exit(2);
	}
	fprintf(foutput, "Output: %ld", res);
	fclose(foutput);

	fclose(finput);

	return 0;
}
