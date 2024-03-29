#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>

#include "list/list.h"
#include "figure/figure.h"
#include "click_handle.h"
#include "geometry/geometry.h"
#include "draw/draw.h"
#include "callbacks.h"
#include "help/help.h"
#include "multi_obj/multi_obj.h"

#include "pechkin/pl.h"
#include "st.h/st.h"

#define ch_handler void (*)(GtkWidget*, list*, double, double)
#define arr_len(arr, elm) sizeof(arr) / sizeof(elm)

static int draw_mode = FG_TYPE_POINT, state = 0;
static figure tmp_figure;
static int spec_hold_clicks = 0;
list *last_selected_node;

void (*last_node_cb)(list*, double, double) = ch_null_op;

void ch_set_draw_mode(int new_mode) {
	draw_mode = new_mode;
}

int ch_get_draw_mode() {
	return draw_mode;
}

void ch_set_state(int new_state) {
	state = new_state;
}

void ch_set_spec_hold_clicks(int clicks) {
	spec_hold_clicks = clicks;
}

int is_spec_mode() {
	if (draw_mode == S_LAST_SELECTED_OP)
		return 1;
	return 0;
}

void ch_click_handler(GtkWidget *draw_area, list *lptr, double x, double y) {
	void (*handlers[])(GtkWidget*, list*, double, double) = {ch_add_point, ch_add_line_pp,
		ch_add_line_la, ch_add_rect_pp, ch_add_rect_wh, ch_add_circle, ch_add_arc,
		ch_add_circle_prm, ch_add_arc_prm, ch_click_cursor_select, ch_move, ch_cp,
		ch_rotate, ch_text};
	int len = 0;

	len = arr_len(handlers, ch_handler);
	if (is_spec_mode()) {
		(*last_node_cb)(last_selected_node, x, y);

		if (!spec_hold_clicks)
			draw_mode = FG_TYPE_NONE;
		spec_hold_clicks--;
	}
	else if (draw_mode < len)
		(*handlers[draw_mode])(draw_area, lptr, x, y);	
}

void ch_add_point(GtkWidget *draw_area, list *lptr, double x, double y) {
	list *last;
	figure *point;

	list_add_node(lptr);

	last = list_get_last(lptr);
	point = figure_new_point(x, y);

	list_set_data(last, point);

	gtk_widget_queue_draw(draw_area);
}

void ch_add_line_pp(GtkWidget *draw_area, list *lptr, double x, double y) {
	list *last;
	figure *line;

	if (!state) {
		figure_fill(&tmp_figure, x, y, 0, 0, FG_TYPE_LINE_PP);
		tmp_figure.visible = VM_PREVIEW;

		dl_send_preview_figure(&tmp_figure);
		hl_set_help(HC_END_POINT);
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

		hl_set_help(HC_START_POINT);
	}
	
	state = !state;
	dl_switch_show_preview();
	gtk_widget_queue_draw(draw_area);
}

void ch_add_line_la(GtkWidget *draw_area, list *lptr, double x, double y) {
	figure *line, *ext_line;
	list *last;

	ext_line = (figure*)pl_read("msg:ext_figure");

	list_add_node(lptr);

	last = list_get_last(lptr);
	line = figure_new_line_pp(x, y, 0, 0);
	line->visible = VM_SHOW;

	gel_calculate_line_la(line, ext_line->a1, ext_line->a2);

	list_set_data(last, line);

	gtk_widget_queue_draw(draw_area);
}

void ch_add_rect_pp(GtkWidget *draw_area, list *lptr, double x, double y) {
	list *last;
	figure *rect;

	if (!state) {
		figure_fill(&tmp_figure, x, y, 0, 0, FG_TYPE_RECT_PP);
		tmp_figure.visible = VM_PREVIEW;

		dl_send_preview_figure(&tmp_figure);	
		hl_set_help(HC_END_POINT);
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

		hl_set_help(HC_START_POINT);
	}

	state = !state;
	dl_switch_show_preview();
	gtk_widget_queue_draw(draw_area);
}

void ch_add_rect_wh(GtkWidget *draw_area, list *lptr, double x, double y) {
	figure *rect, *ext_rect;
	list *last;

	list_add_node(lptr);

	last = list_get_last(lptr);
	rect = figure_new_rect_pp(x, y, 0, 0);
	rect->visible = VM_SHOW;

	ext_rect = (figure*)pl_read("msg:ext_figure");

	rect->a1 = rect->x + ext_rect->a1;
	rect->a2 = rect->y + ext_rect->a2;

	list_set_data(last, rect);

	gtk_widget_queue_draw(draw_area);
}

void ch_add_circle(GtkWidget *draw_area, list *lptr, double x, double y) {
	figure *circle, *rad;
	list *last;
	double radii;

	if (!state) {
		figure_fill(&tmp_figure, x, y, 50, 0, FG_TYPE_CIRCLE);
		tmp_figure.visible = VM_PREVIEW;

		dl_send_preview_figure(&tmp_figure);
		hl_set_help(HC_RADIUS_POINT);
	}
	else {
		list_add_node(lptr);

		last = list_get_last(lptr);
		circle = figure_new_circle(0, 0, 0);

		// get readius
		rad = figure_new_line_pp(tmp_figure.x, tmp_figure.y, x, y);
		radii = gel_calculate_lenght(rad);

		figure_copy(circle, &tmp_figure);

		//set radii
		circle->a1 = radii;
		circle->visible = VM_SHOW;

		list_set_data(last, circle);

		hl_set_help(HC_CENTER_POINT);
	}

	state = !state;
	dl_switch_show_preview();
	gtk_widget_queue_draw(draw_area);
}

void ch_add_arc(GtkWidget *draw_area, list *lptr, double x, double y) {
	static double x1, y1, x2, y2, xh, yh, ang1, ang2;
	static int state = 0;
	figure l, cpoint, *arc, *arc_c, b_side_line, c_side_line;
	list *last;
	double R;

	if (!state) {
		x1 = x;
		y1 = y;

		state = 1;
	}
	else if (state == 1) {
		x2 = x;
		y2 = y;

		figure_fill(&l, x1, y1, x2, y2, FG_TYPE_LINE_PP);

		state = 2;
	}
	else {
		xh = x;
		yh = y;
		printf("Arc: xh = %g, yh = %g\n", xh, yh);

		// calculate the a, b and c side of triangle
		figure_fill(&b_side_line, x1, y1, xh, yh, FG_TYPE_LINE_PP);

		figure_fill(&c_side_line, xh, yh, x2, y2, FG_TYPE_LINE_PP);

		// get center point
		cpoint = *gel_get_arc_center(&b_side_line, &c_side_line);

		// calculate R
		figure_fill(&l, cpoint.x, cpoint.y, xh, yh, FG_TYPE_LINE_PP);
		R = gel_calculate_lenght(&l);

		// get angel of first point
		figure_fill(&l, cpoint.x, cpoint.y, x1, y1, FG_TYPE_LINE_PP);
		ang1 = gel_calculate_line_angle(&l);

		// get angel of second point
		figure_fill(&l, cpoint.x, cpoint.y, x2, y2, FG_TYPE_LINE_PP);
		ang2 = gel_calculate_line_angle(&l);

		printf("Arc: C(%g; %g), a1 = %g, a2 = %g, R = %g\n", cpoint.x, cpoint.y, gel_min(ang1, ang2), gel_max(ang1, ang2), R);

		// make arc
		list_add_node(lptr);

		last = list_get_last(lptr);
		arc = figure_new_arc(cpoint.x, cpoint.y, R, gel_min(ang1, ang2), gel_max(ang1, ang2));
		arc->visible = VM_SHOW;

		list_set_data(last, arc);

		// make center point
		list_add_node(lptr);

		last = list_get_last(lptr);
		arc_c = figure_new_point(cpoint.x, cpoint.y);

		list_set_data(last, arc_c);
		
		gtk_widget_queue_draw(draw_area);
		
		state = 0;
	}
}

void ch_click_cursor_select(GtkWidget *draw_area, list *lptr, double x, double y) {
	list *node, *res = NULL;
	multi_obj *mo;

	node = lptr;
	while (node && !res) {
		if (node->dt == OT_TEXT) {
			res = ch_click_cursor_select_text(node, x, y);

			if (res)
				break;
		}
		else {
			res = ch_click_cursor_select_figure(node, x, y);

			if (res)
				break;
		}
		node = node->next;
	}

	if (res) {
		mo = mol_extract(res);
		mo->visible = VM_SELECTED;
		mol_apply(res, mo);

		last_selected_node = res;
	}
	else
		ch_click_cursor_unselect_all(lptr);

	gtk_widget_queue_draw(draw_area);
}

list *ch_click_cursor_select_figure(list *lptr, double x, double y) {
	figure *fptr, curs;

	figure_fill(&curs, x, y, 0, 0, FG_TYPE_POINT);
	fptr = figure_get_from_node(lptr);

	if (!fptr)
		return NULL;

	switch (fptr->type) {
		case FG_TYPE_POINT:
			if (gel_is_point_in_point(fptr, &curs))
				return lptr;
		break;

		case FG_TYPE_LINE_PP:
			if (gel_is_point_in_line(fptr, &curs))
				return lptr;
		break;

		case FG_TYPE_RECT_PP:
			if (gel_is_point_in_rect(fptr, &curs))
				return lptr;
		break;

		case FG_TYPE_CIRCLE:
			if (gel_is_point_in_circle(fptr, &curs))
				return lptr;
		break;
	}

	return NULL;
}

list *ch_click_cursor_select_text(list *lptr, double x, double y) {
	figure text_area, p;
	text *tptr;

	tptr = (text*)lptr->data;
	figure_fill(&text_area, tptr->x, tptr->y, 0, 0, FG_TYPE_RECT_PP);
	figure_fill(&p, x, y, 0, 0, FG_TYPE_POINT);

	if (gel_is_point_in_point(&text_area, &p) && tptr->visible != VM_NOT_FINISHED) {
		//tptr->visible = VM_SELECTED;
		//last_selected_node = lptr;

		return lptr;
	}
	return NULL;
}

void ch_click_cursor_unselect_all(list *lptr) {
	list_crawl(lptr, unselect);
}

void ch_unselect_last_node() {
	figure *fptr = NULL;
	text *tptr = NULL;

	if (!last_selected_node)
		return ;

	if (last_selected_node->dt == OT_TEXT) {
		tptr = (text*)last_selected_node->data;
		tptr->visible = VM_SHOW;
	}
	else {
		fptr = (figure*)last_selected_node->data;
		fptr->visible = VM_SHOW;
	}
}

double offset_x = 0, offset_y = 0;

void ch_move(GtkWidget *draw_area, list *lptr, double x, double y) {
	static double base_x = 0, base_y = 0;

	if (!state) {
		base_x = x;
		base_y = y;	

		hl_set_help(HC_INS_POINT);
	}
	else {
		offset_x = x - base_x;	
		offset_y = y - base_y;

		list_crawl(lptr, ch_fugure_move);

		hl_set_help(HC_BASE_POINT);
	}

	state = !state;
	gtk_widget_queue_draw(draw_area);
}

void ch_fugure_move(list *lptr) {
	figure *fptr;

	if (lptr->dt == OT_TEXT) {
		ch_text_move(lptr);

		return ;
	}

	fptr = figure_get_from_node(lptr);
	if (!fptr)
		return;

	if (fptr->visible == VM_SELECTED) {
		fptr->x += offset_x;
		fptr->y += offset_y;		
		fptr->a1 += offset_x;
		fptr->a2 += offset_y;
	}
}

void ch_text_move(list *lptr) {
	text *tptr;

	tptr = (text*)lptr->data;
	if (!tptr)
		return ;

	if (tptr->visible == VM_SELECTED) {
		tptr->x += offset_x;
		tptr->y += offset_y;
	}
}

void ch_cp(GtkWidget *draw_area, list *lptr, double x, double y) {
	static double base_x = 0, base_y = 0;

	if (!state) {
		base_x = x;
		base_y = y;

		hl_set_help(HC_INS_POINT);
	}
	else {
		offset_x = x - base_x;
		offset_y = y - base_y;

		list_crawl(lptr, ch_copy_offset);

		hl_set_help(HC_BASE_POINT);
	}

	state = !state;
	gtk_widget_queue_draw(draw_area);
}

void ch_copy_offset(list *lptr) {
	figure *fptr, *new_figure;
	list *last;

	if (lptr && lptr->dt == OT_TEXT) {
		ch_copy_text(lptr);

		return ;
	}

	fptr = figure_get_from_node(lptr);
	if (!fptr)
		return ;

	if (fptr->visible == VM_SELECTED) {
		fptr->visible = VM_SHOW;	

		list_add_node(lptr);

		last = list_get_last(lptr);
		new_figure = figure_new(fptr->type, fptr->x + offset_x, fptr->y + offset_y, fptr->a1 + offset_x, fptr->a2 + offset_y, 0);
		new_figure->visible = VM_SHOW;

		list_set_data(last, new_figure);
	}
}

void ch_copy_text(list *lptr) {
	text *tptr, *new_text;
	list *last;

	tptr = (text*)lptr->data;
	if (!tptr)
		return ;

	if (tptr->visible == VM_SELECTED) {
		tptr->visible = VM_SHOW;

		list_add_node(lptr);

		last = list_get_last(lptr);
		new_text = tl_new(tptr->font, tptr->size, tptr->color_r, tptr->color_g, tptr->color_b);
		new_text->visible = VM_SHOW;
		new_text->x = tptr->x + offset_x;
		new_text->y = tptr->y + offset_y;
		tl_add_buffer(new_text, tptr->buffer);

		last->dt = OT_TEXT;
		list_set_data(last, new_text);
	}
}

double angle = 0;
figure base;

void ch_rotate(GtkWidget *draw_area, list *lptr, double x, double y) {
	figure *ext_ang = NULL;

	ext_ang = (figure*)pl_read("msg:ext_figure");
	angle = gel_convert_grades_to_rads(ext_ang->x);

	base.x = x;
	base.y = y;

	list_crawl(lptr, ch_rot);

	gtk_widget_queue_draw(draw_area);
}

void ch_rot(list *lptr) {
	figure *fptr = NULL;

	fptr = (figure*)lptr->data;
	if (!fptr)
		return ;

	if (fptr->visible == VM_SELECTED) {
		figure_rotate(lptr, &base, angle);	
	}
}

void ch_add_circle_prm(GtkWidget *draw_area, list *lptr, double x, double y) {
	figure *circle, *ext_circle;
	list *last;

	list_add_node(lptr);

	ext_circle = (figure*)pl_read("msg:ext_figure");
	circle = figure_new_circle(x, y, ext_circle->a1);
	last = list_get_last(lptr);

	circle->visible = VM_SHOW;

	list_set_data(last, circle);

	gtk_widget_queue_draw(draw_area);
}

void ch_add_arc_prm(GtkWidget *draw_area, list *lptr, double x, double y) {
	figure *arc, *ext_arc;
	list *last;

	list_add_node(lptr);

	ext_arc = (figure*)pl_read("msg:ext_figure");
	arc = figure_new_arc(x, y, ext_arc->a1, ext_arc->a2, ext_arc->a3);
	last = list_get_last(lptr);

	arc->visible = VM_SHOW;

	list_set_data(last, arc);

	gtk_widget_queue_draw(draw_area);
}

int tx, ty;

void ch_text(GtkWidget *draw_area, list *lptr, double x, double y) {
	tx = x;
	ty = y;

	list_crawl(lptr, ch_proc_text);

	hl_set_help(HC_SELECT_TOOL);

	gtk_widget_queue_draw(draw_area);
}

void ch_proc_text(list *lptr) {
	text *tptr;

	if (lptr->dt == OT_TEXT && lptr->data) {
		tptr = (text*)lptr->data;

		if (tptr->visible == VM_NOT_FINISHED) {
			tptr->x = tx;
			tptr->y = ty;

			tptr->visible = VM_SHOW;
		}
	}
}

void ch_null_op(list *lptr, double x, double y) {

}

void ch_set_last_node_cb(void (*fun)(list *, double, double)) {
	last_node_cb = fun;
}
