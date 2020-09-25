#include <stdio.h>
#include <stdlib.h>

#include "figure.h"

figure *figure_new(int type, int x, int y, int w, int h) {
	figure *fptr = malloc(sizeof(figure));

	if (!fptr) {
		perror(__func__);
		exit(0);
	}

	fptr->type = type;
	fptr->x = x;
	fptr->y = y;
	fptr->w = w;
	fptr->h = h;

	return fptr;
}

void figure_free(figure *fptr) {
	if (fptr)
		free(fptr);
}
