#ifndef __FLAT_DRAW_DRAW_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_DRAW_LIBRARY_HEADER_FILE_INCLUDED__

#include "figure.h"
#include "data/list.h"

void dl_draw_figure(figure *fptr);
void dl_draw_figure_list(list *lptr);

void dl_draw_point(figure *fptr);
void dl_draw_line_pp(figure *fptr);

void dl_set_cairo_context(cairo_t *cr);
void dl_set_preview_coords(int px, int py);

#endif
