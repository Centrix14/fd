#include <stdio.h>
#include <stdlib.h>

#include "figure.h"
#include "data/list.h"

figure *figure_new(int type, int x, int y, int a1, int a2) {
	figure *fptr = malloc(sizeof(figure));

	if (!fptr) {
		perror(__func__);
		exit(0);
	}

	fptr->type = type;
	fptr->x = x;
	fptr->y = y;
	fptr->a1 = a1;
	fptr->a2 = a2;

	fptr->visible = 1;

	return fptr;
}

void figure_free(figure *fptr) {
	if (fptr)
		free(fptr);
}

void figure_free_list(list *lptr) {
	figure_free((figure*)lptr->data);
}

figure *figure_new_point(int x, int y) {
	return figure_new(FG_TYPE_POINT, x, y, 0, 0);
}

figure *figure_new_line_pp(int x1, int y1, int x2, int y2) {
	return figure_new(FG_TYPE_LINE_PP, x1, y1, x2, y2);
}
