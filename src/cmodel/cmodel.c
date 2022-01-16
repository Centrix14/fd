#include <stdio.h>
#include <string.h>
#include "../fd_core.h"

#include "cmodel.h"

#define __get_buf() \
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer"); \
	if (!geometry_buffer) \
		return NULL;

char *data = 0;

/* core functions */

figure *point(double x, double y) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *point = NULL;

	__get_buf();

	point = figure_new_point(x, y);
	if (!point)
		return NULL;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, point);

	return point;
}

figure *line_xy(double x1, double y1, double x2, double y2) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *line = NULL;

	__get_buf();

	line = figure_new_line_pp(x1, y1, x2, y2);
	if (!line)
		return NULL;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, line);

	return line;
}

figure *line_ar(double x, double y, double angle, double r) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *line = NULL;

	__get_buf();

	line = figure_new_line_pp(x, y, 0, 0);
	if (!line)
		return NULL;
	gel_calculate_line_la(line, r, angle);

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, line);

	return line;
}

figure *rect_xy(double x1, double y1, double x2, double y2) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *rect = NULL;

	__get_buf();

	rect = figure_new_rect_pp(x1, y1, x2, y2);
	if (!rect)
		return NULL;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, rect);

	return rect;
}

figure *rect_wh(double x1, double y1, double w, double h) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *rect = NULL;

	__get_buf();

	rect = figure_new_rect_pp(x1, y1, x1 + w, y1 + h);
	if (!rect)
		return NULL;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, rect);

	return rect;
}

figure *circle(double x, double y, double r) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *circle = NULL;

	__get_buf();

	circle = figure_new_circle(x, y, r);
	if (!circle)
		return NULL;

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	list_set_data(last, circle);

	return circle;
}

text *label(double crd[2], int rgb[3], unsigned int size, char *font, char *str) {
	list *geometry_buffer = NULL, *last = NULL;
	text *tptr = NULL;

	__get_buf();

	tptr = tl_new(font, size, rgb[0], rgb[1], rgb[2]);
	if (!tptr)
		return NULL;

	tl_add_buffer(tptr, str);
	if (!tptr->buffer)
		return NULL;

	tptr->x = crd[0]; tptr->y = crd[1];
	tptr->lay = figure_get_current_lay();

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	last->dt = OT_TEXT;
	list_set_data(last, tptr);

	return tptr;
}

void erase(void *ptr, OBJ_TYPE ot) {
	list *geometry_buffer = NULL, *node = NULL,
		 *node_prev = NULL, *node_next = NULL;

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	if (!geometry_buffer)
		return ;

	node = geometry_buffer;
	while (node) {
		if ((node->dt == ot) && (node->data == ptr)) {
			if (node->dt == OT_FIGURE)
				figure_free_list(node);
			else {
				if (((text*)node)->buffer)
					tl_free_buffer((text*)node->dt);
				tl_free((text*)node->dt);
			}

			node_prev = node->prev;
			node_next = node->next;

			if (node_prev)
				node_prev->next = node_next;
			if (node_next)
				node_next->prev = node_prev;

			list_free_node(node);
			node = node_next;
		}
		else
			node = node->next;
	}
}

/* util functions */

void __info_point(figure *fptr) {
	printf("| point [%g; %g]\n", fptr->x, fptr->y);
}

void __info_line(figure *fptr) {
	printf("| line  [%g; %g] [%g; %g]\n", fptr->x, fptr->y, fptr->a1, fptr->a2);
}

void __info_rect(figure *fptr) {
	printf("| rect  [%g; %g] [%g; %g]\n", fptr->x, fptr->y, fptr->a1, fptr->a2);
}

void __info_circle(figure *fptr) {
	printf("| circle [%g; %g] R = %g\n", fptr->x, fptr->y, fptr->a1);
}

void __info_figure(list *lptr) {
	void (*funcs[])(figure*) = {__info_point, __info_line, __info_line, __info_rect,
	__info_rect, __info_circle};
	figure *fptr = NULL;

	fptr = (figure*)lptr->data;
	if (!fptr)
		return ;
	(*funcs[(int)fptr->type])(fptr);
}

void __info_text(list *lptr) {
	text *tptr = NULL;

	tptr = (text*)lptr->data;
	printf("| label [%g; %g] rgb = [%d; %d; %d] size = %d font = %s \"%s\"\n",
			tptr->x, tptr->y, tptr->color_r, tptr->color_g, tptr->color_b, tptr->size,
			tptr->font, tptr->buffer);
}

void info() {
	list *geometry_buffer = NULL, *lptr = NULL;

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer"); \
	if (!geometry_buffer)
		return ;

	lptr = geometry_buffer;
	while (lptr) {
		if (lptr->dt == OT_FIGURE)
			__info_figure(lptr);
		else
			__info_text(lptr);

		lptr = lptr->next;
	}
}

void __ok_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	static char buf[64] = "";
	char *ptr = NULL;

	ptr = (char*)gtk_entry_get_text(GTK_ENTRY(entry));
	strcpy(buf, ptr);
	data = buf;
}

void __make_window(char *msg) {
	GtkWidget *window;
	GtkWidget *main_box;
	GtkWidget *param_entry, *msg_label, *ok_bttn;

	gtk_init(NULL, NULL);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Get parameter");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 60);

	g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	msg_label = gtk_label_new(msg);
	param_entry = gtk_entry_new();
	ok_bttn = gtk_button_new_with_label("OK");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(__ok_bttn_click), param_entry);

	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), msg_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), param_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), ok_bttn, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(window), main_box);
	gtk_widget_show_all(window);

	gtk_main();
}

char *get_param(char *msg) {
	__make_window(msg);

	return data;
}

/* service functions */

void set_msg_list(list *msg_list) {
	pl_set_msg_list(msg_list);
}
