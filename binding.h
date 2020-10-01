#ifndef __FLAT_DRAW_BINDIG_LIBRARY_HEDRE_FILE_INCLUDED__
#define __FLAT_DRAW_BINDIG_LIBRARY_HEDRE_FILE_INCLUDED__

#include "data/list.h"
#include "figure.h"

#define BINDING_AREA 20

void bl_get_bind_from_coords(list *lptr, int *x, int *y);
int bl_is_create_binding(figure *target, int x, int y);
int bl_get_coords_dif(int c1, int c2);

int bl_get_binding_possibility_point(figure *point, int x, int y);
int bl_get_binding_possibility_line(figure *line, int x, int y);
int bl_get_binding_possibility_rect(figure *rect, int x, int y);

void bl_make_binding(figure *fptr, int *x, int *y);
void bl_make_binding_point(figure *point, int *x, int *y);
void bl_make_binding_line(figure *line, int *x, int *y);
void bl_make_binding_rect(figure *rect, int *x, int *y);

#endif
