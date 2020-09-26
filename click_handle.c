#include <stdio.h>
#include <gtk/gtk.h>

#include "data/list.h"
#include "figure.h"
#include "click_handle.h"

static int draw_mode = FG_TYPE_POINT;

void ch_set_draw_mode(int new_mode) {
	draw_mode = new_mode;
}

void ch_click_handler(GtkWidget *draw_area, list *lptr, int x, int y) {
	switch (draw_mode) {
		case FG_TYPE_POINT:
			ch_add_point(draw_area, lptr, x, y);
		break;

		case FG_TYPE_LINE_PP:
			ch_add_line_pp(draw_area, lptr, x, y);
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
		line->visible = 0;

		list_set_data(last, line);

		state = 1;
	}
	else {
		last = list_get_last(lptr);
		line = (figure*)last->data;

		line->a1 = x;
		line->a2 = y;
		line->visible = 1;

		gtk_widget_queue_draw(draw_area);
		state = 0;
	}
}
