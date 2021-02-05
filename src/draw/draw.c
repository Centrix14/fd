#include <stdio.h>
#include <gtk/gtk.h>

#include "draw.h"
#include "../figure/figure.h"
#include "../color/color.h"
#include "../geometry/geometry.h"

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

int dl_get_preview() {
	return show_preview;
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
	cl_set_color_fg(context, fptr->visible);

	if (fptr->visible == VM_PROJECTION && all_lays)
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

	cl_set_color_fg(context, fptr->visible);

	if (fptr->visible == VM_PREVIEW) {
		x2 = preview_x;
		y2 = preview_y;
	}
	else if (fptr->visible == VM_PROJECTION && all_lays)
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

	cl_set_color_fg(context, fptr->visible);

	if (fptr->visible == VM_PREVIEW) {
		w = preview_x - x;
		h = preview_y - y;
	}
	else if (fptr->visible == VM_PROJECTION && all_lays)
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

	cl_set_color_fg(context, fptr->visible);

	if (fptr->visible == VM_PREVIEW) {
		rad_line = figure_new_line_pp(x, y, preview_x, preview_y);
		r = gel_calculate_lenght(rad_line);
	}
	else if (fptr->visible == VM_PROJECTION && all_lays)
		cl_set_color(context, CL_DEF_DRAW_COLOR);

	cairo_arc(context, x, y, r, 0, 2 * G_PI);

	cairo_stroke(context);

	dl_draw_point(fptr);
}

void dl_draw_arc(figure *fptr) {
	double x, y, r, a1, a2;

	x = fptr->x;
	y = fptr->y;
	a1 = fptr->a1;
	a2 = fptr->a2;
	r = fptr->a3;

	cl_set_color_fg(context, fptr->visible);

	if (fptr->visible == VM_PROJECTION && all_lays)
		cl_set_color(context, CL_DEF_DRAW_COLOR);

	cairo_arc(context, x, y, r, gel_convert_grades_to_rads(a1), gel_convert_grades_to_rads(a2));

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

void dl_set_show_preview(int val) {
	show_preview = val;
}

void dl_draw_text(text *tptr) {
	cairo_text_extents_t te;

	if (tptr->lay != figure_get_current_lay()) {
		if (!all_lays)
			if (tptr->visible != VM_PROJECTION)
				return ;
	}

	cairo_select_font_face(context, tptr->font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(context, tptr->size);

	cairo_text_extents(context, tptr->buffer, &te);
	cairo_move_to(context, tptr->x, tptr->y);

	cairo_set_source_rgb(context, tptr->color_r, tptr->color_g, tptr->color_b);

	if (tptr->visible == VM_SELECTED)
		cl_set_color_fg(context, tptr->visible);
	else if (tptr->visible == VM_PROJECTION)
		cl_set_color_fg(context, tptr->visible);
	cairo_show_text(context, tptr->buffer);

	cairo_arc(context, tptr->x, tptr->y, 3, 0, 2 * G_PI);
	cairo_fill(context);
}
