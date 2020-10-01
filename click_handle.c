#include <stdio.h>
#include <gtk/gtk.h>

#include "data/list.h"
#include "figure.h"
#include "click_handle.h"
#include "geometry.h"

static int draw_mode = FG_TYPE_POINT;
static figure *ext_figure;

void ch_set_draw_mode(int new_mode) {
	draw_mode = new_mode;
}

void ch_set_external_figure(figure *fptr) {
	ext_figure = fptr;
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
	static int state = 0;	

	list *last;
	figure *line;

	if (!state) {
		list_add_node(lptr);

		last = list_get_last(lptr);
		line = figure_new_line_pp(x, y, 0, 0);
		line->visible = VM_PREVIEW;

		list_set_data(last, line);
	}
	else {
		last = list_get_last(lptr);
		line = (figure*)last->data;

		line->a1 = x;
		line->a2 = y;
		line->visible = VM_SHOW;
	}
	
	state = !state;
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
	static int state = 0;

	list *last;
	figure *rect;

	if (!state) {
		list_add_node(lptr);

		last = list_get_last(lptr);
		rect = figure_new_rect_pp(x, y, 0, 0);
		rect->visible = VM_PREVIEW;

		list_set_data(last, rect);
	}
	else {
		last = list_get_last(lptr);
		rect = (figure*)last->data;

		rect->a1 = x;
		rect->a2 = y;
		rect->visible = VM_SHOW;
	}

	state = !state;
	gtk_widget_queue_draw(draw_area);
}
