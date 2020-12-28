#include "data/list.h"
#include "figure.h"
#include "text.h"

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
