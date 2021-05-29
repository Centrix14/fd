#include <gtk/gtk.h>

#include "color.h"

void cl_set_color(cairo_t *cr, int color_set) {
	switch (color_set) {
		case CL_DEF_CURS_COLOR:
			cairo_set_source_rgb(cr, 200, 200, 200);
		break;

		case CL_DEF_DRAW_COLOR:
			cairo_set_source_rgb(cr, 0, 200, 200);
		break;

		case CL_DEF_PREVIEW_COLOR:
			cairo_set_source_rgb(cr, 255, 100, 0);
		break;

		case CL_DEF_PROJECTION_COLOR:
			cairo_set_source_rgb(cr, 255, 255, 255);
		break;

		case CL_DEF_SELECTED_COLOR:
			cairo_set_source_rgb(cr, 210, 210, 0);
		break;
	}
}

void cl_set_color_fg(cairo_t *cr, int fg_color_set) {
	int cl_color_set[] = {CL_DEF_DRAW_COLOR, CL_DEF_PREVIEW_COLOR,
		CL_DEF_PROJECTION_COLOR, CL_DEF_SELECTED_COLOR};

	cl_set_color(cr, cl_color_set[fg_color_set-1]);
}

void cl_set_color_rgb(cairo_t *cr, double r, double g, double b) {
	cairo_set_source_rgb(cr, r, g, b);
}
