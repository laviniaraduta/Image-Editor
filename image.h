// Raduta Lavinia-Maria 313CA 2021
#include <stdio.h>

// Structura in care pastrez informatiile imaginii
typedef struct {
	char magic_number[3];
	int width, height, actual_width;
	unsigned int max_val;
	int **picture;
} image_t;

// Structura in care pastrez coordonatele selectate
typedef struct {
	int x1, x2, y1, y2;
} coord_t;

int load(char filename[30], image_t *image, coord_t *coord);

int save_ascii(char filename[30], image_t *image);

int save_binary(char filename[30], image_t *image);

int select_coord(image_t image, coord_t *coord);

int grayscale(image_t *image, coord_t coord);

int sepia(image_t *image, coord_t coord);

int crop(image_t *image, coord_t *coord);

// Extragerea unui subsir dintr-un sir
char *name(char *str, const char *sub);

int rotate(image_t *image, coord_t *coord, int angle);

int **alloc_matrix(int rows, int cols);

int minimum(int a, int b);

int maximum(int a, int b);

// Functie pentru prelucrarea output-ului functiei load
int load_function(char filename[40], int result);

// Functie pentru prelucrarea output-ului functiei select_coord
int select_function(image_t image, coord_t *coord);

// Functie pentru prelucrarea output-ului functiei crop
void crop_function(int ok_load, image_t *image, coord_t *coord);

// Functie pentru prelucrarea output-ului functiei rotate
void rotate_function(int result);

// Functie pentru prelucrarea output-ului functiei grayscale
void grayscale_function(int ok_load, image_t *image, coord_t coord);

// Functie pentru prelucrarea output-ului functiei sepia
void sepia_function(int ok_load, image_t *image, coord_t coord);

// Functie pentru prelucrarea output-ului functiilor save_ascii, save_binary
void save_function(char filename[40], image_t *image);

// Functie pentru dealocarea memoriei
void free_matrix(image_t *image);

// Ordoneaza in ordine crescatoare coordonatele x si y
void order(coord_t *coord);
