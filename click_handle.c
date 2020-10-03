#include <stdio.h>
#include <gtk/gtk.h>

#include "data/list.h"
#include "figure.h"
#include "click_handle.h"
#include "geometry.h"
#include "draw.h"

static int draw_mode = FG_TYPE_POINT, state = 0;
static figure *ext_figure,
			  tmp_figure;

void ch_set_draw_mode(int new_mode) {
	draw_mode = new_mode;
}

void ch_set_external_figure(figure *fptr) {
	ext_figure = fptr;
}

void ch_set_state(int new_state) {
	state = new_state;
}

void ch_click_handler(GtkWidget *draw_area, list *lptr, int x, int y) {
	switch (draw_mode) {
		case FG_TYPE_POINT:
			ch_add_point(draw_area, lptr, x, y);
		break;

		case FG_TYPE_LINE_PP:
			ch_add_line_pp(draw_area, lptr, x, y);
		break;

		case FG_TYPE_LINE_LA:
			ch_add_line_la(draw_area, lptr, x, y);
		break;

		case FG_TYPE_RECT_PP:
			ch_add_rect_pp(draw_area, lptr, x, y);
		break;
	}
}

void ch_add_point(GtkWidget *draw_area, list *lptr, int x, int y) {
	list *last;
	figure *point;

	list_add_node(lptr);

	last = list_get_last(lptr);
	point = figure_new_point(x, y);

	list_set_data(last, point);

	gtk_widget_queue_draw(draw_area);
}

void ch_add_line_pp(GtkWidget *draw_area, list *lptr, int x, int y) {
	list *last;
	figure *line;

	if (!state) {
		figure_fill(&tmp_figure, x, y, 0, 0, FG_TYPE_LINE_PP);
		tmp_figure.visible = VM_PREVIEW;

		dl_send_preview_figure(&tmp_figure);
	}
	else {
		list_add_node(lptr);

		last = list_get_last(lptr);
		line = figure_new_line_pp(0, 0, 0, 0);

		figure_copy(line, &tmp_figure);

		line->a1 = x;
		line->a2 = y;

		line->visible = VM_SHOW;

		list_set_data(last, line);
	}
	
	state = !state;
	dl_switch_show_preview();
	gtk_widget_queue_draw(draw_area);
}

void ch_add_line_la(GtkWidget *draw_area, list *lptr, int x, int y) {
	list *last;
	figure *line;

	list_add_node(lptr);

	last = list_get_last(lptr);
	line = figure_new_line_pp(x, y, 0, 0);
	line->visible = VM_SHOW;

	gel_calculate_line_la(line, ext_figure->a1, ext_figure->a2);

	list_set_data(last, line);

	gtk_widget_queue_draw(draw_area);
}

void ch_add_rect_pp(GtkWidget *draw_area, list *lptr, int x, int y) {
	list *last;
	figure *rect;

	if (!state) {
		figure_fill(&tmp_figure, x, y, 0, 0, FG_TYPE_RECT_PP);
		tmp_figure.visible = VM_PREVIEW;

		dl_send_preview_figure(&tmp_figure);	
	}
	else {
		list_add_node(lptr);

		last = list_get_last(lptr);
		rect = figure_new_rect_pp(0, 0, 0, 0);

		figure_copy(rect, &tmp_figure);

		rect->a1 = x;
		rect->a2 = y;

		rect->visible = VM_SHOW;

		list_set_data(last, rect);
	}

	state = !state;
	dl_switch_show_preview();
	gtk_widget_queue_draw(draw_area);
}
