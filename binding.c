#include <stdio.h>
#include <math.h>

#include "binding.h"
#include "data/list.h"
#include "figure.h"
#include "geometry.h"

void bl_get_bind_from_coords(list *lptr, int *x, int *y) {
	list *node = lptr;
	figure *fptr = NULL;

	while (node) {
		fptr = (figure*)(list_get_data(node->next));

		if (!fptr)
			break;

		if (bl_is_create_binding(fptr, *x, *y)) {
			bl_make_binding(fptr, x, y);
			break;
		}

		node = node->next;
	}
}

int bl_is_create_binding(figure *target, int x, int y) {
	switch (target->type) {
		case FG_TYPE_POINT:
			return bl_get_binding_possibility_point(target, x, y);	
		break;

		case FG_TYPE_LINE_PP:
			return bl_get_binding_possibility_line(target, x, y);
		break;

		case FG_TYPE_RECT_PP:
			return bl_get_binding_possibility_rect(target, x, y);
		break;
	}

	return 0;
}

int bl_get_coords_dif(int c1, int c2) {
	return (int)fabs((double)(c1 - c2));
}

int bl_get_binding_possibility_point(figure *point, int x, int y) {
	if (bl_get_coords_dif(point->x, x) < BINDING_AREA && bl_get_coords_dif(point->y, y) < BINDING_AREA)
		return 1;
	return 0;
}

int bl_get_binding_possibility_line(figure *line, int x, int y) {
	if (bl_get_coords_dif(line->x, x) < BINDING_AREA && bl_get_coords_dif(line->y, y) < BINDING_AREA)
		return 1;
	else if (bl_get_coords_dif(line->a1, x) < BINDING_AREA && bl_get_coords_dif(line->a2, y) < BINDING_AREA)
		return 1;
	else if (gel_is_middle_point_area(line, x, y, BINDING_AREA))
		return 1;
	return 0;
}

int bl_get_binding_possibility_rect(figure *rect, int x, int y) {
	figure *lines = figure_rect_decompose(rect);
	int res = 0;

	for (int i = 0; i < 4; i++) {
		res = bl_get_binding_possibility_line(&lines[i], x, y);

		if (res)
			return res;
	}

	return 0;
}

void bl_make_binding(figure *fptr, int *x, int *y) {
	switch (fptr->type) {
		case FG_TYPE_POINT:
			bl_make_binding_point(fptr, x, y);
		break;

		case FG_TYPE_LINE_PP:
			bl_make_binding_line(fptr, x, y);
		break;

		case FG_TYPE_RECT_PP:
			bl_make_binding_rect(fptr, x, y);
		break;
	}
}

void bl_make_binding_point(figure *point, int *x, int *y) {
	*x = point->x;
	*y = point->y;
}

void bl_make_binding_line(figure *line, int *x, int *y) {
	figure *point;

	if (bl_get_coords_dif(line->x, *x) < BINDING_AREA && bl_get_coords_dif(line->y, *y) < BINDING_AREA) {
		*x = line->x;
		*y = line->y;
	}
	else if (bl_get_coords_dif(line->a1, *x) < BINDING_AREA && bl_get_coords_dif(line->a2, *y) < BINDING_AREA) {
		*x = line->a1;
		*y = line->a2;
	}
	else if (gel_is_middle_point_area(line, *x, *y, BINDING_AREA)) {
		point = gel_get_middle_point(line);

		*x = point->x;
		*y = point->y;
	}
}

void bl_make_binding_rect(figure *rect, int *x, int *y) {
	figure *lines = figure_rect_decompose(rect);

	for (int i = 0; i < 4; i++) {
		if (bl_is_create_binding(&lines[i], *x, *y))
			bl_make_binding(&lines[i], x, y);
	}
}
