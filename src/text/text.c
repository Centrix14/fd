#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../text/text.h"
#include "../dbg.h"

text *tl_new(char *font, int size, int r, int g, int b) {
	text *tptr = malloc(sizeof(text));

	if (!tptr) {
		perror(__func__);

		return NULL;
	}

	tptr->font = malloc(strlen(font)+1);
	if (!tptr->font) {
		perror(__func__);

		return NULL;
	}
	strcpy(tptr->font, font);

	tptr->lay = 0;
	tptr->size = size;
	tptr->color_r = r;
	tptr->color_g = g;
	tptr->color_b = b;

	return tptr;
}

void tl_add_buffer(text *tptr, char *buf) {
	tptr->buffer = malloc(strlen(buf)+1);

	if (!tptr->buffer) {
		perror(__func__);

		return ;
	}

	strcpy(tptr->buffer, buf);
}

void tl_free(text *tptr) {
	if (tptr) {
		free(tptr->font);
		free(tptr);
	}
}

void tl_free_buffer(text *tptr) {
	if (tptr->buffer)
		free(tptr->buffer);
}
