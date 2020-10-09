#ifndef __FLAT_DRAW_DRAW_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_DRAW_LIBRARY_HEADER_FILE_INCLUDED__

#include "figure.h"
#include "data/list.h"

void dl_draw_figure(figure *fptr);
void dl_draw_figure_list(list *lptr);

void dl_draw_point(figure *fptr);
void dl_draw_line_pp(figure *fptr);
void dl_draw_rect_pp(figure *fptr);
void dl_draw_circle(figure *fptr);
void dl_draw_arc(figure *fptr);

void dl_set_cairo_context(cairo_t *cr);
void dl_set_preview_coords(double px, double py);

int dl_is_need_draw(figure *fptr);

void dl_switch_display_all_lays();
void dl_switch_show_preview();

void dl_send_preview_figure(figure *fptr);

void dl_draw_preview();

void dl_set_arc_type(int t);

#endif
