#include <stdio.h>
#include <gtk/gtk.h>

#include "draw.h"
#include "figure.h"
#include "color.h"
#include "geometry.h"

static cairo_t *context;
static double preview_x = 0, preview_y = 0;
static int all_lays = 0, show_preview = 0;
static figure *preview;

void dl_set_cairo_context(cairo_t *cr) {
	context = cr;
}

void dl_set_preview_coords(double px, double py) {
	preview_x = px;
	preview_y = py;
}

void dl_switch_display_all_lays() {
	all_lays = !all_lays;
}

void dl_draw_figure(figure *fptr) {
	if (!dl_is_need_draw(fptr))
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

		case FG_TYPE_CIRCLE:
			dl_draw_circle(fptr);
		break;

		case FG_TYPE_ARC:
			dl_draw_arc(fptr);
		break;
	}
}

void dl_draw_figure_list(list *lptr) {
	dl_draw_figure((figure*)(lptr->data));
}

void dl_draw_point(figure *fptr) {
	if (fptr->visible == VM_PROJECTION && !all_lays)
		cl_set_color(context, CL_DEF_PROJECTION_COLOR);
	else
		cl_set_color(context, CL_DEF_DRAW_COLOR);

	cairo_arc(context, fptr->x, fptr->y, 3, 0, 2 * G_PI);

	cairo_fill(context);
}

void dl_draw_line_pp(figure *fptr) {
	double x1, y1, x2, y2;

	x1 = fptr->x;
	y1 = fptr->y;
	
	x2 = fptr->a1;
	y2 = fptr->a2;

	if (fptr->visible == VM_PREVIEW) {
		x2 = preview_x;
		y2 = preview_y;

		cl_set_color(context, CL_DEF_PREVIEW_COLOR);
	}
	else if (fptr->visible == VM_PROJECTION && !all_lays)
		cl_set_color(context, CL_DEF_PROJECTION_COLOR);
	else
		cl_set_color(context, CL_DEF_DRAW_COLOR);

	cairo_move_to(context, x1, y1);
	cairo_line_to(context, x2, y2);

	cairo_stroke(context);
}

void dl_draw_rect_pp(figure *fptr) {
	double x, y, w, h;

	x = fptr->x;
	y = fptr->y;
	
	w = fptr->a1 - x;
	h = fptr->a2 - y;

	if (fptr->visible == VM_PREVIEW) {
		w = preview_x - x;
		h = preview_y - y;

		cl_set_color(context, CL_DEF_PREVIEW_COLOR);
	}
	else if (fptr->visible == VM_PROJECTION && !all_lays)
		cl_set_color(context, CL_DEF_PROJECTION_COLOR);
	else
		cl_set_color(context, CL_DEF_DRAW_COLOR);

	cairo_rectangle(context, x, y, w, h);

	cairo_stroke(context);
}

void dl_draw_circle(figure *fptr) {
	figure *rad_line;
	double x, y, r;

	x = fptr->x;
	y = fptr->y;
	r = fptr->a1;

	if (fptr->visible == VM_PREVIEW) {
		rad_line = figure_new_line_pp(x, y, preview_x, preview_y);
		r = gel_calculate_lenght(rad_line);

		cl_set_color(context, CL_DEF_PREVIEW_COLOR);
	}
	else if (fptr->visible == VM_PROJECTION && !all_lays)
		cl_set_color(context, CL_DEF_PROJECTION_COLOR);
	else
		cl_set_color(context, CL_DEF_DRAW_COLOR);

	cairo_arc(context, x, y, r, 0, 2 * G_PI);

	cairo_stroke(context);
}

void dl_draw_arc(figure *fptr) {
	double x, y, r, a;

	x = fptr->x;
	y = fptr->y;
	r = fptr->a1;
	a = fptr->a2;

	if (fptr->visible == VM_PROJECTION && !all_lays)
		cl_set_color(context, CL_DEF_PROJECTION_COLOR);
	else
		cl_set_color(context, CL_DEF_DRAW_COLOR);

	cairo_arc(context, x, y, r, 0, gel_convert_grades_to_rads(a));

	cairo_stroke(context);
}

int dl_is_need_draw(figure *fptr) {
	if (!fptr || fptr->visible == VM_HIDE || (fptr->lay != figure_get_current_lay() && !all_lays && fptr->visible != VM_PROJECTION))
		return 0;
	return 1;
}

void dl_send_preview_figure(figure *fptr) {
	preview = fptr;
}

void dl_draw_preview() {
	if (show_preview)
		dl_draw_figure(preview);
}

void dl_switch_show_preview() {
	show_preview = !show_preview;
}
