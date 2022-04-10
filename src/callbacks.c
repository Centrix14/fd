/**
 * \file
 * \brief Contains callbacks for FlatDraw interface
 *
 * This file contains callbacks and util functions
 * for FlatDraw interface.
 *
 * \warning Use the functions from this file with great care.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include "fd_core.h"

st_debug_start(1);
st_name("_fd");

#define POS_BOX 0
#define SIZE_BOX 1
#define RESULTS_MULT 3

double curs_x = 0, curs_y = 0,
	   click_x = 0, click_y = 0;
int direction_val = 1, type_val = 1;

static GtkWidget *target_window, *dialog;

/**
 * \brief Callback for the window folding button.
 * \param[bttn] Button, call source.
 * \param[window] The main window.
 */

void hide_bttn_click(GtkWidget *bttn, GtkWidget *window) {
	if (window)
		gtk_window_iconify(GTK_WINDOW(window));
}

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data) {
	list *geometry_buffer = NULL;

	// make bg black
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	// recive message
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	// draw figures
	dl_set_cairo_context(cr);
	list_crawl(geometry_buffer, mol_draw_obj_from_node);
	dl_draw_preview();

	// draw cursor preview
	if (ch_get_draw_mode() != FG_TYPE_NONE) {
		cl_set_color(cr, CL_DEF_CURS_COLOR);
		cairo_arc(cr, curs_x, curs_y, 3, 0, 2 * G_PI);
		cairo_fill(cr);
	}

	// send cairo context
	pl_remove("msg:cairo-context");
	pl_send("msg:cairo-context", &cr, sizeof(cairo_t*));

	return TRUE;
}

gboolean mouse_move(GtkWidget *widget, GdkEvent *event, GtkWidget *crd_label) {
	GdkEventMotion *em = (GdkEventMotion*)event;
	list *geometry_buffer = NULL;
	char crd[256];

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	if (event->type == GDK_MOTION_NOTIFY) {
		curs_x = (double)em->x;
		curs_y = (double)em->y;

		bl_bind(geometry_buffer, &curs_x, &curs_y);
		dl_set_preview_coords(curs_x, curs_y);

		sprintf(crd, "(%g;%g)", curs_x, curs_y);
		gtk_label_set_text(GTK_LABEL(crd_label), crd);

		gtk_widget_queue_draw(widget);
	}

	return TRUE;
}

gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventButton *eb = (GdkEventButton*)event;
	list *geometry_buffer = NULL;

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	if (event->type == GDK_BUTTON_PRESS) {
		switch (eb->button) {
			case GDK_BUTTON_PRIMARY:
				click_x = (double)eb->x;
				click_y = (double)eb->y;

				bl_bind(geometry_buffer, &click_x, &click_y);
				ch_click_handler(widget, geometry_buffer, click_x, click_y);
			break;

			case GDK_BUTTON_SECONDARY:
				if (dl_get_preview()) {
					dl_switch_show_preview();
					ch_set_state(0);
				}
				else
					ch_unselect_last_node();
			
				gtk_widget_queue_draw(widget);
			break;
		}
	}

	return TRUE;
}

gboolean key_press(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventKey *ek = (GdkEventKey*)event;

	switch (ek->keyval) {
		case GDK_KEY_Escape:
			
		break;
	}

	return TRUE;
}

void point_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_POINT);
	hl_set_help(HC_POINT);
}

void line_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_LINE_PP);
	hl_set_help(HC_START_POINT);
}

void line_la_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *lenght_label, *angle_label, *direction_label,
			  *ok_bttn, *help_bttn, *direction_bttn;
	GtkWidget *lenght_box, *angle_box, *bttn_box, *direction_box, *main_box;
	GtkWidget *lenght_entry, *angle_entry;

	dialog = gtk_dialog_new_with_buttons("Line (PRMT)", GTK_WINDOW(parent_window),
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init labels
	lenght_label = gtk_label_new("Lenght");
	angle_label = gtk_label_new("Angle");

	// init entrys
	lenght_entry = gtk_entry_new();
	angle_entry = gtk_entry_new();

	// removing messages
	pl_remove("msg:lenght_entry");
	pl_remove("msg:angle_entry");

	// send message
	pl_send("msg:lenght_entry", &lenght_entry, sizeof(GtkWidget*));
	pl_send("msg:angle_entry", &angle_entry, sizeof(GtkWidget*));

	// init direction widgets
	direction_label = gtk_label_new("Direction");
	direction_bttn = gtk_button_new_with_label("↑");

	g_signal_connect(G_OBJECT(direction_bttn), "clicked", G_CALLBACK(direction_bttn_click), NULL);

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");
	help_bttn = gtk_button_new_with_label("Help");

	// bind signals
	g_signal_connect(G_OBJECT(ok_bttn), "clicked",
			G_CALLBACK(line_la_dialog_ok_bttn_click), NULL);
	g_signal_connect(G_OBJECT(help_bttn), "clicked",
			G_CALLBACK(line_la_dialog_help_bttn_click), dialog);

	// init lenght- angle- boxes
	lenght_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	angle_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack boxes
	gtk_box_pack_start(GTK_BOX(lenght_box), lenght_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(lenght_box), lenght_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(angle_box), angle_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(angle_box), angle_entry, FALSE, FALSE, 5);

	// init direction box
	direction_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack direction box
	gtk_box_pack_start(GTK_BOX(direction_box), direction_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(direction_box), direction_bttn, FALSE, FALSE, 5);

	// init button box
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack button box
	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_box), help_bttn, TRUE, TRUE, 5);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack box
	gtk_box_pack_start(GTK_BOX(main_box), lenght_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), angle_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), direction_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void line_la_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *lenght_entry, *angle_entry;
	static figure line_data;

	// read messages
	lenght_entry = *(GtkWidget**)pl_read("msg:lenght_entry");
	angle_entry = *(GtkWidget**)pl_read("msg:angle_entry");

	// extract data from line & angle entry
	line_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(lenght_entry)));
	line_data.a2 = atof(gtk_entry_get_text(GTK_ENTRY(angle_entry))) * direction_val;

	pl_remove("msg:ext_figure");
	pl_send("msg:ext_figure", &line_data, sizeof(figure));

	ch_set_draw_mode(FG_TYPE_LINE_LA);

	gtk_widget_destroy(dialog);

	hl_set_help(HC_POINT);
}

void rect_pp_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_RECT_PP);
	hl_set_help(HC_START_POINT);
}

void set_lay_bttn_click(GtkWidget *bttn, GtkWidget *spin) {
	int new_lay = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));

	figure_set_current_lay(new_lay);
}

void all_bttn_click(GtkWidget *bttn, gpointer data) {
	dl_switch_display_all_lays();
} 

void set_window(GtkWidget *new_window) {
	target_window = new_window;
}

void save_bttn_click(GtkWidget *bttn, gpointer data) {
	const char *name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(bttn));
	list *geometry_buffer = NULL;

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	fdl_target_file((char*)name);
	fdl_write_from_list(geometry_buffer);
}

void open_bttn_click(GtkWidget *bttn, gpointer data) {
	const char *name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(bttn));
	GtkWidget *drawing_area = NULL;
	list *geometry_buffer = NULL;

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	fdl_target_file((char*)name);
	fdl_read_file(geometry_buffer);

	drawing_area = *(GtkWidget**)pl_read("msg:drawing_area");
	gtk_widget_queue_draw(drawing_area);
}

void rect_wh_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *width_label, *height_label, *direction_label,
			  *ok_bttn, *help_bttn;
	GtkWidget *width_box, *height_box, *direction_box, *bttn_box, *main_box;
	GtkWidget *width_entry, *height_entry, *direction_bttn;

	dialog = gtk_dialog_new_with_buttons("Rect (PRMT)",
			GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init labels
	width_label = gtk_label_new("Width");
	height_label = gtk_label_new("Height");

	// init entrys
	width_entry = gtk_entry_new();
	height_entry = gtk_entry_new();

	// remove messages
	pl_remove("msg:width_entry");
	pl_remove("msg:height_entry");

	// send message
	pl_send("msg:width_entry", &width_entry, sizeof(GtkWidget*));
	pl_send("msg:height_entry", &height_entry, sizeof(GtkWidget*));

	// init direction widgets
	direction_label = gtk_label_new("Direction");
	direction_bttn = gtk_button_new_with_label("↑");

	g_signal_connect(G_OBJECT(direction_bttn), "clicked", G_CALLBACK(direction_bttn_click), NULL);

	// send message
	pl_send("msg:direction_bttn", &direction_bttn, sizeof(GtkWidget*));

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");
	help_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked",
			G_CALLBACK(rect_wh_dialog_ok_bttn_click), NULL);
	g_signal_connect(G_OBJECT(help_bttn), "clicked",
			G_CALLBACK(rect_wh_dialog_help_bttn_click), dialog);

	// init lenght- angle- boxes
	width_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	height_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack boxes
	gtk_box_pack_start(GTK_BOX(width_box), width_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(width_box), width_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(height_box), height_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(height_box), height_entry, FALSE, FALSE, 5);

	// init direction box
	direction_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack direction box
	gtk_box_pack_start(GTK_BOX(direction_box), direction_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(direction_box), direction_bttn, FALSE, FALSE, 5);

	// init button box
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack button box
	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_box), help_bttn, TRUE, TRUE, 5);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack box
	gtk_box_pack_start(GTK_BOX(main_box), width_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), height_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), direction_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void direction_bttn_click(GtkWidget *bttn, gpointer data) {
	const char *text = NULL;

	text = gtk_button_get_label(GTK_BUTTON(bttn));
	if (!strcmp(text, "↑")) {
		gtk_button_set_label(GTK_BUTTON(bttn), "↓");
		direction_val = -1;
	}
	else {
		gtk_button_set_label(GTK_BUTTON(bttn), "↑");
		direction_val = 1;
	}
}

void rect_wh_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *width_entry, *height_entry;
	static figure rect_data;

	// read messages
	width_entry = *(GtkWidget**)pl_read("msg:width_entry");
	height_entry = *(GtkWidget**)pl_read("msg:height_entry");

	rect_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(width_entry)));
	rect_data.a2 = atof(gtk_entry_get_text(GTK_ENTRY(height_entry))) * direction_val;

	pl_remove("msg:ext_figure");
	pl_send("msg:ext_figure", &rect_data, sizeof(figure));
	ch_set_draw_mode(FG_TYPE_RECT_WH);

	gtk_widget_destroy(dialog);

	hl_set_help(HC_POINT);
}

void add_projection_lay_bttn_click(GtkWidget *bttn, GtkWidget *spin) {
	int lay = 0;
	list *geometry_buffer = NULL;

	lay = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	mol_switch_pr_mode_for_lay(geometry_buffer, lay);
}

void help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *help_dialog, *dialog_content;
	GtkWidget *main_box;
	GtkWidget *help_label, *logo;

	PangoAttrList *attrs;
	PangoAttribute *text_size, *text_font, *text_letter_spacing, *text_label_fallback,
				   *text_stretch, *text_font_variant, *text_font_weight;

	help_dialog = gtk_dialog_new_with_buttons("Help", GTK_WINDOW(parent_window),
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(help_dialog));
	g_signal_connect_swapped(help_dialog, "response", G_CALLBACK(gtk_widget_destroy),
			help_dialog);

	// widgets label
	help_label = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(help_label), hl_get_help(HC_MAIN));
	logo = gtk_image_new_from_file("res/fd.png");

	// adding attributes to text
	attrs = pango_attr_list_new();

	text_size = pango_attr_size_new(16);
	text_font = pango_attr_family_new("Monospace");
	text_letter_spacing = pango_attr_letter_spacing_new(1024 * 10);
	text_label_fallback = pango_attr_fallback_new(TRUE);
	text_stretch = pango_attr_stretch_new(PANGO_STRETCH_NORMAL);
	text_font_variant = pango_attr_variant_new(PANGO_VARIANT_NORMAL);
	text_font_weight = pango_attr_weight_new(PANGO_WEIGHT_NORMAL);

	pango_attr_list_insert(attrs, text_size);
	pango_attr_list_insert(attrs, text_font);
	pango_attr_list_insert(attrs, text_letter_spacing);
	pango_attr_list_insert(attrs, text_label_fallback);
	pango_attr_list_insert(attrs, text_stretch);
	pango_attr_list_insert(attrs, text_font_variant);
	pango_attr_list_insert(attrs, text_font_weight);

	gtk_label_set_attributes(GTK_LABEL(help_label), attrs);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_box_pack_start(GTK_BOX(main_box), logo, TRUE, TRUE, 10);
	gtk_box_pack_start(GTK_BOX(main_box), help_label, TRUE, TRUE, 10);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(help_dialog);
}

void circle_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_CIRCLE);

	hl_set_help(HC_CENTER_POINT);
}

void arc_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	ch_set_draw_mode(FG_TYPE_ARC);
}

void curs_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_NONE);
	hl_set_help(HC_CUROR);
}

void unselect(list *node) {
	multi_obj *mo;

	mo = mol_extract(node);

	if (mo && mo->visible == VM_SELECTED) {
		mo->visible = VM_SHOW;
		mol_apply(node, mo);
	}
}

char *get_fig_type(figure *fig) {
	char *types[] = {"Point", "Line", "Line", "Rect", "Rect", "Circle", "Arc", "Circle", "Arc"};

	return types[fig->type];
}

char *extract_type(list *lptr) {
	if (lptr->dt == OT_FIGURE)
		return get_fig_type((figure*)lptr->data);
	else
		return "Text";
}

char *get_figure_mnemonic(list *lptr) {
	list *node = NULL, *target = NULL;
	multi_obj *mo = NULL;

	int j = 0;

	node = lptr;
	while (node) {
		mo = mol_extract(node);
		if (mo && mo->visible == VM_SELECTED) {
			target = node;
			j++;
		}

		node = node->next;
	}

	if (j == 0)
		return "None";
	if (j == 1)
		return extract_type(target);
	else
		return "Group";
}

char *get_figure_pos(list *lptr) {
	static char out[256] = "";
	multi_obj *mo = NULL;
	list *node = NULL;

	node = lptr;
	while (node) {
		mo = mol_extract(node);
		if (mo && mo->visible == VM_SELECTED)
			break;

		mo = NULL;
		node = node->next;
	}

	if (mo)
		sprintf(out, "%g %g", mo->x, mo->y);
	else
		return "";
	return out;
}

char *get_figure_lay(list *lptr) {
	list *sel = NULL;
	multi_obj *mo;
	static char lay[8] = "";

	sel = ul_get_selected_node(lptr);
	if (!sel)
		return "None";
	mo = mol_extract(sel);

	sprintf(lay, "%d", mo->lay);
	return lay;
}

char *get_figure_is_pr(list *lptr) {
	list *sel = NULL;
	multi_obj *mo;

	sel = ul_get_selected_node(lptr);
	if (!sel)
		return "None";
	mo = mol_extract(sel);

	if (mo->pr_mode)
		return "Projection: True";
	return "Projection: False";
}

int get_index_of(int *arr, int size, int val) {
	for (int i = 0; i < size; i++)
		if (arr[i] == val)
			return i;
	return -1;
}

int *get_layers_list(list *buffer) {
	list *node;
	multi_obj *mo;
	int *layers = NULL, i = 0;
	size_t sz = 16;

	node = buffer;
	layers = (int*)calloc(sz, sizeof(int));
	memset(layers, -1, sz);

	while (node) {
		if (!node->data) {
			node = node->next;

			continue;
		}

		mo = mol_extract(node);
		if (get_index_of(layers, i, mo->lay) == -1)
			layers[i++] = mo->lay;

		// check for overflow
		if (i == sz) {
			sz += 8;
			layers = (int*)realloc(layers, sz * sizeof(int));
		}

		node = node->next;
	}

	layers[i] = -1;
	return layers;
}

int get_list_length(int *lst, int end) {
	int i = 0;

	for (i = 0; lst[i] != end; i++) ;
	return i;
}

void fill_layers_list(GtkWidget *lst, list *lptr) {
	int *layers_list = NULL, len = 0;
	GtkWidget **labels = NULL;
	char label_str[64] = "";

	// get list of layers
	layers_list = get_layers_list(lptr);
	len = get_list_length(layers_list, -1);

	// create array of labels
	labels = (GtkWidget**)malloc(len * sizeof(GtkWidget*));
	for (int i = 0; i < len; i++) {
		sprintf(label_str, "Layer: %d", layers_list[i]);

		labels[i] = gtk_label_new(label_str);
		gtk_list_box_prepend(GTK_LIST_BOX(lst), labels[i]);
	}

	free(layers_list);
}

void options_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;

	GtkWidget *mode_notebook;
	GtkWidget *position_page_label, *size_page_label, *color_page_label,
			  *lay_page_label, *group_page_label;

	GtkWidget *position_box_entry, *position_box_set_bttn, *position_box_select,
			  *position_box_help_bttn, *position_box_ok_bttn,
			  *position_box_figure_type_label;
	GtkWidget *position_box, *position_data_box, *position_bttn_box;

	GtkWidget *size_box, *size_format_box, *size_data_box, *size_data_entry_box,
			  *size_data_line_box1, *size_data_line_box2, *size_bttn_box;
	GtkWidget *size_format_box_pp_bttn, *size_format_box_prm_bttn,
			  *size_data_box_set_bttn1, *size_data_box_set_bttn2,
			  *size_data_entry_box_entr1, *size_data_entry_box_entr2,
			  *size_data_box_select_bttn,
			  *size_data_entry_box_label1, *size_data_entry_box_label2,
			  *size_bttn_box_ok_bttn, *size_bttn_box_help_bttn;

	GtkWidget *color_box, *color_red_box, *color_green_box, *color_blue_box,
			  *color_rgb_box,
			  *color_bttn_box;
	GtkWidget *color_red_box_label, *color_red_box_spin,
			  *color_green_box_label, *color_green_box_spin,
			  *color_blue_box_label, *color_blue_box_spin,
			  *color_data_box_color_bttn, *color_data_box_set_bttn,
			  *color_bttn_box_ok_bttn, *color_bttn_box_help_bttn;

	GtkWidget *layer_box, *layer_obj_box, *layer_obj_bttn_box;
	GtkWidget *layer_obj_box_type_label, *layer_obj_box_lay_label,
			  *layer_obj_box_pr_label, *layer_obj_bttn_box_to_0,
			  *layer_obj_bttn_box_to_sel,
			  *layer_box_scroll, *layer_box_list;

	GtkWidget *group_box, *group_data_box, *group_remove_box, *group_set_box,
			  *group_list_box;
	GtkWidget *group_data_box_tag_entry, *group_data_box_group_entry, *group_data_box_move_bttn,
			  *group_remove_box_tag_bttn, *group_remove_box_group_bttn,
			  *group_set_box_tag_bttn, *group_set_box_group_bttn,
			  *group_box_scroll;

	GdkRGBA color;

	list *geometry_buffer = NULL;

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	dialog = gtk_dialog_new_with_buttons("Options", GTK_WINDOW(parent_window),
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// remove message
	pl_remove("msg:options_dialog");

	// send dialog message
	pl_send("msg:options_dialog", &dialog, sizeof(GtkWidget*));

	// create position box widgets
	position_box_entry = gtk_entry_new();
	position_box_set_bttn = gtk_button_new_with_label("Set");
	position_box_select = gtk_button_new_with_label("Select");
	position_box_help_bttn = gtk_button_new_with_label("Help");
	position_box_ok_bttn = gtk_button_new_with_label("OK");
	position_box_figure_type_label = gtk_label_new("");

	// setting values for widgets
	gtk_entry_set_text(GTK_ENTRY(position_box_entry), get_figure_pos(geometry_buffer));
	gtk_label_set_text(GTK_LABEL(position_box_figure_type_label),
			get_figure_mnemonic(geometry_buffer));

	// create position data box
	position_data_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(position_data_box), position_box_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(position_data_box), position_box_set_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(position_data_box), position_box_select, TRUE, TRUE, 5);

	// position bttn box
	position_bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(position_bttn_box), position_box_ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(position_bttn_box), position_box_help_bttn,
			TRUE, TRUE, 5);

	// create position box
	position_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(position_box),
			position_box_figure_type_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(position_box), position_data_box, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(position_box), position_bttn_box, TRUE, FALSE, 5);

	// adding signals
	g_signal_connect(G_OBJECT(position_box_set_bttn), "clicked",
			G_CALLBACK(options_dialog_set_button),
			(char*)gtk_entry_get_text(GTK_ENTRY(position_box_entry)));
	g_signal_connect(G_OBJECT(position_box_select), "clicked",
			G_CALLBACK(options_dialog_select_bttn_click), dialog);
	g_signal_connect(G_OBJECT(position_box_ok_bttn), "clicked",
			G_CALLBACK(options_dialog_ok_bttn_click), dialog);
	
	// create size box widgets
	size_format_box_pp_bttn = gtk_button_new_with_label("PP");
	size_format_box_prm_bttn = gtk_button_new_with_label("PRM");
	size_data_box_set_bttn1 = gtk_button_new_with_label("Set");
	size_data_box_set_bttn2 = gtk_button_new_with_label("Set");
	size_data_box_select_bttn = gtk_button_new_with_label("Select");

	size_data_entry_box_entr1 = gtk_entry_new();
	size_data_entry_box_entr2 = gtk_entry_new();

	size_data_entry_box_label1 = gtk_label_new("Inital point (x y)");
	size_data_entry_box_label2 = gtk_label_new("End point (x y)");

	// remove messages
	pl_remove("msg:size_data_entry1");
	pl_remove("msg:size_data_entry2");
	pl_remove("msg:size_data_box_set_bttn1");
	pl_remove("msg:size_data_box_set_bttn2");
	pl_remove("msg:size_data_entry_box_label1");
	pl_remove("msg:size_data_entry_box_label2");

	// send message
	pl_send("msg:size_data_entry1", &size_data_entry_box_entr1, sizeof(GtkWidget*));
	pl_send("msg:size_data_entry2", &size_data_entry_box_entr2, sizeof(GtkWidget*));
	pl_send("msg:size_data_box_set_bttn1", &size_data_box_set_bttn1, sizeof(GtkWidget*));
	pl_send("msg:size_data_box_set_bttn2", &size_data_box_set_bttn2, sizeof(GtkWidget*));
	pl_send("msg:size_data_entry_box_label1", &size_data_entry_box_label1,
			sizeof(GtkWidget*));
	pl_send("msg:size_data_entry_box_label2", &size_data_entry_box_label2,
			sizeof(GtkWidget*));

	// bind signals
	g_signal_connect(G_OBJECT(size_data_box_set_bttn1), "clicked",
			G_CALLBACK(options_dialog_size_box_set_bttn_click), NULL);
	g_signal_connect(G_OBJECT(size_data_box_set_bttn2), "clicked",
			G_CALLBACK(options_dialog_size_box_set_prm_bttn_click), NULL);
	g_signal_connect(G_OBJECT(size_data_box_select_bttn), "clicked",
			G_CALLBACK(options_dialog_size_box_select_bttn_click), dialog);
	g_signal_connect(G_OBJECT(size_format_box_prm_bttn), "clicked",
			G_CALLBACK(options_dialog_size_format_box_prm_bttn_click),
			size_data_box_select_bttn);
	g_signal_connect(G_OBJECT(size_format_box_pp_bttn), "clicked",
			G_CALLBACK(options_dialog_size_format_box_pp_bttn_click),
			size_data_box_select_bttn);

	// create size format box
	size_format_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(size_format_box), size_format_box_pp_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(size_format_box), size_format_box_prm_bttn,
			TRUE, TRUE, 5);

	// size data line box 1
	size_data_line_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	 
	gtk_box_pack_start(GTK_BOX(size_data_line_box1),
			size_data_entry_box_label1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(size_data_line_box1),
			size_data_entry_box_entr1, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(size_data_line_box1),
			size_data_box_set_bttn1, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(size_data_line_box1),
			size_data_box_set_bttn2, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(size_data_line_box1),
			size_data_box_select_bttn, TRUE, TRUE, 5);

	// size data line box 2
	size_data_line_box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(size_data_line_box2),
			size_data_entry_box_label2, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(size_data_line_box2),
			size_data_entry_box_entr2, TRUE, TRUE, 5);

	// create size data entry box
	size_data_entry_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(size_data_entry_box), size_data_line_box1, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(size_data_entry_box), size_data_line_box2, TRUE, TRUE, 5);

	// create size data box
	size_data_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(size_data_box), size_data_entry_box, TRUE, TRUE, 5);

	// create size bttns
	size_bttn_box_ok_bttn = gtk_button_new_with_label("OK");
	size_bttn_box_help_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(size_bttn_box_ok_bttn), "clicked",
			G_CALLBACK(options_dialog_ok_bttn_click), dialog);

	// create size bttn box
	size_bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(size_bttn_box), size_bttn_box_ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(size_bttn_box), size_bttn_box_help_bttn, TRUE, TRUE, 5);

	// create size box
	size_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(size_box), size_format_box, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(size_box), size_data_box, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(size_box), size_bttn_box, TRUE, FALSE, 5);

	// color_red_box
	color_red_box_label = gtk_label_new("Red value");
	color_red_box_spin = gtk_spin_button_new_with_range(0, 255, 1);
	color_data_box_color_bttn = gtk_color_button_new();
	color_data_box_set_bttn = gtk_button_new_with_label("Set");

	color_red_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(color_red_box), color_red_box_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(color_red_box), color_red_box_spin, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(color_red_box), color_data_box_color_bttn,
			TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(color_red_box), color_data_box_set_bttn, TRUE, FALSE, 5);

	// add signal
	g_signal_connect(G_OBJECT(color_data_box_set_bttn), "clicked",
			G_CALLBACK(options_dialog_color_data_box_set_bttn_click), NULL);
	g_signal_connect(G_OBJECT(color_data_box_color_bttn), "color-set",
			G_CALLBACK(options_dialog_color_data_box_color_bttn_click), NULL);

	// fill color
	gdk_rgba_parse(&color, "rgb(255,255,255)");

	// set color of color button
	gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(color_data_box_color_bttn), &color);

	// color_green_box
	color_green_box_label = gtk_label_new("Green value");
	color_green_box_spin = gtk_spin_button_new_with_range(0, 255, 1);

	color_green_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(color_green_box), color_green_box_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(color_green_box), color_green_box_spin, TRUE, TRUE, 5);

	// color_blue_box
	color_blue_box_label = gtk_label_new("Blue value");
	color_blue_box_spin = gtk_spin_button_new_with_range(0, 255, 1);

	color_blue_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(color_blue_box), color_blue_box_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(color_blue_box), color_blue_box_spin, TRUE, TRUE, 5);

	// color_rgb_box
	color_rgb_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(color_rgb_box), color_red_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(color_rgb_box), color_green_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(color_rgb_box), color_blue_box, TRUE, TRUE, 5);

	// create bttns
	color_bttn_box_ok_bttn = gtk_button_new_with_label("OK");
	color_bttn_box_help_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(color_bttn_box_ok_bttn), "clicked",
			G_CALLBACK(options_dialog_ok_bttn_click), dialog);

	// create color bttn box
	color_bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(color_bttn_box), color_bttn_box_ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(color_bttn_box), color_bttn_box_help_bttn, TRUE, TRUE, 5);

	// color_data_box
	color_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(color_box), color_rgb_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(color_box), color_bttn_box, TRUE, TRUE, 5);

	// send spins
	pl_remove("msg:color_red_box_spin");
	pl_remove("msg:color_green_box_spin");
	pl_remove("msg:color_blue_box_spin");

	pl_send("msg:color_red_box_spin", &color_red_box_spin, sizeof(GtkWidget*));
	pl_send("msg:color_green_box_spin", &color_green_box_spin, sizeof(GtkWidget*));
	pl_send("msg:color_blue_box_spin", &color_blue_box_spin, sizeof(GtkWidget*));

	// create buttons
	layer_obj_bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	layer_obj_bttn_box_to_0 = gtk_button_new_with_label("Move to layer 0");
	layer_obj_bttn_box_to_sel = gtk_button_new_with_label("Move to selected");

	gtk_box_pack_start(GTK_BOX(layer_obj_bttn_box), layer_obj_bttn_box_to_0,
			TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(layer_obj_bttn_box), layer_obj_bttn_box_to_sel,
			TRUE, TRUE, 5);

	g_signal_connect(G_OBJECT(layer_obj_bttn_box_to_0), "clicked",
			G_CALLBACK(options_dialog_layer_obj_bttn_box_to_0), NULL);

	// ! signal for layer_obj_bttn_box_to_sel slithly lower

	// create layer obj widgets
	layer_obj_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	layer_obj_box_type_label = gtk_label_new("Type");
	layer_obj_box_lay_label = gtk_label_new("Lay");
	layer_obj_box_pr_label = gtk_label_new("PR");

	gtk_label_set_justify(GTK_LABEL(layer_obj_box_type_label), GTK_JUSTIFY_LEFT);
	gtk_label_set_justify(GTK_LABEL(layer_obj_box_lay_label), GTK_JUSTIFY_LEFT);
	gtk_label_set_justify(GTK_LABEL(layer_obj_box_pr_label), GTK_JUSTIFY_LEFT);

	gtk_box_pack_start(GTK_BOX(layer_obj_box), layer_obj_box_type_label,
			TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(layer_obj_box), layer_obj_box_lay_label,
			TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(layer_obj_box), layer_obj_box_pr_label,
			TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(layer_obj_box), layer_obj_bttn_box,
			TRUE, TRUE, 5);

	// change labels text
	gtk_label_set_text(GTK_LABEL(layer_obj_box_type_label),
			get_figure_mnemonic(geometry_buffer));
	gtk_label_set_text(GTK_LABEL(layer_obj_box_lay_label),
			get_figure_lay(geometry_buffer));
	gtk_label_set_text(GTK_LABEL(layer_obj_box_pr_label),
		get_figure_is_pr(geometry_buffer));

	// create list box
	layer_box_list = gtk_list_box_new();

	gtk_list_box_set_selection_mode(GTK_LIST_BOX(layer_box_list), GTK_SELECTION_SINGLE);
	
	g_signal_connect(G_OBJECT(layer_obj_bttn_box_to_sel), "clicked",
			G_CALLBACK(options_dialog_layer_obj_bttn_box_to_sel), layer_box_list);

	// fill layer_box_list
	fill_layers_list(layer_box_list, geometry_buffer);

	// create scrolled window for list box
	layer_box_scroll = gtk_scrolled_window_new(NULL, NULL);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(layer_box_scroll),
			GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add(GTK_CONTAINER(layer_box_scroll), layer_box_list);

	// create layer box
	layer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(layer_box), layer_obj_box,
			FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(layer_box), layer_box_scroll,
			TRUE, TRUE, 5);

	// group set box
	group_set_box_tag_bttn = gtk_button_new_with_label("Set tag");
	group_set_box_group_bttn = gtk_button_new_with_label("Set group");
	group_set_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(group_set_box), group_set_box_tag_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(group_set_box), group_set_box_group_bttn, TRUE, TRUE, 5);

	// ! callback for group_set_box_group_bttn in 'group_data_box' section
	// ! callback for group_set_box_tag_bttn in 'group_data_box' section

	// group remove box
	group_remove_box_tag_bttn = gtk_button_new_with_label("Remove tag");
	group_remove_box_group_bttn = gtk_button_new_with_label("Remove group");
	group_remove_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(group_remove_box), group_remove_box_tag_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(group_remove_box), group_remove_box_group_bttn,
			TRUE, TRUE, 5);

	g_signal_connect(G_OBJECT(group_remove_box_tag_bttn), "clicked",
			G_CALLBACK(options_dialog_remove_tag_bttn_click), NULL);
	g_signal_connect(G_OBJECT(group_remove_box_group_bttn), "clicked",
			G_CALLBACK(options_dialog_remove_group_bttn_click), NULL);

	// group data box
	group_data_box_tag_entry = gtk_entry_new();
	group_data_box_group_entry = gtk_entry_new();
	group_data_box_move_bttn = gtk_button_new_with_label("Move to selected");
	group_data_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(group_data_box), group_data_box_tag_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(group_data_box), group_data_box_group_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(group_data_box), group_set_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(group_data_box), group_remove_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(group_data_box), group_data_box_move_bttn, TRUE, TRUE, 5);
	
	g_signal_connect(G_OBJECT(group_set_box_group_bttn), "clicked",
			G_CALLBACK(options_dialog_set_group_bttn_click), group_data_box_group_entry);
	g_signal_connect(G_OBJECT(group_set_box_tag_bttn), "clicked",
			G_CALLBACK(options_dialog_set_tag_bttn_click), group_data_box_tag_entry);

	// ! callback for group_data_box_move_bttn in 'group_box' section

	gtk_entry_set_text(GTK_ENTRY(group_data_box_tag_entry), "Tag");
	gtk_entry_set_text(GTK_ENTRY(group_data_box_group_entry), "Group");
	
	// group box
	group_list_box = gtk_list_box_new();
	group_box_scroll = gtk_scrolled_window_new(NULL, NULL);
	group_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_container_add(GTK_CONTAINER(group_box_scroll), group_list_box);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(group_box_scroll),
			GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);

	gtk_box_pack_start(GTK_BOX(group_box), group_data_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(group_box), group_box_scroll, TRUE, TRUE, 5);

	g_signal_connect(G_OBJECT(group_data_box_move_bttn), "clicked",
			G_CALLBACK(options_dialog_group_data_box_move_bttn_click), group_list_box);

	// set entrys
	options_dialog_get_group_name(group_data_box_group_entry);
	options_dialog_get_tag(group_data_box_tag_entry);

	// fill group_list_box
	options_dialog_fill_group_list(group_list_box);

	// create mode notebook
	mode_notebook = gtk_notebook_new();

	// create labels for pages
	position_page_label = gtk_label_new("Position");
	size_page_label = gtk_label_new("Size");
	color_page_label = gtk_label_new("Color");
	lay_page_label = gtk_label_new("Layer");
	group_page_label = gtk_label_new("Group");

	// pack mode notebook
	gtk_notebook_append_page(GTK_NOTEBOOK(mode_notebook), position_box,
			position_page_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(mode_notebook), size_box, size_page_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(mode_notebook), color_box, color_page_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(mode_notebook), layer_box, lay_page_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(mode_notebook), group_box, group_page_label);
	
	// show dialog
	gtk_container_add(GTK_CONTAINER(dialog_content), mode_notebook);
	gtk_widget_show_all(dialog);

	// hide bttn2
	gtk_widget_hide(size_data_box_set_bttn2);
}

void options_dialog_set_button(GtkWidget *bttn, char *coords) {
	list *node = NULL;
	multi_obj *mo = NULL;
	double x = 0, y = 0;

	sscanf(coords, "%lf %lf", &x, &y);

	node = *(list**)pl_read("msg:geometry_buffer");
	while (node) {
		mo = mol_extract(node);

		if (mo && mo->visible == VM_SELECTED) {
			mo->x = x;
			mo->y = y;

			mol_apply(node, mo);
		}

		node = node->next;
	}
}

void del_bttn_click(GtkWidget *bttn, GtkWidget *da) {
	list *node, *node_next, *node_prev;
	figure *fptr;

	node = *(list**)pl_read("msg:geometry_buffer");
	while (node) {
		fptr = mol_conv_to_figure(node);
		if (!fptr) {
			node = node->next;

			continue;
		}

		if (fptr->visible == VM_SELECTED) {
			node_prev = node->prev;
			node_next = node->next;

			if (node_prev)
				node_prev->next = node_next;
			if (node_next)
				node_next->prev = node_prev;

			figure_free(fptr);
			list_free_node(node);

			node = node_next;

			continue;
		}

		node = node->next;
	}

	gtk_widget_queue_draw(da);
}

void prm_bttn_click(GtkWidget *bttn, GtkWidget *mode_label) {
	GtkWidget *free_mode_box, *prm_mode_box;
	static int mode = 0;

	free_mode_box = *(GtkWidget**)pl_read("msg:free_mode_box");
	prm_mode_box = *(GtkWidget**)pl_read("msg:prm_mode_box");

	if (!mode) {
		gtk_label_set_text(GTK_LABEL(mode_label), "prmt");

		gtk_widget_hide(free_mode_box);
		gtk_widget_show(prm_mode_box);
	}
	else {
		gtk_label_set_text(GTK_LABEL(mode_label), "free");

		gtk_widget_hide(prm_mode_box);
		gtk_widget_show(free_mode_box);
	}

	mode = !mode;
}

void move_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(WM_MOVE);
	hl_set_help(HC_BASE_POINT);
}

void cp_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(WM_CP);
	hl_set_help(HC_BASE_POINT);
}

void dc_bttn_click(GtkWidget *bttn, GtkWidget *draw_area) {
	list *geometry_buffer = NULL;

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	list_crawl(geometry_buffer, cb_dc);

	gtk_widget_queue_draw(draw_area);
}

void cb_dc(list *lptr) {
	figure *fptr = NULL, *elms = NULL, *new = NULL;
	list *last = NULL;

	fptr = figure_get_from_node(lptr);
	if (!fptr)
		return ;

	if (fptr->visible == VM_SELECTED) {
		if (fptr->type == FG_TYPE_RECT_PP) {
			elms = figure_rect_decompose(fptr);

			for (int i = 0; i < 4; i++) {
				list_add_node(lptr);

				last = list_get_last(lptr);

				new = figure_new_point(0, 0);
				figure_copy(new, &elms[i]);
				new->visible = VM_SHOW;

				list_set_data(last, new);
			}

			figure_free(fptr);
			list_set_data(lptr, NULL);
		}
		else {
			el_call_error(ET_WRONG_SELECTING);
		}
	}
}

void rot_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *dialog_box;
	GtkWidget *info_label, *angle_entry, *apply_bttn;

	dialog = gtk_dialog_new_with_buttons("Rotate", GTK_WINDOW(parent_window),
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init widgets
	info_label = gtk_label_new("Specify the angle of rotation (in degrees)");
	angle_entry = gtk_entry_new();
	apply_bttn = gtk_button_new_with_label("Apply");

	g_signal_connect(G_OBJECT(apply_bttn), "clicked", G_CALLBACK(rot_dialog_apply_bttn),
			angle_entry);

	// init main box
	dialog_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(dialog_box), info_label, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(dialog_box), angle_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(dialog_box), apply_bttn, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), dialog_box);
	gtk_widget_show_all(dialog);
}

void rot_dialog_apply_bttn(GtkWidget *bttn, GtkWidget *entry) {
	static figure ang_figure;

	ang_figure.x = atof(gtk_entry_get_text(GTK_ENTRY(entry)));

	pl_remove("msg:ext_figure");
	pl_send("msg:ext_figure", &ang_figure, sizeof(figure));

	gtk_widget_destroy(dialog);

	ch_set_draw_mode(WM_ROTATE);

	hl_set_help(HC_BASE_POINT);
}

void circle_prm_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *radii_label, *radii_entry, *ok_bttn;
	GtkWidget *radii_box, *bttn_box, *main_box;

	dialog = gtk_dialog_new_with_buttons("Circle (PRMT)", GTK_WINDOW(parent_window),
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init labels
	radii_label = gtk_label_new("Circle Radius");

	// init entrys
	radii_entry = gtk_entry_new();

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked",
			G_CALLBACK(circle_dialog_ok_bttn_click), radii_entry);

	// init lenght- angle- boxes
	radii_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack boxes
	gtk_box_pack_start(GTK_BOX(radii_box), radii_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(radii_box), radii_entry, FALSE, FALSE, 5);

	// init button box
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack button box
	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack box
	gtk_box_pack_start(GTK_BOX(main_box), radii_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void circle_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	static figure circle_data;

	circle_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(entry)));

	pl_remove("msg:ext_figure");
	pl_send("msg:ext_figure", &circle_data, sizeof(figure));

	ch_set_draw_mode(FG_TYPE_CIRCLE_PRM);

	hl_set_help(HC_CENTER_POINT);

	gtk_widget_destroy(dialog);
}

void arc_prm_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *ang1_label, *ang2_label,
			  *radii_label,
			  *ok_bttn;
	GtkWidget *entrys[3];
	GtkWidget *ang1_box, *ang2_box, *radii_box, *bttn_box, *main_box;
	GtkWidget *ang1_entry, *ang2_entry, *radii_entry;

	dialog = gtk_dialog_new_with_buttons("Arc (PRMT)",
			GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init labels
	ang1_label = gtk_label_new("First angle");
	ang2_label = gtk_label_new("Second angle");
	radii_label = gtk_label_new("Arc radius");

	// init entrys
	ang1_entry = gtk_entry_new();
	ang2_entry = gtk_entry_new();
	radii_entry = gtk_entry_new();

	// removing messages
	pl_remove("msg:ang1_entry");
	pl_remove("msg:ang2_entry");
	pl_remove("msg:radii_entry");

	// send messages
	pl_send("msg:ang1_entry", &ang1_entry, sizeof(GtkWidget*));
	pl_send("msg:ang2_entry", &ang2_entry, sizeof(GtkWidget*));
	pl_send("msg:radii_entry", &radii_entry, sizeof(GtkWidget*));

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");

	entrys[0] = radii_entry;
	entrys[1] = ang1_entry;
	entrys[2] = ang2_entry;
	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(arc_dialog_ok_bttn_click), entrys);

	// init lenght- angle- boxes
	ang1_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	ang2_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	radii_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack boxes
	gtk_box_pack_start(GTK_BOX(ang1_box), ang1_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(ang1_box), ang1_entry, TRUE, TRUE, 5);

	gtk_box_pack_start(GTK_BOX(ang2_box), ang2_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(ang2_box), ang2_entry, TRUE, TRUE, 5);

	gtk_box_pack_start(GTK_BOX(radii_box), radii_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(radii_box), radii_entry, TRUE, TRUE, 5);

	// init button box
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack button box
	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack box
	gtk_box_pack_start(GTK_BOX(main_box), ang1_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), ang2_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), radii_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void arc_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *ang1_entry, *ang2_entry, *radii_entry;
	static figure arc_data;

	// read messages
	ang1_entry = *(GtkWidget**)pl_read("msg:ang1_entry");
	ang2_entry = *(GtkWidget**)pl_read("msg:ang2_entry");
	radii_entry = *(GtkWidget**)pl_read("msg:radii_entry");

	// radius
	arc_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(radii_entry)));

	// angles
	arc_data.a2 = atof(gtk_entry_get_text(GTK_ENTRY(ang1_entry)));
	arc_data.a3 = atof(gtk_entry_get_text(GTK_ENTRY(ang2_entry)));

	pl_remove("msg:ext_figure");
	pl_send("msg:ext_figure", &arc_data, sizeof(figure));

	ch_set_draw_mode(FG_TYPE_ARC_PRM);

	hl_set_help(HC_CENTER_POINT);

	gtk_widget_destroy(dialog);
}

void text_bttn_click(GtkWidget *bttn, GtkWindow *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *text_view, *ok_bttn, *help_bttn, *ch_color_bttn, *ch_font_bttn;
	GtkWidget *text_size_label, *text_font_label, *text_color_label;
	GtkWidget *bttn_box, *label_size_box, *label_font_box, *label_color_box, *label_opt_box,
			  *main_box, *label_style_box, *label_style_entrys_box;
	GtkWidget *text_size_entry, *text_font_entry, *text_color_entry;
	GtkTextBuffer *text_buffer;
	GdkRGBA color_button_inital_color;
	
	int colors[3] = {-1, -1, -1};
	int font_size = -1;
	char font_name[64] = "";

	dialog = gtk_dialog_new_with_buttons("Text", GTK_WINDOW(parent_window),
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// label options
	text_size_label = gtk_label_new("Text size");
	text_font_label = gtk_label_new("Font");
	text_color_label = gtk_label_new("Color (r g b)");

	text_size_entry = gtk_entry_new();
	text_font_entry = gtk_entry_new();
	text_color_entry = gtk_entry_new();

	// remove messages
	pl_remove("msg:size_entry");
	pl_remove("msg:font_entry");
	pl_remove("msg:color_entry");

	pl_send("msg:size_entry", &text_size_entry, sizeof(GtkWidget*));
	pl_send("msg:font_entry", &text_font_entry, sizeof(GtkWidget*));
	pl_send("msg:color_entry", &text_color_entry, sizeof(GtkWidget*));

	// create text buffer & view
	text_buffer = gtk_text_buffer_new(NULL);
	text_view = gtk_text_view_new_with_buffer(text_buffer);

	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);

	// buttons
	ok_bttn = gtk_button_new_with_label("OK");
	help_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked",
			G_CALLBACK(text_dialog_ok_bttn_click), text_buffer);
	g_signal_connect(G_OBJECT(help_bttn), "clicked",
			G_CALLBACK(text_dialog_help_bttn_click), dialog);

	// create color button
	gdk_rgba_parse(&color_button_inital_color, "rgb(255,255,255)");
	ch_color_bttn = gtk_color_button_new_with_rgba(&color_button_inital_color);

	// set show-editor property of ch_color_bttn to false
	g_object_set(G_OBJECT(ch_color_bttn), "show-editor", FALSE, NULL);

	g_signal_connect(G_OBJECT(ch_color_bttn), "color-set",
			G_CALLBACK(text_dialog_color_button_set), colors);

	// create font button
	ch_font_bttn = gtk_font_button_new_with_font((const gchar*)"Corbel 0");

	g_signal_connect(G_OBJECT(ch_font_bttn), "font-set",
			G_CALLBACK(text_dialog_font_button_set), NULL);

	// pack label size box
	label_size_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(label_size_box), text_size_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(label_size_box), text_size_entry, TRUE, TRUE, 5);

	// pack label font box
	label_font_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(label_font_box), text_font_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(label_font_box), text_font_entry, TRUE, TRUE, 5);

	// pack entry style box
	label_style_entrys_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(label_style_entrys_box), label_size_box, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(label_style_entrys_box), label_font_box, TRUE, FALSE, 5);

	// pack style box
	label_style_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(label_style_box), label_style_entrys_box, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(label_style_box), ch_font_bttn, TRUE, FALSE, 5);

	// pack label color box
	label_color_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(label_color_box), text_color_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(label_color_box), text_color_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(label_color_box), ch_color_bttn, FALSE, FALSE, 5);

	// pack label options box
	label_opt_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(label_opt_box), label_style_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(label_opt_box), label_color_box, FALSE, FALSE, 5);

	// pack button box
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_box), help_bttn, TRUE, TRUE, 5);

	// pack main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(main_box), label_opt_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), text_view, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);

	// remove message
	pl_remove("msg:colors");
	pl_remove("msg:font_size_int");
	pl_remove("msg:font_name_str");

	// send messages
	pl_send("msg:colors", &colors, sizeof(int) * 3);
	pl_send("msg:font_size_int", &font_size, sizeof(int));
	pl_send("msg:font_name_str", &font_name, 64);
}

void text_dialog_ok_bttn_click(GtkWidget *bttn, GtkTextBuffer *tb) {
	GtkWidget *text_font_entry, *text_size_entry, *text_color_entry;
	GtkTextIter start, end;
	char *buf = NULL, *font_name = NULL;
	list *last, *geometry_buffer;
	text *tptr;
	int *colors, font_size;

	// read messages
	text_font_entry = *(GtkWidget**)pl_read("msg:font_entry");
	text_size_entry = *(GtkWidget**)pl_read("msg:size_entry");
	text_color_entry = *(GtkWidget**)pl_read("msg:color_entry");

	colors = (int*)pl_read("msg:colors");
	font_size = *(int*)pl_read("msg:font_size_int");

	// get iterators
	gtk_text_buffer_get_start_iter(tb, &start);
	gtk_text_buffer_get_end_iter(tb, &end);

	// read message
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	buf = gtk_text_buffer_get_text(tb, &start, &end, TRUE);

	// read message
	font_name = (char*)pl_read("msg:font_name_str");

	if (!strlen(font_name))
		strcpy(font_name, gtk_entry_get_text(GTK_ENTRY(text_font_entry)));
	if (font_size == -1)
		font_size = atoi(gtk_entry_get_text(GTK_ENTRY(text_size_entry)));
	if (*colors == -1)
		ul_get_colors(colors, text_color_entry);

	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	last->dt = OT_TEXT;

	tptr = tl_new(font_name, font_size, colors[0], colors[1], colors[2]);
	tl_add_buffer(tptr, buf);

	tptr->visible = VM_NOT_FINISHED;
	tptr->lay = figure_get_current_lay();

	list_set_data(last, tptr);

	gtk_widget_destroy(dialog);

	ch_set_draw_mode(WM_TEXT);
	hl_set_help(HC_INS_POINT);

	// resetting the buffers
	*colors = -1;
	font_size = -1;
	strcpy(font_name, "");
}

void text_dialog_color_button_set(GtkColorButton *bttn, gpointer data) {
	GdkRGBA color;
	int *arr = NULL;

	// get color
	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(bttn), &color);

	// get array of rgb
	arr = (int*)pl_read("msg:colors");

	arr[0] = ul_map(color.red, 0, 1, 0, 255);
	arr[1] = ul_map(color.green, 0, 1, 0, 255);
	arr[2] = ul_map(color.blue, 0, 1, 0, 255);
}

void text_dialog_font_button_set(GtkFontButton *bttn, gpointer data) {
	char *font = NULL, *font_name = NULL;
	char size[256] = "";
	int font_size = 0;

	font = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(bttn));

	// read message
	font_name = (char*)pl_read("msg:font_name_str");
	strcpy(font_name, "");
	ul_pars_font(font, font_name, size);

	font_size = atoi(size);

	// remove & send message
	pl_remove("msg:font_size_int");
	pl_send("msg:font_size_int", &font_size, sizeof(int));
}

void select_point_cb(list *node, double x, double y) {
	GtkWidget *drawing_area = NULL;
	multi_obj *mo = NULL;

	mo = mol_extract(node);
	if (mo) {
		mo->x = x;
		mo->y = y;

		mol_apply(node, mo);
	}

	drawing_area = *(GtkWidget**)pl_read("msg:drawing_area");
	gtk_widget_queue_draw(drawing_area);
}

void options_dialog_select_bttn_click(GtkWidget *bttn, GtkWidget *dialog) {
	gtk_widget_destroy(dialog);

	ch_set_draw_mode(S_LAST_SELECTED_OP);
	ch_set_last_node_cb(select_point_cb);
}

void options_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *dialog) {
	gtk_widget_destroy(dialog);
}

void options_dialog_size_box_set_bttn_click(GtkWidget *bttn, gpointer data) {
	int coords[4] = {0, 0, 0, 0};
	char buf1[256] = "", buf2[256] = "";
	GtkWidget *entry1, *entry2, *draw_area;
	list *node = NULL;
	multi_obj *mo = NULL;
	figure *fptr = NULL;

	// read messages
	entry1 = *(GtkWidget**)pl_read("msg:size_data_entry1");
	entry2 = *(GtkWidget**)pl_read("msg:size_data_entry2");

	// get strings from entrys
	strcpy(buf1, (char*)gtk_entry_get_text(GTK_ENTRY(entry1)));
	strcpy(buf2, (char*)gtk_entry_get_text(GTK_ENTRY(entry2)));

	// get coords from bufs
	ul_pars_coords(buf1, 2, &coords[0]);
	ul_pars_coords(buf2, 2, &coords[2]);

	// changing coords
	node = *(list**)pl_read("msg:geometry_buffer");
	while (node) {
		mo = mol_extract(node);

		if (!mo || node->dt != OT_FIGURE) {
			node = node->next;

			continue;
		}

		if (mo->visible == VM_SELECTED) {
			fptr = figure_get_from_node(node);

			fptr->x = coords[0];
			fptr->y = coords[1];
			fptr->a1 = coords[2];
			fptr->a2 = coords[3];
		}

		node = node->next;
	}

	// read message
	draw_area = *(GtkWidget**)pl_read("msg:drawing_area");

	gtk_widget_queue_draw(draw_area);
}

void select_size_cb(list *node, double x, double y) {
	static int state = 0;
	GtkWidget *draw_area = NULL;
	figure *fptr = NULL;

	if (node->dt != OT_FIGURE)
		return ;

	fptr = figure_get_from_node(node);
	if (!state) {
		fptr->x = x;
		fptr->y = y;
	}
	else {
		fptr->a1 = x;
		fptr->a2 = y;
	}

	// change state
	state = !state;

	// read mesage
	draw_area = *(GtkWidget**)pl_read("msg:drawing_area");

	gtk_widget_queue_draw(draw_area);
}

void options_dialog_size_box_select_bttn_click(GtkWidget *bttn, GtkWidget *dialog) {
	gtk_widget_destroy(dialog);

	ch_set_draw_mode(S_LAST_SELECTED_OP);
	ch_set_last_node_cb(select_size_cb);
	ch_set_spec_hold_clicks(2);
}

void options_dialog_size_box_set_prm_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *entry1, *entry2, *draw_area;
	list *geometry_buffer = NULL, *node = NULL;
	multi_obj *mo = NULL;
	figure *fptr = NULL;
	
	int width = 0, height = 0;
	char *buf1 = NULL, *buf2 = NULL;

	// read messages
	entry1 = *(GtkWidget**)pl_read("msg:size_data_entry1");
	entry2 = *(GtkWidget**)pl_read("msg:size_data_entry2");

	// get strings from entrys
	buf1 = (char*)gtk_entry_get_text(GTK_ENTRY(entry1));
	buf2 = (char*)gtk_entry_get_text(GTK_ENTRY(entry2));

	// get parametrs from buffers
	width = atof(buf1);
	height = atof(buf2);

	// changing coords
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	node = geometry_buffer;
	while (node) {
		mo = mol_extract(node);

		if (!mo || node->dt != OT_FIGURE) {
			node = node->next;

			continue;
		}

		if (mo->visible == VM_SELECTED) {
			fptr = figure_get_from_node(node);

			fptr->a1 = fptr->x + width;
			fptr->a2 = fptr->y + height;
		}

		node = node->next;
	}

	// read message
	draw_area = *(GtkWidget**)pl_read("msg:drawing_area");

	gtk_widget_queue_draw(draw_area);
}

void options_dialog_size_format_box_prm_bttn_click(GtkWidget *bttn, GtkWidget *select_bttn) {
	GtkWidget *size_data_box_set_bttn1 = NULL, *size_data_box_set_bttn2 = NULL;
	GtkWidget *label1, *label2;

	// read message
	size_data_box_set_bttn1 = *(GtkWidget**)pl_read("msg:size_data_box_set_bttn1");
	size_data_box_set_bttn2 = *(GtkWidget**)pl_read("msg:size_data_box_set_bttn2");
	label1 = *(GtkWidget**)pl_read("msg:size_data_entry_box_label1");
	label2 = *(GtkWidget**)pl_read("msg:size_data_entry_box_label2");

	// hide select bttn
	gtk_widget_hide(select_bttn);

	// hide bttn1
	gtk_widget_hide(size_data_box_set_bttn1);

	// show bttn2
	gtk_widget_show(size_data_box_set_bttn2);

	// set label text
	gtk_label_set_text(GTK_LABEL(label1), "Width");
	gtk_label_set_text(GTK_LABEL(label2), "Height");
}

void options_dialog_size_format_box_pp_bttn_click(GtkWidget *bttn,
		GtkWidget *select_bttn) {
	GtkWidget *size_data_box_set_bttn1 = NULL, *size_data_box_set_bttn2 = NULL;
	GtkWidget *label1, *label2;

	// read message
	size_data_box_set_bttn1 = *(GtkWidget**)pl_read("msg:size_data_box_set_bttn1");
	size_data_box_set_bttn2 = *(GtkWidget**)pl_read("msg:size_data_box_set_bttn2");
	label1 = *(GtkWidget**)pl_read("msg:size_data_entry_box_label1");
	label2 = *(GtkWidget**)pl_read("msg:size_data_entry_box_label2");

	// hide select_bttn
	gtk_widget_show(select_bttn);

	// hide bttn1
	gtk_widget_hide(size_data_box_set_bttn1);

	// show bttn2
	gtk_widget_show(size_data_box_set_bttn2);

	// set label text
	gtk_label_set_text(GTK_LABEL(label1), "Inital point (x y)");
	gtk_label_set_text(GTK_LABEL(label2), "End point (x y)");
}

void line_la_dialog_help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	hl_show_help_by_dialog(HC_LINE_PRMT, parent_window);
}

void rect_wh_dialog_help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	hl_show_help_by_dialog(HC_RECT_PRMT, parent_window);
}

void text_dialog_help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	hl_show_help_by_dialog(HC_TEXT, parent_window);
}

void options_dialog_color_data_box_set_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *red_spin, *green_spin, *blue_spin;
	list *geometry_buffer, *sel;
	double red, green, blue;

	// read msg
	red_spin = *(GtkWidget**)pl_read("msg:color_red_box_spin");
	green_spin = *(GtkWidget**)pl_read("msg:color_green_box_spin");
	blue_spin = *(GtkWidget**)pl_read("msg:color_blue_box_spin");

	// get value
	red = (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(red_spin));
	green = (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(green_spin));
	blue = (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(blue_spin));

	// get geometry buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	// get selected
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel) {
		el_call_error(ET_WRONG_SELECTING);
		return ;
	}

	// add options
	ol_check_options(sel);
	ol_set_color(sel, red, green, blue);
}

void options_dialog_color_data_box_color_bttn_click(GtkWidget *bttn, gpointer data) {
	GdkRGBA color;
	int red, green, blue;
	list *geometry_buffer, *sel;

	// get color from button
	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(bttn), &color);

	// get colors
	red = ul_map(color.red, 0, 1, 0, 255);
	green = ul_map(color.green, 0, 1, 0, 255);
	blue = ul_map(color.blue, 0, 1, 0, 255);

	// get geometry_buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	// get selected figure
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel) {
		el_call_error(ET_WRONG_SELECTING);

		return ;
	}

	// add options
	ol_check_options(sel);
	ol_set_color(sel, red, green, blue);
}

void options_dialog_layer_obj_bttn_box_to_0(GtkWidget *bttn, gpointer data) {
	list *geometry_buffer = NULL, *sel = NULL;
	multi_obj *mo;

	// get geometry buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");

	// get selected figure
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel) {
		el_call_error(ET_WRONG_SELECTING);

		return ;
	}

	// move figure to layer 0
	mo = mol_extract(sel);
	mo->lay = 0;
	mol_apply(sel, mo);
} 

void options_dialog_layer_obj_bttn_box_to_sel(GtkWidget *bttn, GtkWidget *layers_list) {
	GtkListBoxRow *selected_row;
	GtkWidget *selected_widget;
	list *geometry_buffer = NULL, *sel = NULL;
	multi_obj *mo;
	int new_lay = 0;

	// get selected row
	selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(layers_list));
	selected_widget = gtk_bin_get_child(GTK_BIN(selected_row));

	// get selected node
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel) {
		el_call_error(ET_WRONG_SELECTING);

		return ;
	}
	
	// move node to new lay
	new_lay = ul_pars_layer_str((char*)gtk_label_get_text(GTK_LABEL(selected_widget)));
	mo = mol_extract(sel);
	mo->lay = new_lay;
	mol_apply(sel, mo);
} 

void options_dialog_get_group_name(GtkWidget *entry) {
	list *sel = NULL, *geometry_buffer = NULL;
	options *opt;
	char *group_name = NULL;

	// get gb and selected node
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel)
		return ;

	// get group
	opt = ol_get_opt(sel);
	if (!opt || !opt->group)
		return ;
	else
		group_name = opt->group;

	// set entry text
	gtk_entry_set_text(GTK_ENTRY(entry), group_name);
}

void options_dialog_get_tag(GtkWidget *entry) {
	list *sel = NULL, *geometry_buffer = NULL;
	options *opt;
	char *tag = NULL;

	// get gb and selected node
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel)
		return ;

	// get tag
	opt = ol_get_opt(sel);
	if (!opt || !opt->tag)
		return ;
	else
		tag = opt->tag;

	// set entry text
	gtk_entry_set_text(GTK_ENTRY(entry), tag);
}

void options_dialog_set_group_bttn_click(GtkWidget *bttn, GtkWidget *group_entry) {
	list *geometry_buffer = NULL, *sel = NULL;
	options *opt;
	char *group_name = "";

	// get geometry buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel) {
		el_call_error(ET_WRONG_SELECTING);

		return ;
	}

	// get options
	ol_check_options(sel);
	opt = ol_get_opt(sel);
	if (!opt) {
		el_call_error(ET_CREATE_OPTIONS_FAIL);

		return ;
	}

	// get group_name
	group_name = (char*)gtk_entry_get_text(GTK_ENTRY(group_entry));
	if (!strlen(group_name))
		return ;

	// allocate group field
	if (opt->group)
		opt->group = (char*)realloc(opt->group, strlen(group_name) + 1);
	else
		opt->group = (char*)malloc(strlen(group_name) + 1);

	// copy
	strcpy(opt->group, group_name);
}

void options_dialog_set_tag_bttn_click(GtkWidget *bttn, GtkWidget *tag_entry) {
	list *geometry_buffer = NULL, *sel = NULL;
	options *opt;
	char *tag_name = "";

	// get geometry buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);
	if (!sel) {
		el_call_error(ET_WRONG_SELECTING);

		return ;
	}

	// get options
	ol_check_options(sel);
	opt = ol_get_opt(sel);
	if (!opt) {
		el_call_error(ET_CREATE_OPTIONS_FAIL);

		return ;
	}

	// get tag_name
	tag_name = (char*)gtk_entry_get_text(GTK_ENTRY(tag_entry));
	if (!strlen(tag_name)) {
		el_call_error(ET_WRONG_DATA);
		return ;
	}

	// allocate group field
	if (opt->tag)
		opt->tag = (char*)realloc(opt->tag, strlen(tag_name) + 1);
	else
		opt->tag = (char*)malloc(strlen(tag_name) + 1);

	// copy
	strcpy(opt->tag, tag_name);
}

void options_dialog_remove_tag_bttn_click(GtkWidget *bttn, gpointer data) {
	list *geometry_buffer = NULL, *sel = NULL;
	options *opt = NULL;

	// get geometry_buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);

	// get options
	opt = ol_get_opt(sel);
	if (!opt || !opt->tag) {
		el_call_error(ET_CREATE_OPTIONS_FAIL);

		return ;
	}

	// remove tag
	free(opt->tag);
	opt->tag = NULL;
}

void options_dialog_remove_group_bttn_click(GtkWidget *bttn, gpointer data) {
	list *geometry_buffer = NULL, *sel = NULL;
	options *opt = NULL;

	// get geometry_buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);

	// get options
	opt = ol_get_opt(sel);
	if (!opt || !opt->group) {
		el_call_error(ET_CREATE_OPTIONS_FAIL);

		return ;
	}

	// remove tag
	free(opt->group);
	opt->group = NULL;
}

int find_str(char **arr, char *elm, int size) {
	for (int i = 0; i < size; i++)
		if (arr[i] && !strcmp(arr[i], elm))
			return i;
	return -1;
}

void free_str_arr(char **arr, int size) {
	for (int i = 0; i < size; i++)
		if (arr[i])
			free(arr[i]);
	free(arr);
}

void fill_arr(char **arr, int start, int end, char *val) {
	for (int i = start; i < end; i++)
		arr[i] = val;
}

void options_dialog_fill_group_list(GtkWidget *group_list) {
	GtkWidget *label = NULL;
	list *geometry_buffer = NULL, *lptr = NULL;
	options *opt = NULL;
	char *group_name = NULL, **group_names_list = NULL;
	int i = 0, sz = 8;

	// get geometry_buffer
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	lptr = geometry_buffer;

	// allocate list
	group_names_list = (char**)malloc(sz * sizeof(char**));
	memset(group_names_list, 0, sz * sizeof(char**));

	// main loop
	while (lptr) {
		opt = ol_get_opt(lptr);
		if (opt && opt->group) {
			group_name = opt->group;

			if (find_str(group_names_list, group_name, sz) == -1) {
				// add group_name to list
				group_names_list[i] = malloc(strlen(group_name) + 1);
				strcpy(group_names_list[i], group_name);
				
				// create label
				label = gtk_label_new(group_name);
				gtk_list_box_prepend(GTK_LIST_BOX(group_list), label);
			}
		}

		// realloc if overflow
		i++;
		if (i == sz) {
			sz += 8;

			group_names_list = (char**)realloc(group_names_list, sz * sizeof(char**));
			fill_arr(group_names_list, i, sz, NULL);
		}

		lptr = lptr->next;
	}

	free_str_arr(group_names_list, sz);
}

void options_dialog_group_data_box_move_bttn_click(GtkWidget *bttn, GtkWidget *list_box) {
	GtkListBoxRow *row;
	GtkWidget *selected_label;
	list *geometry_buffer, *sel;
	options *opt;
	char *group = NULL;

	// get selected label
	row = gtk_list_box_get_selected_row(GTK_LIST_BOX(list_box));
	selected_label = gtk_bin_get_child(GTK_BIN(row));

	// get selected node
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node(geometry_buffer);

	// get options
	ol_check_options(sel);
	opt = ol_get_opt(sel);

	// get group
	group = (char*)gtk_label_get_text(GTK_LABEL(selected_label));
	if (!group) {
		el_call_error(ET_CREATE_OPTIONS_FAIL);

		return ;
	}

	// set group
	opt->group = (char*)malloc(strlen(group) + 1);
	strcpy(opt->group, group);
}

int __check_f(list *node) {
	multi_obj *mo = NULL;

	mo = mol_extract(node);
	if (!mo || mo->visible != VM_SELECTED)
		return 0;
	return 1;
}

int __get_selected_len(list *results) {
	list *start = NULL;

	start = results->next;
	if (!start)
		return 0;
	else if (!start->next)
		return 1;
	else if (!start->next->next)
		return 2;
	return RESULTS_MULT;
}

void __show_size_of_figure(char *msg) {
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new(NULL, (GtkDialogFlags)NULL, GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK, msg);
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	gtk_widget_show_all(dialog);
}

void __get_size_of_1(list *results) {
	list *node = NULL;
	char *info = NULL;

	node = results->next;
	if (!node)
		return ;

	switch (node->dt) {
		case OT_FIGURE:
			info = figure_get_size_info((figure*)node->data);
			__show_size_of_figure(info);
		break;

		case OT_TEXT:
			info = tl_get_info((text*)node->data);
			__show_size_of_figure(info);
		break;
	}
}

char *__get_size_of_pair(figure *fptr1, figure *fptr2) {
	double dx = 0, dy = 0, d = 0;
	static char buf[64] = "";

	dx = fabs(fptr1->x - fptr2->x);
	dy = fabs(fptr1->y - fptr2->y);

	d = sqrt(pow(dx, 2) + pow(dy, 2));

	sprintf(buf, "%g", d);

	return buf;
}

void __get_size_of_2(list *results) {
	figure *fptr1 = NULL, *fptr2 = NULL;
	list *node = NULL;
	char *info = NULL;

	node = results->next;
	if (!node)
		return ;

	if (node->dt == OT_FIGURE) {
		fptr1 = (figure*)node->data;
		fptr2 = (figure*)node->next->data;

		if (fptr1->type == FG_TYPE_POINT && fptr2->type == FG_TYPE_POINT)
			info = __get_size_of_pair(fptr1, fptr2);
		else
			info = "The objects must be points.";
	}
	else
		info = "The objects must be figures.";

	__show_size_of_figure(info);
}

void __get_size_of_group(list *results) {
	__show_size_of_figure("Measuring the size of the group is not yet available.");
}
 
void __check_selected(list *results) {
	int len = 0;

	len = __get_selected_len(results);
	switch (len) {
		case 0:
			return ;
		break;

		case 1:
			__get_size_of_1(results);
		break;

		case 2:
			__get_size_of_2(results);
		break;

		default:
			__get_size_of_group(results);
		break;
	}
}

void size_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	list *geometry_buffer = NULL, *sel = NULL;

	// get geometry_buffer and selected nodes
	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	sel = ul_get_selected_node_multiple(geometry_buffer, __check_f);

	// check selected
	__check_selected(sel);
}

void plugin_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *dialog;
	GtkWidget *content_box;
	GtkWidget *main_box;

	GtkWidget *plugin_path_entry, *plugin_run_bttn;

	dialog = gtk_dialog_new_with_buttons("Plugins", NULL,
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);

	content_box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	plugin_path_entry = gtk_entry_new();
	plugin_run_bttn = gtk_button_new_with_label("Run");

	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), plugin_path_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), plugin_run_bttn, FALSE, FALSE, 5);

	g_signal_connect(G_OBJECT(plugin_run_bttn), "clicked",
			G_CALLBACK(plugin_run_bttn_click), plugin_path_entry);

	gtk_container_add(GTK_CONTAINER(content_box), main_box);
	gtk_widget_show_all(dialog);
}

void plugin_run_bttn_click(GtkWidget *bttn, GtkWidget *plugin_path_entry) {
	char *path = NULL, *path_buf = NULL;
	list *msg_list = NULL;

	// create path duplicate
	path = (char*)gtk_entry_get_text(GTK_ENTRY(plugin_path_entry));
	path_buf = (char*)malloc(strlen(path) + 1);
	if (!path_buf) {
		el_call_error(ET_FAIL_TO_ALLOCATE_MEMORY_FOR_TEXT_BUFFER);

		return ;
	}
	strcpy(path_buf, path);

	// call plugin
	msg_list = pl_get_msg_list();
	pil_call_plugin(path_buf, "plug_call", msg_list);

	// free path duplicate
	free(path_buf);
}

void load_proc_model_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog;
	GtkWidget *content_box, *main_box, *bttn_box;
	GtkWidget *load_lua_model_bttn, *load_cmodel_model_bttn, *path_entry;

	dialog = gtk_dialog_new_with_buttons("Load procedural model", GTK_WINDOW(parent_window),
			(GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	content_box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	// widgets
	load_lua_model_bttn = gtk_button_new_with_label("Load Lua model");
	load_cmodel_model_bttn = gtk_button_new_with_label("Load CModel model");

	path_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(path_entry), "path");

	// signals
	g_signal_connect(G_OBJECT(load_cmodel_model_bttn), "clicked",
			G_CALLBACK(load_cmodel_model_bttn_click), path_entry);
	g_signal_connect(G_OBJECT(load_lua_model_bttn), "clicked",
			G_CALLBACK(load_lua_model_bttn_click), path_entry);

	// pack buttons
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(bttn_box), load_lua_model_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_box), load_cmodel_model_bttn, TRUE, TRUE, 5);

	// pack main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), path_entry, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(content_box), main_box);
	gtk_widget_show_all(dialog);
}

void load_cmodel_model_bttn_click(GtkWidget *bttn, GtkWidget *path_entry) {
	void (*init)(list*) = NULL, (*model)() = NULL;
	char *path = NULL;

	path = (char*)gtk_entry_get_text(GTK_ENTRY(path_entry));

	sl_open_file(path);

	// run init function
	init = sl_get_function("init");
	init(pl_get_msg_list());

	// run model function
	model = sl_get_function("model");
	model();

	sl_close();
}

void load_lua_model_bttn_click(GtkWidget *bttn, GtkWidget *path_entry) {
	char *path = NULL;
	lua_State *L;
	int ret = 0;

	path = (char*)gtk_entry_get_text(GTK_ENTRY(path_entry));

	L = luaL_newstate();
	if (!L) {
		fprintf(stderr, "Fail to start Lua");

		return ;
	}

	luaL_openlibs(L);
	lml_register(L);

	luaL_loadfile(L, path);
	ret = lua_pcall(L, 0, 0, 0);
	if (ret) {
		fprintf(stderr, "Fail: %s\n", lua_tostring(L, -1));

		return ;
	}

	lua_close(L);
}
