#include <stdio.h>
#include "../fd_core.h"

#include "cmodel.h"

#define __get_buf() \
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer"); \
	if (!geometry_buffer) \
		return ;

/* core functions */

void point(double x, double y) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *point = NULL;

	__get_buf();

	point = figure_new_point(x, y);
	if (!point)
		return ;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, point);
}

void line_xy(double x1, double y1, double x2, double y2) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *line = NULL;

	__get_buf();

	line = figure_new_line_pp(x1, y1, x2, y2);
	if (!line)
		return ;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, line);
}

void line_ar(double x, double y, double angle, double r) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *line = NULL;

	__get_buf();

	line = figure_new_line_pp(x, y, 0, 0);
	if (!line)
		return ;
	gel_calculate_line_la(line, r, angle);

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, line);
}

void rect_xy(double x1, double y1, double x2, double y2) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *rect = NULL;

	__get_buf();

	rect = figure_new_rect_pp(x1, y1, x2, y2);
	if (!rect)
		return ;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, rect);
}

void rect_wh(double x1, double y1, double w, double h) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *rect = NULL;

	__get_buf();

	rect = figure_new_rect_pp(x1, y1, x1 + w, y1 + h);
	if (!rect)
		return ;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, rect);
}

void circle(double x, double y, double r) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *circle = NULL;

	__get_buf();

	circle = figure_new_circle(x, y, r);
	if (!circle)
		return ;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, circle);
}

void label(double crd[2], int rgb[3], unsigned int size, char *font, char *str) {
	list *geometry_buffer = NULL, *last = NULL;
	text *tptr = NULL;

	__get_buf();

	tptr = tl_new(font, size, rgb[0], rgb[1], rgb[2]);
	if (!tptr)
		return ;

	tl_add_buffer(tptr, str);
	if (!tptr->buffer)
		return ;

	tptr->x = crd[0]; tptr->y = crd[1];
	tptr->lay = figure_get_current_lay();

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	last->dt = OT_TEXT;
	list_set_data(last, tptr);
}

/* util functions */

void __info_point(figure *fptr) {
	printf("| point [%g; %g]\n", fptr->x, fptr->y);
}

void __info_line(figure *fptr) {
	printf("| line  [%g; %g] [%g; %g]\n", fptr->x, fptr->y, fptr->a1, fptr->a2);
}

void __info_rect(figure *fptr) {
	printf("| rect  [%g; %g] [%g; %g]\n", fptr->x, fptr->y, fptr->a1, fptr->a2);
}

void __info_circle(figure *fptr) {
	printf("| circle [%g; %g] R = %g\n", fptr->x, fptr->y, fptr->a1);
}

void __info_figure(list *lptr) {
	void (*funcs[])(figure*) = {__info_point, __info_line, __info_line, __info_rect,
	__info_rect, __info_circle};
	figure *fptr = NULL;

	fptr = (figure*)lptr->data;
	if (!fptr)
		return ;
	(*funcs[(int)fptr->type])(fptr);
}

void __info_text(list *lptr) {
	text *tptr = NULL;

	tptr = (text*)lptr->data;
	printf("| label [%g; %g] rgb = [%d; %d; %d] size = %d font = %s \"%s\"\n",
			tptr->x, tptr->y, tptr->color_r, tptr->color_g, tptr->color_b, tptr->size,
			tptr->font, tptr->buffer);
}

void info() {
	list *geometry_buffer = NULL, *lptr = NULL;

	__get_buf();

	lptr = geometry_buffer;
	while (lptr) {
		if (lptr->dt == OT_FIGURE)
			__info_figure(lptr);
		else
			__info_text(lptr);

		lptr = lptr->next;
	}
}

/* service functions */

void set_msg_list(list *msg_list) {
	pl_set_msg_list(msg_list);
}
