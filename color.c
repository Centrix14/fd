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
			cairo_set_source_rgb(cr, 50, 50, 50);
		break;
	}
}
