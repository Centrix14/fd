#include <stdio.h>
#include <math.h>

#include "binding.h"
#include "../list/list.h"
#include "../figure/figure.h"
#include "../geometry/geometry.h"
#include "../text/text.h"
#include "../multi_obj/multi_obj.h"

char *bl_try_make_object_bind(list *lptr, double *x, double *y) {
	list *node = lptr;
	figure *fptr = NULL;

	while (node) {
		fptr = mol_conv_to_figure(node->next);
		if (!fptr)
			break;

		if (fptr->lay != figure_get_current_lay())
			node = node->next;
		else if (bl_is_create_binding(fptr, *x, *y)) {
			bl_make_binding(fptr, x, y);

			return "";
		}

		node = node->next;
	}

	return NULL;
}

int bl_is_create_binding(figure *target, double x, double y) {
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

		case FG_TYPE_CIRCLE:
			return bl_get_binding_possibility_circle(target, x, y);
		break;
	}

	return 0;
}

double bl_get_coords_dif(double c1, double c2) {
	return fabs(c1 - c2);
}

int bl_get_binding_possibility_point(figure *point, double x, double y) {
	if (point->lay != figure_get_current_lay())
		return 0;

	if (bl_get_coords_dif(point->x, x)< BINDING_AREA
			&& bl_get_coords_dif(point->y, y) < BINDING_AREA)
		return 1;
	return 0;
}

int bl_get_binding_possibility_line(figure *line, double x, double y) {
	if (line->lay != figure_get_current_lay())
		return 0;

	if (bl_get_coords_dif(line->x, x) < BINDING_AREA
			&& bl_get_coords_dif(line->y, y) < BINDING_AREA)
		return 1;
	else if (bl_get_coords_dif(line->a1, x) < BINDING_AREA
			&& bl_get_coords_dif(line->a2, y) < BINDING_AREA)
		return 1;
	else if (gel_is_middle_point_area(line, x, y, BINDING_AREA))
		return 1;
	return 0;
}

int bl_get_binding_possibility_rect(figure *rect, double x, double y) {
	figure *lines = figure_rect_decompose(rect);
	int res = 0;

	for (int i = 0; i < 4; i++) {
		res = bl_get_binding_possibility_line(&lines[i], x, y);

		if (res)
			return res;
	}

	return 0;
}

void bl_make_binding(figure *fptr, double *x, double *y) {
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

		case FG_TYPE_CIRCLE:
			bl_make_binding_circle(fptr, x, y);
		return;
	}
}

void bl_make_binding_point(figure *point, double *x, double *y) {
	*x = point->x;
	*y = point->y;
}

void bl_make_binding_line(figure *line, double *x, double *y) {
	figure *point;

	if (bl_get_coords_dif(line->x, *x) < BINDING_AREA
			&& bl_get_coords_dif(line->y, *y) < BINDING_AREA) {
		*x = line->x;
		*y = line->y;
	}
	else if (bl_get_coords_dif(line->a1, *x) < BINDING_AREA
			&& bl_get_coords_dif(line->a2, *y) < BINDING_AREA) {
		*x = line->a1;
		*y = line->a2;
	}
	else if (gel_is_middle_point_area(line, *x, *y, BINDING_AREA)) {
		point = gel_get_middle_point(line);

		*x = point->x;
		*y = point->y;
	}
}

void bl_make_binding_rect(figure *rect, double *x, double *y) {
	figure *lines = figure_rect_decompose(rect);

	for (int i = 0; i < 4; i++) {
		if (bl_is_create_binding(&lines[i], *x, *y))
			bl_make_binding(&lines[i], x, y);
	}
}

char *bl_try_make_intersection_binding(list *lptr, double *x, double *y) {
	figure *l1, *l2, p;
	list *node1 = lptr, *node2 = lptr;
	char *is_correct;

	while (node1) {
		l1 = (figure*)node1->data;
		node2 = lptr;

		while (node2) {
			l2 = (figure*)node2->data;

			if (node1 != node2) {
				if (figure_is_line(l1) && figure_is_line(l2)) {
					if (l1->lay != figure_get_current_lay())
						if (l2->lay != figure_get_current_lay())
							return NULL;

					is_correct = gel_calculate_intersection(l1, l2, &p);
					if (is_correct) {
						if (bl_get_coords_dif(p.x, *x) <= BINDING_AREA
								&& bl_get_coords_dif(p.y, *y) <= BINDING_AREA) {
							*x = p.x;
							*y = p.y;

							return "";
						}
					}
				}
			}

			node2 = node2->next;
		}

		node1 = node1->next;
	}

	return NULL;
}

void bl_bind(list *lptr, double *x, double *y) {
	char *res = NULL;

	res = bl_try_make_object_bind(lptr, x, y);
	if (res)
		return ;

	res = bl_try_make_intersection_binding(lptr, x, y);
	if (res)
		return ;

	res = bl_try_make_vertical_binding(lptr, x, y);
	if (res)
		return ;
}

char *bl_try_make_vertical_binding(list *lptr, double *x, double *y) {
	char *is_correct;
	list *node;

	node = lptr;
	while (node) {
		is_correct = bl_make_vertical_bind_if_possible(node, x, y);
		if (is_correct)
			return "";

		node = node->next;
	}

	return NULL;
}

char *bl_make_vertical_bind_if_possible(list *node, double *x, double *y) {
	figure *fptr;

	fptr = (figure*)node->data;
	if (!fptr)
		return NULL;
	if (fptr->lay != figure_get_current_lay())
		return NULL;

	if (bl_get_coords_dif(fptr->x, *x) <= BINDING_AREA) {
		*x = fptr->x;

		return "";
	}
	if (bl_get_coords_dif(fptr->a1, *x) <= BINDING_AREA) {
		*x = fptr->a1;

		return "";
	}

	if (bl_get_coords_dif(fptr->y, *y) <= BINDING_AREA) {
		*y = fptr->y;

		return "";
	}
	if (bl_get_coords_dif(fptr->a2, *y) <= BINDING_AREA) {
		*y = fptr->a2;

		return "";
	}

	return NULL;
}

int bl_get_binding_possibility_circle(figure *circle, double x, double y) {
	figure p;

	p.x = x;
	p.y = y;

	return gel_is_point_in_circle(circle, &p);
}

void __bl_make_trivial_circle_binding(figure *circle, double *x, double *y) {
	double R = circle->a1;

	if (circle->x == *x) {
		if (circle->y < *y)
			*y = circle->y + R;
		else
			*y = fabs(R - circle->y);
	}
	else {
		if (circle->x < *x)
			*x = circle->x + R;
		else
			*x = fabs(R - circle->x);
	}
}

double __bl_eval_coords_positive(double base, double c, double step) {
	if (c > base)
		return c - step;
	else
		return c + step;
}

double __bl_eval_coords_negative(double base, double c, double step) {
	if (c < base)
		return c - step;
	else
		return c + step;
}

void bl_make_binding_circle(figure *circle, double *x, double *y) {
	double R, len, c, a;
	figure l;

	l.x = circle->x;
	l.y = circle->y;
	l.a1 = *x;
	l.a2 = *y;

	// check for trivial case
	if (circle->x == *x || circle->y == *y)
		__bl_make_trivial_circle_binding(circle, x, y);

	// get radii, and length of l
	R = circle->a1;
	len = gel_calculate_lenght(&l);

	// get c
	c = fabs(len - R);

	// get side of square
	a = sqrt(pow(c, 2) / 2);

	// make binding
	if ((len - R) > 0) {
		*x = __bl_eval_coords_positive(circle->x, *x, a);
		*y = __bl_eval_coords_positive(circle->y, *y, a);
	}
	else {
		*x = __bl_eval_coords_negative(circle->x, *x, a);
		*y = __bl_eval_coords_negative(circle->y, *y, a);
	}
}
