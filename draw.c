#include <stdio.h>
#include <gtk/gtk.h>

#include "draw.h"
#include "figure.h"

static cairo_t *context;

void dl_set_cairo_context(cairo_t *cr) {
	context = cr;
}

void dl_draw_figure(figure *fptr) {
	if (!fptr || !fptr->visible)
		return ;

	switch (fptr->type) {
		case FG_TYPE_POINT:
			dl_draw_point(fptr);
		break;

		case FG_TYPE_LINE_PP:
			dl_draw_line_pp(fptr);
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
	cairo_set_source_rgb(context, 0, 200, 200);

	cairo_move_to(context, fptr->x, fptr->y);
	cairo_line_to(context, fptr->a1, fptr->a2);

	cairo_stroke(context);
}
