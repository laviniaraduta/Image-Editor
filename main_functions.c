// Raduta Lavinia-Maria 313CA 2021
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image.h"

// Functiile folosite in main pentru prelucrarea output-ului
// primit de la functiile de baza (din image.c)

void order(coord_t *coord)
{
	int m = minimum(coord->x1, coord->x2);
	int M = maximum(coord->x1, coord->x2);
	coord->x1 = m;
	coord->x2 = M;
	m = minimum(coord->y1, coord->y2);
	M = maximum(coord->y1, coord->y2);
	coord->y1 = m;
	coord->y2 = M;
	coord->x2--;
	coord->y2--;
}

void free_matrix(image_t *image)
{
	for (int i = 0; i < image->height; i++)
		free(image->picture[i]);
	free(image->picture);
}

int load_function(char filename[40], int result)
{
	int loaded;
	if (result != 0) {
		printf("Failed to load %s\n", filename);
		loaded = 0;
	} else {
		printf("Loaded %s\n", filename);
		loaded = 1;
	}
	return loaded;
}

void rotate_function(int result)
{
	if (result == -1)
		printf("Unsupported rotation angle\n");
	else if (result == 1)
		printf("Selection must be square\n");
	else
		printf("Rotated %d\n", result);
}

void save_function(char filename[40], image_t *image)
{
	char *x = NULL, *aux = NULL;

	// Elimin spatiile dintre numele pozei si "ascii"
	if (strstr(filename, "ascii")) {
		char *p = strchr(filename, ' ');
		strcpy(p, "ascii");
	}
	x = strstr(filename, "ascii");
	if (!x) {
		// Extrag numele pozei
		aux = name(filename, " ");
		if (save_binary(aux, image) == 0)
			printf("Saved %s\n", filename);
	} else {
		aux = name(filename, "ascii");
		if (save_ascii(aux, image) == 0)
			printf("Saved %s\n", aux);
	}
}

int select_function(image_t image, coord_t *coord)
{
	int ok_com = 1;
	int result = select_coord(image, coord);
	if (result == 1) {
		printf("Selected ALL\n");
	} else {
		if (result == 0) {
			order(coord);
			printf("Selected %d %d ", coord->x1, coord->y1);
			printf("%d %d\n", coord->x2 + 1, coord->y2 + 1);
		} else if (result == -1) {
			printf("Invalid set of coordinates\n");
		} else if (result == 2) {
			ok_com = 0;
		}
	}
	return ok_com;
}

void grayscale_function(int ok_load, image_t *image, coord_t coord)
{
	if (ok_load) {
		if (grayscale(image, coord) == 0)
			printf("Grayscale filter applied\n");
		else
			printf("Grayscale filter not available\n");
	} else {
		printf("No image loaded\n");
	}
}

void sepia_function(int ok_load, image_t *image, coord_t coord)
{
	if (ok_load) {
		if (sepia(image, coord) == 0)
			printf("Sepia filter applied\n");
		else
			printf("Sepia filter not available\n");
	} else {
		printf("No image loaded\n");
	}
}

void crop_function(int ok_load, image_t *image, coord_t *coord)
{
	if (ok_load) {
		if (crop(image, coord) == 0)
			printf("Image cropped\n");
	} else {
		printf("No image loaded\n");
	}
}
