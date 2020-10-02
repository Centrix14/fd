#include <stdio.h>
#include <stdlib.h>

#include "figure.h"
#include "data/list.h"

int curr_lay = 0;

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
	fptr->lay = curr_lay;

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

figure *figure_new_rect_pp(int x, int y, int w, int h) {
	return figure_new(FG_TYPE_RECT_PP, x, y, w, h);
}

void figure_fill(figure *fptr, double x, double y, double a1, double a2, double type) {
	fptr->x = x;
	fptr->y = y;
	fptr->a1 = a1;
	fptr->a2 = a2;

	fptr->type = type;
	fptr->visible = VM_HIDE;
}

figure *figure_rect_decompose(figure *rect) {
	static figure elms[4];
	int x1, y1, x2, y2;

	// get coords
	x1 = rect->x;
	y1 = rect->y;
	x2 = rect->a1;
	y2 = rect->a2;

	// get lines
	figure_fill(&elms[0], x1, y1, x2, y1, FG_TYPE_LINE_PP);
	figure_fill(&elms[1], x1, y1, x1, y2, FG_TYPE_LINE_PP);
	figure_fill(&elms[2], x1, y2, x2, y2, FG_TYPE_LINE_PP);
	figure_fill(&elms[3], x2, y1, x2, y2, FG_TYPE_LINE_PP);

	return elms;
}

void figure_set_current_lay(int new_lay) {
	curr_lay = new_lay;
}

int figure_get_current_lay() {
	return curr_lay;
}
