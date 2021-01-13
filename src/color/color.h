#ifndef __FLAT_DRAW_COLOR_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_COLOR_LIBRARY_HEADER_FILE_INCLUDED__

#include <gtk/gtk.h>

void cl_set_color(cairo_t *cr, int color_set);
void cl_set_color_fg(cairo_t *cr, int fg_color_set);

enum DEF_COLORS {
	CL_DEF_CURS_COLOR = 0,
	CL_DEF_DRAW_COLOR,
	CL_DEF_PREVIEW_COLOR,
	CL_DEF_PROJECTION_COLOR,
	CL_DEF_SELECTED_COLOR
};

#endif
