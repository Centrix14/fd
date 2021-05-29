#include <gtk/gtk.h>

#include "../list/list.h"
#include "../figure/figure.h"
#include "../text/text.h"
#include "../draw/draw.h"
#include "../options/opt.h"
#include "../color/color.h"
#include "multi_obj.h"

#include "../pechkin/pl.h"

void mol_free_from_node(list *lptr) {
	text *tptr;

	if (!lptr || !lptr->data)
		return ;

	switch (lptr->dt) {
		case OT_FIGURE:
			figure_free_list(lptr);
		break;

		case OT_TEXT:
			tptr = (text*)lptr->data;

			if (tptr->buffer)
				tl_free_buffer(tptr);
			tl_free(tptr);
		break;
	}

	if (lptr->opt)
		ol_free_node(lptr);
}

void mol_draw_obj_from_node(list *lptr) {
	text *tptr;
	figure *fptr;
	options *opt;
	cairo_t *cr;

	if (!lptr || !lptr->data)
		return ;

	switch(lptr->dt) {
		case OT_FIGURE:
			cr = *(cairo_t**)pl_read("msg:cairo-context");
			fptr = (figure*)lptr->data;
			opt = (options*)lptr->opt;

			if (fptr->visible == VM_SELECTED)
				cl_set_color(cr, CL_DEF_SELECTED_COLOR);
			else if (opt)
				cl_set_color_rgb(cr, opt->r, opt->g, opt->b);
			else if (fptr->pr_mode) {
				if (figure_get_current_lay() != fptr->lay)
					cl_set_color(cr, CL_DEF_PROJECTION_COLOR);
				else
					cl_set_color(cr, CL_DEF_DRAW_COLOR);
			}
			else
				cl_set_color_fg(cr, fptr->visible);
			dl_draw_figure(fptr);
		break;

		case OT_TEXT:
			tptr = (text*)lptr->data;

			if (tptr->visible != VM_NOT_FINISHED)
				dl_draw_text((text*)lptr->data);
		break;
	}
}

figure *mol_conv_to_figure(list *lptr) {
	figure *fptr;
	text *tptr;

	if (!lptr || !lptr->data)
		return NULL;

	if (lptr->dt == OT_FIGURE)
		return (figure*)(lptr->data);
	else {
		tptr = (text*)lptr->data;
		fptr = figure_new_point(tptr->x, tptr->y);
		fptr->visible = tptr->visible;
		fptr->lay = tptr->lay;

		return fptr;
	}
}

multi_obj *mol_extract(list *lptr) {
	if (!lptr || !lptr->data)
		return NULL;

	if (lptr->dt == OT_TEXT)
		return mol_extract_from_text(lptr);
	else
		return mol_extract_from_figure(lptr);
}

multi_obj *mol_extract_from_text(list *lptr) {
	static multi_obj mo;
	text *tptr;

	tptr = (text*)lptr->data;

	mo.x = tptr->x;
	mo.y = tptr->y;
	mo.visible = tptr->visible;
	mo.lay = tptr->lay;
	mo.pr_mode = tptr->pr_mode;

	return &mo;
}

multi_obj *mol_extract_from_figure(list *lptr) {
	static multi_obj mo;
	figure *fptr;

	fptr = (figure*)lptr->data;

	mo.x = fptr->x;
	mo.y = fptr->y;
	mo.visible = fptr->visible;
	mo.lay = fptr->lay;
	mo.pr_mode = fptr->pr_mode;

	return &mo;
}

void mol_apply(list *lptr, multi_obj *mo) {
	if (!lptr || !lptr->data)
		return ;

	if (lptr->dt == OT_TEXT)
		mol_apply_text(lptr, mo);
	else
		mol_apply_figure(lptr, mo);
}

void mol_apply_text(list *lptr, multi_obj *mo) {
	text *tptr;

	tptr = (text*)lptr->data;
	tptr->x = mo->x;
	tptr->y = mo->y;
	tptr->visible = mo->visible;
	tptr->lay = mo->lay;
	tptr->pr_mode = mo->pr_mode;
}

void mol_apply_figure(list *lptr, multi_obj *mo) {
	figure *fptr;

	fptr = (figure*)lptr->data;
	fptr->x = mo->x;
	fptr->y = mo->y;
	fptr->visible = mo->visible;
	fptr->lay = mo->lay;
	fptr->pr_mode = mo->pr_mode;
}

void mol_context_color_set(list *lptr) {
	figure *fptr;
	options *opt;
	cairo_t *cr;

	cr = *(cairo_t**)pl_read("msg:cairo-context");
	opt = ol_get_opt(lptr);
	fptr = (figure*)lptr->data;

	if (opt)
		cl_set_color_rgb(cr, opt->r, opt->g, opt->b);
	else
		cl_set_color_fg(cr, fptr->visible);
}

void mol_switch_pr_mode_for_lay(list *lptr, int lay) {
	list *node = NULL;
	multi_obj *mo;

	node = lptr;
	while (node) {
		if (!node->data) {
			node = node->next;

			continue;
		}
		mo = mol_extract(node);

		if (mo->lay == lay) {
			mo->pr_mode = !mo->pr_mode;
			mol_apply(node, mo);
		}

		node = node->next;
	}
}
