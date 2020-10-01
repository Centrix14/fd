#include <stdio.h>
#include <gtk/gtk.h>

#include "draw.h"
#include "figure.h"

static cairo_t *context;
static int preview_x = 0, preview_y = 0;

void dl_set_cairo_context(cairo_t *cr) {
	context = cr;
}

void dl_set_preview_coords(int px, int py) {
	preview_x = px;
	preview_y = py;
}

void dl_draw_figure(figure *fptr) {
	if (!fptr || fptr->visible == VM_HIDE)
		return ;

	switch (fptr->type) {
		case FG_TYPE_POINT:
			dl_draw_point(fptr);
		break;

		case FG_TYPE_LINE_PP:
			dl_draw_line_pp(fptr);
		break;

		case FG_TYPE_RECT_PP:
			dl_draw_rect_pp(fptr);
		break;
	}
}

void dl_draw_figure_list(list *lptr) {
	dl_draw_figure((figure*)(lptr->data));
}

void dl_draw_point(figure *fptr) {
	cairo_set_source_rgb(context, 0, 200, 200);	
	cairo_arc(context, fptr->x, fptr->y, 3, 0, 2 * G_PI);

	cairo_fill(context);
}

void dl_draw_line_pp(figure *fptr) {
	int x1, y1, x2, y2;
	int r, g, b;

	x1 = fptr->x;
	y1 = fptr->y;

	if (fptr->visible == VM_PREVIEW) {
		x2 = preview_x;
		y2 = preview_y;

		r = 255;
		g = 100;
		b = 0;
	}
	else {
		x2 = fptr->a1;
		y2 = fptr->a2;

		r = 0;
		g = 200;
		b = 200;
	}

	cairo_set_source_rgb(context, r, g, b);

	cairo_move_to(context, x1, y1);
	cairo_line_to(context, x2, y2);

	cairo_stroke(context);
}

void dl_draw_rect_pp(figure *fptr) {
	int x, y, w, h;
	int r, g, b;

	x = fptr->x;
	y = fptr->y;

	if (fptr->visible == VM_PREVIEW) {
		w = preview_x - x;
		h = preview_y - y;

		r = 255;
		g = 100;
		b = 0;
	}
	else {
		w = fptr->a1 - x;
		h = fptr->a2 - y;

		r = 0;
		g = 200;
		b = 200;
	}

	cairo_set_source_rgb(context, r, g, b);

	cairo_rectangle(context, x, y, w, h);

	cairo_stroke(context);
}
