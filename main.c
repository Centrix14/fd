#include <stdio.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "data/list.h"
#include "figure.h"

list *figure_list = NULL;

int main() {
	GtkWidget *window;
	GtkWidget *main_box, *right_box;
	GtkWidget *scrolled_window, *draw_area;
	GtkWidget *point_bttn, *line_pp_bttn, *line_la_bttn, *rect_pp_bttn;

	figure_list = list_init_node(NULL);

	gtk_init(NULL, NULL);

	// init window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "FlatDraw");
	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// init drawing area
	draw_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(draw_area, 1000, 800);

	g_signal_connect(G_OBJECT(draw_area), "draw", G_CALLBACK(draw_area_draw), NULL);
	g_signal_connect(G_OBJECT(draw_area), "motion-notify-event", G_CALLBACK(mouse_move), NULL);
	g_signal_connect(G_OBJECT(draw_area), "button-press-event", G_CALLBACK(mouse_click), NULL);

	gtk_widget_set_events(draw_area, gtk_widget_get_events(draw_area) | GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK);

	// init scrolled window
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), draw_area);

	// init buttons
	point_bttn = gtk_button_new_with_label("Point");
	line_pp_bttn = gtk_button_new_with_label("Line (PP)");
	line_la_bttn = gtk_button_new_with_label("Line (LA)");
	rect_pp_bttn = gtk_button_new_with_label("Rect (PP)");

	g_signal_connect(G_OBJECT(point_bttn), "clicked", G_CALLBACK(point_bttn_click), NULL);
	g_signal_connect(G_OBJECT(line_pp_bttn), "clicked", G_CALLBACK(line_bttn_click), NULL);
	g_signal_connect(G_OBJECT(line_la_bttn), "clicked", G_CALLBACK(line_la_bttn_click), window);

	// init right box
	right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(right_box), point_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), line_pp_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), line_la_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), rect_pp_bttn, TRUE, TRUE, 0);

	// init main_box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box), scrolled_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), right_box, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(window), main_box);
	gtk_widget_show_all(window);

	gtk_main();

	list_crawl(figure_list, figure_free_list);
	list_free_list(figure_list);

	return 0;
}
