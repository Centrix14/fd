#include <gtk/gtk.h>

#include "../list/list.h"
#include "../figure/figure.h"
#include "../text/text.h"
#include "../draw/draw.h"
#include "multi_obj.h"

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
}

void mol_draw_obj_from_node(list *lptr) {
	text *tptr;

	if (!lptr || !lptr->data)
		return ;

	switch(lptr->dt) {
		case OT_FIGURE:
			dl_draw_figure((figure*)lptr->data);
		break;

		case OT_TEXT:
			tptr = (text*)lptr->data;

			if ((tptr->visible != VM_NOT_FINISHED) && (tptr->lay == figure_get_current_lay()))
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
}

void mol_apply_figure(list *lptr, multi_obj *mo) {
	figure *fptr;

	fptr = (figure*)lptr->data;
	fptr->x = mo->x;
	fptr->y = mo->y;
	fptr->visible = mo->visible;
	fptr->lay = mo->lay;
}
