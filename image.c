// Raduta Lavinia-Maria 313CA 2021
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "image.h"

// contine functiile elementare, care fac efectiv operatiile

int minimum(int a, int b)
{
	if (a <= b)
		return a;
	else
		return b;
}

int maximum(int a, int b)
{
	if (a <= b)
		return b;
	else
		return a;
}

int **alloc_matrix(int rows, int cols)
{
	int i;
	int **matrix = (int **)malloc(rows * sizeof(int *));
	if (!matrix)
		return NULL;
	for (i = 0; i < rows; i++) {
		matrix[i] = (int *)calloc(cols, sizeof(int));
		if (!matrix[i]) {
			// Daca o alocare pe parcurs esueaza
			// se elibereaza memoria alocata pentru liniile anterioare
			while (i >= 0) {
				i--;
				free(matrix[i]);
			}
			free(matrix);
			return NULL;
		}
	}
	return matrix;
}

int load(char filename[30], image_t *image, coord_t *coord)
{
	int binary; // Decide daca tipul imaginii este binar sau nu
	FILE *in = fopen(filename, "rb");
	if (!in)
		return -1;
	fscanf(in, "%s %d ", image->magic_number, &image->width);
	fscanf(in, "%d %d ", &image->height, &image->max_val);

	// Daca imaginea este color, matricea care retine valorile
	// pixelilor va avea latimea de 3 ori mai mare
	// decat latimea efectiva a pozei
	if (image->magic_number[1] == '3' || image->magic_number[1] == '6')
		image->actual_width = 3 * image->width;
	else
		image->actual_width = image->width;
	if (image->magic_number[1] == '2' || image->magic_number[1] == '3')
		binary = 0;
		else if (image->magic_number[1] == '5' || image->magic_number[1] == '6')
			binary = 1;

	image->picture = alloc_matrix(image->height, image->actual_width);
	if (!image->picture)
		return -1;

	// Citirea valorilor fiecarui pixel
	// Sau a valorii fiecarui canal (RGB)
	if (binary) {
		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->actual_width; j++)
				fread(&image->picture[i][j], sizeof(unsigned char), 1, in);
	} else {
		for (int i = 0; i < image->height; i++) {
			for (int j = 0; j < image->actual_width; j++)
				fscanf(in, "%d", &image->picture[i][j]);
		}
	}

	// Implicit este selectata toata imaginea
	coord->x1 = 0;
	coord->y1 = 0;
	coord->x2 = image->width - 1;
	coord->y2 = image->height - 1;
	fclose(in);
	return 0;
}

char *name(char *str, const char *sub)
{
	int len = strlen(sub);
	if (len > 0) {
		char *p = str;
		while ((p = strstr(p, sub)) != NULL)
			memmove(p, p + len, strlen(p + len) + 1);
	}
	return str;
}

int save_ascii(char filename[30], image_t *image)
{
	FILE *out = fopen(filename, "wt");
	if (!out)
		return -1;
	// Daca imaginea avea format binar isi va schimba
	// numarul magic pentru formatul ascii corespunzator
	if (image->magic_number[1] == '5')
		image->magic_number[1] = '2';
	if (image->magic_number[1] == '6')
		image->magic_number[1] = '3';
	fprintf(out, "%s\n", image->magic_number);
	fprintf(out, "%d %d\n", image->width, image->height);
	fprintf(out, "%d\n", image->max_val);

	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->actual_width; j++)
			fprintf(out, "%d ", image->picture[i][j]);
		fprintf(out, "\n");
	}
	fclose(out);
	return 0;
}

int save_binary(char filename[30], image_t *image)
{
	FILE *out = fopen(filename, "wb");
	if (!out)
		return -1;
	// Daca imaginea avea format ascii isi va schimba
	// numarul magic pentru formatul ascii corespunzator
	if (image->magic_number[1] == '2')
		image->magic_number[1] = '5';
	if (image->magic_number[1] == '3')
		image->magic_number[1] = '6';

	fprintf(out, "%s\n", image->magic_number);
	fprintf(out, "%d %d\n", image->width, image->height);
	fprintf(out, "%d\n", image->max_val);

	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->actual_width; j++)
			fprintf(out, "%c", (unsigned char)image->picture[i][j]);
	}
	fclose(out);
	return 0;
}

int select_coord(image_t image, coord_t *coord)
{
	// In cazul in care se face o selectie invalida pastrez
	// coordonatele vechi
	coord_t backup;
	backup.x1 = coord->x1;
	backup.y1 = coord->y1;
	backup.x2 = coord->x2;
	backup.y2 = coord->y2;
	int result = 0, number[4], count = 0;
	// result = 1 => selected all
	// result = 2 => comanda invalida
	// result = -1 => coordonate invalide
	char parameter[40], *q, *p;
	scanf(" %[^\n]%*c", parameter);
	while (parameter[strlen(parameter) - 1] == ' ')
		parameter[strlen(parameter) - 1] = '\0';
	if (strcmp(parameter, "ALL") == 0) {
		coord->x1 = 0;
		coord->y1 = 0;
		coord->x2 = image.width - 1;
		coord->y2 = image.height - 1;
		result = 1;
	} else {
		for (unsigned long i = 0; i < strlen(parameter) - 1; i++) {
			if (parameter[i] < '0' || parameter[i] > '9') {
				if (parameter[i] != ' ' && parameter[i] != '-')
					result = 2;
			}
		}
	}

	if (result == 0) {
		q = parameter;
		p = strchr(q, ' ');
		while (p) {
			char *nr = strndup(q, p - q);
			number[count] = atoi(nr);
			q += p - q + 1;
			p++;
			p = strchr(p, ' ');
			count++;
			free(nr);
		}
		number[count] = atoi(q);
		if (count != 3) {
			result = 2;
		} else {
			coord->x1 = number[0];
			coord->y1 = number[1];
			coord->x2 = number[2];
			coord->y2 = number[3];
		}
	}
	if (coord->x1 > image.width || coord->x2 > image.width ||
		coord->y1 > image.height || coord->y2 > image.height)
		result = -1;
	if (coord->x1 < 0 || coord->x2 < 0)
		result = -1;
	if (coord->y1 < 0 || coord->y2 < 0)
		result = -1;
	if (coord->x1 == coord->x2 || coord->y1 == coord->y2)
		result = -1;

	if (result == -1 || result == 2) {
		coord->x1 = backup.x1;
		coord->y1 = backup.y1;
		coord->x2 = backup.x2;
		coord->y2 = backup.y2;
	}
	return result;
}

int grayscale(image_t *image, coord_t coord)
{
	// Verific daca imaginea este color
	// altfel filtrul nu e disponibil
	if ((image->magic_number[1] != '3') && (image->magic_number[1] != '6'))
		return -1;

	// Se aplica numai pentru selectia aleasa
	for (int i = coord.y1; i <= coord.y2; i++)
		for (int j = coord.x1; j <= coord.x2; j++) {
			double auxd = (double)(image->picture[i][3 * j + 0] +
				image->picture[i][3 * j + 1] +
				image->picture[i][3 * j + 2]) / 3;
			int aux = (int)round(auxd);
			image->picture[i][3 * j + 0] = aux; // Valoarea noua pentru rosu
			image->picture[i][3 * j + 1] = aux; // Valoarea noua pentru verde
			image->picture[i][3 * j + 2] = aux; // Valoarea noua pentru albastru
		}

	return 0;
}

int sepia(image_t *image, coord_t coord)
{
	int new_red, new_green, new_blue;

	// Verific daca imaginea este color
	// altfel filtrul nu e disponibil
	if ((image->magic_number[1] != '3') && (image->magic_number[1] != '6'))
		return -1;

	// Se aplica numai pentru selectia aleasa
	for (int i = coord.y1; i <= coord.y2; i++)
		for (int j = coord.x1; j <= coord.x2; j++) {
			new_red = (int)round(0.393 * image->picture[i][3 * j + 0] +
				0.769 * image->picture[i][3 * j + 1] +
				0.189 * image->picture[i][3 * j + 2]);
			new_green = (int)round(0.349 * image->picture[i][3 * j + 0] +
				0.686 * image->picture[i][3 * j + 1] +
				0.168 * image->picture[i][3 * j + 2]);
			new_blue = (int)round(0.272 * image->picture[i][3 * j + 0] +
				0.534 * image->picture[i][3 * j + 1] +
				0.131 * image->picture[i][3 * j + 2]);

			// Am grija sa nu se depaseasca valoarea maxima a intensitatii
			image->picture[i][3 * j + 0] = minimum(new_red, image->max_val);
			image->picture[i][3 * j + 1] = minimum(new_green, image->max_val);
			image->picture[i][3 * j + 2] = minimum(new_blue, image->max_val);
		}
	return 0;
}

int crop(image_t *image, coord_t *coord)
{
	int new_width, new_height, new_actual_width;
	new_width = coord->x2 - coord->x1 + 1;
	new_height = coord->y2 - coord->y1 + 1;

	// Daca poza este color adaptez coordonatele la latimea de 3 ori mai mare
	if (strcmp(image->magic_number, "P3") == 0 ||
		strcmp(image->magic_number, "P6") == 0) {
		new_actual_width = 3 * new_width;
		coord->x1 *= 3;
		coord->x2 *= 3;
	} else {
		new_actual_width = new_width;
	}

	// Folosesc o matrice auxiliara pentru a salva temporar
	// selectia din poza pe care o pastrez
	int **aux = alloc_matrix(new_height, new_actual_width);
	if (!aux)
		return -1;

	for (int i = 0; i < new_height; i++) {
		for (int j = 0; j < new_actual_width; j++)
			aux[i][j] = image->picture[coord->y1 + i][coord->x1 + j];
	}

	// Dealoc memoria pentru vechea poza
	for (int i = 0; i < image->height; i++)
		free(image->picture[i]);
	free(image->picture);

	// Updatez noile dimensiuni
	image->width = new_width;
	image->height = new_height;
	image->actual_width = new_actual_width;

	// Pastrez numai partea selectata
	image->picture = alloc_matrix(image->height, image->actual_width);
	if (!image->picture)
		return -1;

	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->actual_width; j++)
			image->picture[i][j] = aux[i][j];
	}
	for (int i = 0; i < new_height; i++)
		free(aux[i]);
	free(aux);

	// Dupa taiere avem implicit selectata toata poza
	coord->x1 = 0;
	coord->y1 = 0;
	coord->x2 = image->width - 1;
	coord->y2 = image->height - 1;
	return 0;
}

// Rotirea la +90 de grade pentru o selectie patratica
void rotate_right90(image_t *image, coord_t coord, int colors)
{
	int N = coord.x2 - coord.x1 + 1, aux;
	// Matricea se parcurge pe contururi pana in centru
	for (int i = 0; i < N / 2; i++) {
		for (int j = i; j < N - i - 1; j++) {
			// Daca poza e color, colors = 3
			// Daca e grayscale, colors = 1
			for (int l = 0; l < colors; l++) {
				aux = image->picture[coord.y1 + i][(coord.x1 + j) * colors + l];
				image->picture[coord.y1 + i][(coord.x1 + j) * colors + l] =
				image->picture[coord.y2 - j][(coord.x1 + i) * colors + l];
				image->picture[coord.y2 - j][(coord.x1 + i) * colors + l] =
				image->picture[coord.y2 - i][(coord.x2 - j) * colors + l];
				image->picture[coord.y2 - i][(coord.x2 - j) * colors + l] =
				image->picture[coord.y1 + j][(coord.x2 - i) * colors + l];
				image->picture[coord.y1 + j][(coord.x2 - i) * colors + l] = aux;
			}
		}
	}
}

// Rotirea la -90 de grade pentru o selectie patratica
void rotate_left90(image_t *image, coord_t coord, int colors)
{
	int N = coord.x2 - coord.x1 + 1, aux;
	for (int i = 0; i < N / 2; i++) {
		// Matricea se parcurge pe contururi pana in centru
		for (int j = i; j < N - i - 1; j++) {
			// Daca poza e color, colors = 3
			// Daca e grayscale, colors = 1
			for (int l = 0; l < colors; l++) {
				aux = image->picture[coord.y1 + i][(coord.x1 + j) * colors + l];
				image->picture[coord.y1 + i][(coord.x1 + j) * colors + l] =
				image->picture[coord.y1 + j][(coord.x2 - i) * colors + l];
				image->picture[coord.y1 + j][(coord.x2 - i) * colors + l] =
				image->picture[coord.y2 - i][(coord.x2 - j) * colors + l];
				image->picture[coord.y2 - i][(coord.x2 - j) * colors + l] =
				image->picture[coord.y2 - j][(coord.x1 + i) * colors + l];
				image->picture[coord.y2 - j][(coord.x1 + i) * colors + l] = aux;
			}
		}
	}
}

// Rotirea la +90 a intregii imagini
void rotate_all_right90(image_t *image, coord_t *coord, int colors)
{
	// Matricea in care salvez temporar noua imagine
	// Se inverseaza latimea si inaltimea
	int **a = alloc_matrix(image->width, colors * image->height);

	// Matricea rezultata este matricea initiala, transpusa "de jos in sus"
	for (int i = image->height - 1; i >= 0; i--) {
		for (int j = 0; j < image->width; j++) {
			for (int l = 0; l < colors; l++)
				a[j][(image->height - i - 1) * colors + l] =
				image->picture[i][j * colors + l];
		}
	}
	for (int i = 0; i < image->height; i++)
		free(image->picture[i]);
	free(image->picture);

	// Aloc spatiu pentru noua imagine cu dimensiunile inversate
	image->picture = alloc_matrix(image->width, colors * image->height);

	// Schimb intre ele latimea si inaltimea
	int aux = image->width;
	image->width = image->height;
	image->height = aux;
	image->actual_width = colors * image->width;

	// Pun la loc valorile pixelilor
	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->actual_width; j++)
			image->picture[i][j] = a[i][j];
	}
	for (int i = 0; i < image->height; i++)
		free(a[i]);
	free(a);

	// Dupa rotirea intregii poze se considera selectata complet
	// si se schimba noile coordonate
	coord->x1 = 0;
	coord->y1 = 0;
	coord->x2 = image->width - 1;
	coord->y2 = image->height - 1;
}

// Rotirea la +90 a intregii imagini
void rotate_all_left90(image_t *image, coord_t *coord, int colors)
{
	int **a = alloc_matrix(image->width, colors * image->height);

	// Matricea rezultata este matricea initiala
	// transpusa "de dreapta la stanga"
	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->width; j++) {
			for (int l = 0; l < colors; l++)
				a[image->width - j - 1][i * colors + l] =
				image->picture[i][j * colors + l];
		}
	}

	for (int i = 0; i < image->height; i++)
		free(image->picture[i]);
	free(image->picture);

	image->picture = alloc_matrix(image->width, colors * image->height);

	// Schimb intre ele latimea si inaltimea
	int aux = image->width;
	image->width = image->height;
	image->height = aux;
	image->actual_width = colors * image->width;

	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->actual_width; j++)
			image->picture[i][j] = a[i][j];
	}

	for (int i = 0; i < image->height; i++)
		free(a[i]);
	free(a);

	// Dupa rotirea intregii poze se considera selectata complet
	// si se schimba noile coordonate
	coord->x1 = 0;
	coord->y1 = 0;
	coord->x2 = image->width - 1;
	coord->y2 = image->height - 1;
}

int rotate(image_t *image, coord_t *coord, int angle)
{
	int all = 0, colors;
	if (coord->x1 != 0 || coord->y1 != 0 || coord->x2 != image->width - 1 ||
		coord->y2 != image->height - 1) {
		if ((coord->x2 - coord->x1) != (coord->y2 - coord->y1))
			return 1; // Selectia nu este patrat
	} else {
		all = 1; // Se va roti toata imaginea
	}
	if (angle % 90 != 0)
		return -1; // Unghi nesuportat
	if ((image->magic_number[1] == '3') || (image->magic_number[1] == '6'))
		colors = 3; // Poza color
	if ((image->magic_number[1] == '2') || (image->magic_number[1] == '5'))
		colors = 1; // Poza grayscale

	if (all == 0) {
		if (0 < angle && angle < 360) {
			for (int k = 1; k <= angle / 90; k++)
				rotate_right90(image, *coord, colors);
		} else {
			if (angle < 0 && angle > -360) {
				for (int k = 1; k <= angle / (-90); k++)
					rotate_left90(image, *coord, colors);
			}
		}
	} else {
		if (0 < angle && angle < 360) {
			for (int k = 1; k <= angle / 90; k++)
				rotate_all_right90(image, coord, colors);
		} else {
			if (angle < 0 && angle > -360) {
				for (int k = 1; k <= angle / (-90); k++)
					rotate_all_left90(image, coord, colors);
			}
		}
	}
	return angle;
}
