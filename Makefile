# Raduta Lavinia-Maria 313CA 2021
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c main_functions.c image.c image.h -o image_editor -lm 

clean:
	rm -f $(TARGETS)

.PHONY: clean
