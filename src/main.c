/*
 * main.c -- main fd file, contains gui
 * v0.11
 * 14.10.2020
 * by Centrix
 */

#include <stdio.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "data/list.h"
#include "figure.h"
#include "error.h"

list *figure_list = NULL;
GtkWidget *window;

int main() {
	GtkWidget *main_box, *right_box, *draw_box, *down_tool_box, *left_box;

	GtkWidget *scrolled_window, *draw_area;
	GtkWidget *point_bttn, *line_pp_bttn, *line_la_bttn, *rect_pp_bttn, *rect_wh_bttn, *arc_tp_bttn, *circle_rc_bttn, *help_bttn, *curs_bttn;
	GtkWidget *lay_entry, *set_bttn, *all_bttn, *add_projection_lay_bttn, *crd_label, *hint_label, *option_bttn;
	GtkWidget *save_bttn, *open_bttn, *ver_sep;
	GtkWidget *del_bttn, *copy_paste_bttn, *move_bttn, *rot_bttn, *decouple_bttn;
	GtkWidget *draw_mode_bttn;
	GtkWidget *draw_bttns[DRAW_BUTTONS];
	GtkWidget *line_icon1, *rect_icon1, *arc_icon, *circle_icon, *point_icon, *cursor_icon, *line_icon2, *rect_icon2, *del_icon, *cp_icon, *move_icon, *rot_icon, *decouple_icon;

	figure_list = list_init_node(NULL);

	gtk_init(NULL, NULL);

	// init window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "FlatDraw");
	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
	gtk_window_set_default_icon_from_file("res/fd.png", NULL);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	el_set_par_window(window);

	// init drawing area
	draw_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(draw_area, 1000, 800);

	// init scrolled window
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), draw_area);

	// init buttons
	point_bttn = gtk_button_new_with_label("Point");
	line_pp_bttn = gtk_button_new_with_label("Line (free)");
	line_la_bttn = gtk_button_new_with_label("Line (prmt)");
	rect_pp_bttn = gtk_button_new_with_label("Rect (free)");
	rect_wh_bttn = gtk_button_new_with_label("Rect (prmt)");
	arc_tp_bttn = gtk_button_new_with_label("Arc (free)");
	circle_rc_bttn = gtk_button_new_with_label("Circle (free)");
	help_bttn = gtk_button_new_with_label("Help");
	curs_bttn = gtk_button_new_with_label("Cursor");

	g_signal_connect(G_OBJECT(point_bttn), "clicked", G_CALLBACK(point_bttn_click), NULL);
	g_signal_connect(G_OBJECT(line_pp_bttn), "clicked", G_CALLBACK(line_bttn_click), NULL);
	g_signal_connect(G_OBJECT(line_la_bttn), "clicked", G_CALLBACK(line_la_bttn_click), window);
	g_signal_connect(G_OBJECT(rect_pp_bttn), "clicked", G_CALLBACK(rect_pp_bttn_click), NULL);
	g_signal_connect(G_OBJECT(rect_wh_bttn), "clicked", G_CALLBACK(rect_wh_bttn_click), window);
	g_signal_connect(G_OBJECT(circle_rc_bttn), "clicked", G_CALLBACK(circle_bttn_click), NULL);
	g_signal_connect(G_OBJECT(arc_tp_bttn), "clicked", G_CALLBACK(arc_bttn_click), window);

	g_signal_connect(G_OBJECT(help_bttn), "clicked", G_CALLBACK(help_bttn_click), window);

	g_signal_connect(G_OBJECT(curs_bttn), "clicked", G_CALLBACK(curs_bttn_click), NULL);

	// create icon for buttons
	line_icon1 = gtk_image_new_from_file("res/line.png");
	rect_icon1 = gtk_image_new_from_file("res/rectangle.png");
	line_icon2 = gtk_image_new_from_file("res/line.png");
	rect_icon2 = gtk_image_new_from_file("res/rectangle.png");
	arc_icon = gtk_image_new_from_file("res/arc.png");
	circle_icon = gtk_image_new_from_file("res/ellipse.png");
	point_icon = gtk_image_new_from_file("res/pointer.png");
	cursor_icon = gtk_image_new_from_file("res/cursor.png");
	del_icon = gtk_image_new_from_file("res/del.png");
	cp_icon = gtk_image_new_from_file("res/cp.png");
	move_icon = gtk_image_new_from_file("res/exclude.png");
	rot_icon = gtk_image_new_from_file("res/rotate.png");
	decouple_icon = gtk_image_new_from_file("res/decouple.png");

	// set icons
	// line_pp
	gtk_button_set_image(GTK_BUTTON(line_pp_bttn), line_icon1);
	gtk_button_set_always_show_image(GTK_BUTTON(line_pp_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(line_pp_bttn), GTK_POS_TOP);

	// line_la
	gtk_button_set_image(GTK_BUTTON(line_la_bttn), line_icon2);
	gtk_button_set_always_show_image(GTK_BUTTON(line_la_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(line_la_bttn), GTK_POS_TOP);

	// rect_pp
	gtk_button_set_image(GTK_BUTTON(rect_pp_bttn), rect_icon1);
	gtk_button_set_always_show_image(GTK_BUTTON(rect_pp_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(rect_pp_bttn), GTK_POS_TOP);

	// rect_wh
	gtk_button_set_image(GTK_BUTTON(rect_wh_bttn), rect_icon2);
	gtk_button_set_always_show_image(GTK_BUTTON(rect_wh_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(rect_wh_bttn), GTK_POS_TOP);

	// arc
	gtk_button_set_image(GTK_BUTTON(arc_tp_bttn), arc_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(arc_tp_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(arc_tp_bttn), GTK_POS_TOP);

	// circle
	gtk_button_set_image(GTK_BUTTON(circle_rc_bttn), circle_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(circle_rc_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(circle_rc_bttn), GTK_POS_TOP);

	// cursor
	gtk_button_set_image(GTK_BUTTON(curs_bttn), cursor_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(curs_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(curs_bttn), GTK_POS_TOP);

	// point
	gtk_button_set_image(GTK_BUTTON(point_bttn), point_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(point_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(point_bttn), GTK_POS_TOP);

	// init down tool panel widgets
	lay_entry = gtk_entry_new();
	set_bttn = gtk_button_new_with_label("Set");
	all_bttn = gtk_button_new_with_label("All");
	add_projection_lay_bttn = gtk_button_new_with_label("+ / -");
	option_bttn = gtk_button_new_with_label("Options");
	draw_mode_bttn = gtk_button_new_with_label("free");

	crd_label = gtk_label_new("(0; 0)");
	hint_label = gtk_label_new("Select tool");

	// fills draw bttns
	draw_bttns[0] = point_bttn;
	draw_bttns[1] = line_pp_bttn;
	draw_bttns[2] = line_la_bttn;
	draw_bttns[3] = rect_pp_bttn;
	draw_bttns[4] = rect_wh_bttn;

	g_signal_connect(G_OBJECT(set_bttn), "clicked", G_CALLBACK(set_lay_bttn_click), lay_entry);
	g_signal_connect(G_OBJECT(all_bttn), "clicked", G_CALLBACK(all_bttn_click), lay_entry);
	g_signal_connect(G_OBJECT(add_projection_lay_bttn), "clicked", G_CALLBACK(add_projection_lay_bttn_click), lay_entry);
	g_signal_connect(G_OBJECT(option_bttn), "clicked", G_CALLBACK(options_bttn_click), window);
	g_signal_connect(G_OBJECT(draw_mode_bttn), "clicked", G_CALLBACK(prm_bttn_click), draw_bttns);

	// file operations widgets
	save_bttn = gtk_button_new_with_label("Save");
	open_bttn = gtk_button_new_with_label("Open");
	ver_sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);

	g_signal_connect(G_OBJECT(save_bttn), "clicked", G_CALLBACK(save_bttn_click), window);
	g_signal_connect(G_OBJECT(open_bttn), "clicked", G_CALLBACK(open_bttn_click), window);

	// init right box
	right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	
	gtk_box_pack_start(GTK_BOX(right_box), curs_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), point_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), line_pp_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), line_la_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), rect_pp_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), rect_wh_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), arc_tp_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), circle_rc_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_box), help_bttn, TRUE, TRUE, 0);

	// init tools widgets
	del_bttn = gtk_button_new_with_label("Delete");
	copy_paste_bttn = gtk_button_new_with_label("Copy / Paste");
	move_bttn = gtk_button_new_with_label("Move");
	rot_bttn = gtk_button_new_with_label("Rotate");
	decouple_bttn = gtk_button_new_with_label("Decouple");

	g_signal_connect(G_OBJECT(del_bttn), "clicked", G_CALLBACK(del_bttn_click), draw_area);
	g_signal_connect(G_OBJECT(move_bttn), "clicked", G_CALLBACK(move_bttn_click), NULL);
	g_signal_connect(G_OBJECT(copy_paste_bttn), "clicked", G_CALLBACK(cp_bttn_click), NULL);
	g_signal_connect(G_OBJECT(decouple_bttn), "clicked", G_CALLBACK(dc_bttn_click), draw_area);

	// del
	gtk_button_set_image(GTK_BUTTON(del_bttn), del_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(del_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(del_bttn), GTK_POS_TOP);

	// cp
	gtk_button_set_image(GTK_BUTTON(copy_paste_bttn), cp_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(copy_paste_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(copy_paste_bttn), GTK_POS_TOP);

	// move
	gtk_button_set_image(GTK_BUTTON(move_bttn), move_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(move_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(move_bttn), GTK_POS_TOP);

	// rotation
	gtk_button_set_image(GTK_BUTTON(rot_bttn), rot_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(rot_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(rot_bttn), GTK_POS_TOP);

	// decouple
	gtk_button_set_image(GTK_BUTTON(decouple_bttn), decouple_icon);
	gtk_button_set_always_show_image(GTK_BUTTON(decouple_bttn), TRUE);
	gtk_button_set_image_position(GTK_BUTTON(decouple_bttn), GTK_POS_TOP);

	// init left box
	left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	gtk_box_pack_start(GTK_BOX(left_box), del_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(left_box), copy_paste_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(left_box), move_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(left_box), rot_bttn, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(left_box), decouple_bttn, TRUE, TRUE, 0);

	// init draw box
	draw_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(draw_box), left_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(draw_box), scrolled_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(draw_box), right_box, FALSE, FALSE, 0);

	// init down tool box
	down_tool_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(down_tool_box), crd_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(down_tool_box), hint_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(down_tool_box), lay_entry, TRUE, TRUE, 3);
	gtk_box_pack_start(GTK_BOX(down_tool_box), set_bttn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(down_tool_box), all_bttn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(down_tool_box), add_projection_lay_bttn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(down_tool_box), option_bttn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(down_tool_box), draw_mode_bttn, FALSE, FALSE, 0);

	// file widgets
	gtk_box_pack_start(GTK_BOX(down_tool_box), ver_sep, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(down_tool_box), save_bttn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(down_tool_box), open_bttn, FALSE, FALSE, 0);

	// init main_box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box), draw_box, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), down_tool_box, FALSE, FALSE, 0);

	// add events
	g_signal_connect(G_OBJECT(draw_area), "draw", G_CALLBACK(draw_area_draw), NULL);
	g_signal_connect(G_OBJECT(draw_area), "motion-notify-event", G_CALLBACK(mouse_move), crd_label);
	g_signal_connect(G_OBJECT(draw_area), "button-press-event", G_CALLBACK(mouse_click), NULL);

	gtk_widget_add_events(draw_area, GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK);
	gtk_widget_set_can_focus(draw_area, TRUE);

	gtk_container_add(GTK_CONTAINER(window), main_box);
	gtk_widget_show_all(window);
	
	gtk_widget_hide(line_la_bttn);
	gtk_widget_hide(rect_wh_bttn);

	gtk_main();

	list_crawl(figure_list, figure_free_list);
	list_free_list(figure_list);

	return 0;
}
