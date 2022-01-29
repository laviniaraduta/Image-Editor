// Raduta Lavinia-Maria 313CA 2021
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image.h"

int main(void)
{
	char filename[40], command[12], junk[70];
	// junk folosit pentru citirea in continuare
	// a randului unei comenzi invalide
	image_t image;
	coord_t coord;
	int ok_load = 0, ok_com = 0, angle;
	// ok_load verifica daca este deja incarcata o fotografie in memorie
	// ok_com verifica daca comanda introdusa este valida
	scanf(" %s", command);
	while (strcmp(command, "EXIT") != 0) {
		if (strcmp(command, "LOAD") == 0) {
			if (ok_load == 1)
				free_matrix(&image);
			ok_com = 1;
			scanf(" %s", filename);
			ok_load = load_function(filename, load(filename, &image, &coord));
		}
		if (strcmp(command, "SELECT") == 0) {
			if (ok_load) {
				ok_com = 1;
				if (select_function(image, &coord) == 0)
					printf("Invalid command\n");
			} else {
				ok_com = 1;
				scanf(" %[^\n]%*c", filename);
				printf("No image loaded\n");
			}
		}
		if (strcmp(command, "ROTATE") == 0) {
			ok_com = 1;
			scanf("%d", &angle);
			if (ok_load)
				rotate_function(rotate(&image, &coord, angle));
			else
				printf("No image loaded\n");
		}
		if (strcmp(command, "CROP") == 0) {
			ok_com = 1;
			crop_function(ok_load, &image, &coord);
		}
		if (strcmp(command, "GRAYSCALE") == 0) {
			ok_com = 1;
			grayscale_function(ok_load, &image, coord);
		}
		if (strcmp(command, "SEPIA") == 0) {
			ok_com = 1;
			sepia_function(ok_load, &image, coord);
		}
		if (strcmp(command, "SAVE") == 0) {
			ok_com = 1;
			scanf(" %[^\n]%*c", filename);
			if (ok_load == 1)
				save_function(filename, &image);
			else
				printf("No image loaded\n");
		}
		if (ok_com == 0) {
			fgets(junk, 39, stdin);
		printf("Invalid command\n");
		}
		scanf(" %s", command);
		ok_com = 0;
	}
	if (ok_load)
		free_matrix(&image);
	else
		printf("No image loaded\n");
	return 0;
}
