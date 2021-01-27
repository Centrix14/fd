#ifndef __FLAT_DRAW_MULTI_OBJECT_PROCESSING_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_MULTI_OBJECT_PROCESSING_LIBRARY_HEADER_FILE_INCLUDED__

typedef struct {
	double x;
	double y;

	int visible;
} multi_obj;

void mol_free_from_node(list *lptr);
void mol_draw_obj_from_node(list *lptr);
figure *mol_conv_to_figure(list *lptr);

multi_obj *mol_extract(list *lptr);
multi_obj *mol_extract_from_text(list *lptr);
multi_obj *mol_extract_from_figure(list *lptr);

void mol_apply(list *lptr, multi_obj *mo);
void mol_apply_text(list *lptr, multi_obj *mo);
void mol_apply_figure(list *lptr, multi_obj *mo);

#endif
