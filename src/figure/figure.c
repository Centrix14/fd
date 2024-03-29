#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../fd_core.h"

int curr_lay = 0, fg_num = 0;

st_debug_start(0);

figure *figure_new(double type, double x, double y, double a1, double a2, double a3) {
	figure *fptr = malloc(sizeof(figure));

	if (!fptr) {
		el_call_error(ET_FAIL_TO_CREATE_FIGURE);

		exit(0);
	}

	fptr->type = type;
	fptr->x = x;
	fptr->y = y;
	fptr->a1 = a1;
	fptr->a2 = a2;
	fptr->a3 = a3;

	fptr->visible = VM_SHOW;
	fptr->lay = curr_lay;
	fptr->pr_mode = 0;

	fg_num++;

	return fptr;
}

void figure_free(figure *fptr) {
	if (fptr)
		free(fptr);
}

void figure_free_list(list *lptr) {
	figure_free((figure*)lptr->data);
}

figure *figure_new_point(double x, double y) {
	return figure_new(FG_TYPE_POINT, x, y, 0, 0, 0);
}

figure *figure_new_line_pp(double x1, double y1, double x2, double y2) {
	return figure_new(FG_TYPE_LINE_PP, x1, y1, x2, y2, 0);
}

figure *figure_new_rect_pp(double x, double y, double w, double h) {
	return figure_new(FG_TYPE_RECT_PP, x, y, w, h, 0);
}

figure *figure_new_circle(double x, double y, double r) {
	return figure_new(FG_TYPE_CIRCLE, x, y, r, 0, 0);
}

figure *figure_new_arc(double xc, double yc, double r, double angle1, double angle2) {
	return figure_new(FG_TYPE_ARC, xc, yc, r, angle1, angle2);
}

void figure_fill(figure *fptr, double x, double y, double a1, double a2, double type) {
	fptr->x = x;
	fptr->y = y;
	fptr->a1 = a1;
	fptr->a2 = a2;

	fptr->type = type;
	fptr->visible = VM_HIDE;
	fptr->lay = curr_lay;
}

figure *figure_rect_decompose(figure *rect) {
	static figure elms[4];
	double x1, y1, x2, y2;

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

	st_debug {
		for (int i = 0; i < 4; i++)
			printf("%d | %f\t%f\t%f\t%f\n", i, elms[i].x, elms[i].y, elms[i].a1, elms[i].a2);
		putc('\n', stdout);
	}

	return elms;
}

void figure_set_current_lay(int new_lay) {
	curr_lay = new_lay;
}

int figure_get_current_lay() {
	return curr_lay;
}

void figure_copy(figure *dst, figure *src) {
	dst->type = src->type;
	dst->visible = src->visible;
	dst->lay = src->lay;

	dst->x = src->x;
	dst->y = src->y;
	dst->a1 = src->a1;
	dst->a2 = src->a2;
}

int figure_is_line(figure *fptr) {
	if (!fptr)
		return 0;

	if (fptr->type == FG_TYPE_LINE_PP || fptr->type == FG_TYPE_LINE_LA)
		return 1;
	return 0;
}

int figure_is_projection_lay_list(list *lptr, int lay) {
	multi_obj *mo;
	list *node;

	node = lptr;
	while (node) {
		mo = mol_extract(node);

		if (!mo) {
			node = node->next;

			continue;
		}

		if (mo->lay == lay && mo->pr_mode)
			return 1;

		node = node->next;
	}

	return 0;
}

void figure_set_visible_by_lay_list(list *lptr, int lay, int vm_mode) {
	multi_obj *mo;
	list *node;

	node = lptr;
	while (node) {
		mo = mol_extract(node);

		if (!mo) {
			node = node->next;

			continue;
		}

		if (mo->lay == lay)
			mo->visible = vm_mode;
		mol_apply(node, mo);

		node = node->next;
	}
}

void figure_rotate_point(figure *base, figure *p, double angle) {
	double px = 0, py = 0;

	px = fabs(p->x - base->x);
	py = fabs(p->y - base->y);

	gel_rotate_point(&px, &py, angle);

	p->x = px + base->x;
	p->y = py + base->y;
}

void figure_rotate_line(figure *base, figure *l, double angle) {
	figure p1, p2;

	p1.x = l->x;
	p1.y = l->y;

	p2.x = l->a1;
	p2.y = l->a2;

	figure_rotate_point(base, &p1, angle);
	figure_rotate_point(base, &p2, angle);

	l->x = p1.x;
	l->y = p1.y;
	l->a1 = p2.x;
	l->a2 = p2.y;
}

void figure_rotate_rect(figure *base, figure *r, double angle, list *lptr) {
	figure *lines = NULL, *new = NULL;
	list *last = NULL;

	lines = figure_rect_decompose(r);

	for (int i = 0; i < 4; i++) {
		figure_rotate_line(base, lines + i, angle);	
		lines[i].visible = VM_SHOW;

		list_add_node(lptr);

		last = list_get_last(lptr);

		new = figure_new_point(0, 0);
		figure_copy(new, lines + i);
		new->visible = VM_SHOW;

		list_set_data(last, new);
	}
}

void figure_rotate(list *lptr, figure *base, double angle) {
	figure *fptr = NULL;

	fptr = (figure*)lptr->data;
	if (!fptr)
		return ;

	switch (fptr->type) {
		case FG_TYPE_CIRCLE:	
		case FG_TYPE_POINT:
			figure_rotate_point(base, fptr, angle);
		break;

		case FG_TYPE_LINE_PP:
			figure_rotate_line(base, fptr, angle);
		break;

		case FG_TYPE_RECT_PP:
			figure_rotate_rect(base, fptr, angle, lptr);

			figure_free(fptr);
			lptr->data = NULL;
		break;
	}
}

figure *figure_get_from_node(list *lptr) {
	if (lptr && lptr->dt == OT_FIGURE)
		return (figure*)(lptr->data);
	return NULL;
}

void figure_show(list *node) {
	figure *fptr;

	fptr = figure_get_from_node(node);
	if (fptr)
		printf("x1 = %g\ny1 = %g\nx2 = %g\ny2 = %g\n\n",
				fptr->x, fptr->y, fptr->a1, fptr->a2);
}

char *__get_point_info(figure *p) {
	static char buf[256] = "";

	sprintf(buf, "P(%g; %g)\n", p->x, p->y);

	return buf;
}

char *__get_line_info(figure *l) {
	static char buf[256] = "";

	sprintf(buf, "l = %g\n", gel_calculate_lenght(l));

	return buf;
}

char *__get_rect_info(figure *r) {
	static char buf[256] = "";
	double h = 0, w = 0, S = 0, d = 0;

	d = gel_calculate_lenght(r);
	h = fabs(r->y - r->a2);
	w = fabs(r->x - r->a1);
	S = h * w;

	sprintf(buf, "d = %g\nh = %g\nw = %g\nS = %g\n", d, h, w, S);

	return buf;
}

char *__get_circle_info(figure *c) {
	static char buf[256] = "";
	double R = 0, D = 0, l = 0;

	R = c->a1;
	D = 2 * R;
	l = PI * D;

	sprintf(buf, "R = %g\nD = %g\nl = %g\n", R, D, l);

	return buf;
}

char *figure_get_size_info(figure *fptr) {
	switch (fptr->type) {
		case FG_TYPE_POINT:
			return __get_point_info(fptr);
		break;

		case FG_TYPE_LINE_PP:
			return __get_line_info(fptr);
		break;

		case FG_TYPE_RECT_PP:
			return __get_rect_info(fptr);
		break;

		case FG_TYPE_CIRCLE:
			return __get_circle_info(fptr);
		break;
	}
	
	return "Empty";
}
