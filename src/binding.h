#ifndef __FLAT_DRAW_BINDIG_LIBRARY_HEDRE_FILE_INCLUDED__
#define __FLAT_DRAW_BINDIG_LIBRARY_HEDRE_FILE_INCLUDED__

#include "data/list.h"
#include "figure.h"

#define BINDING_AREA 20

void bl_bind(list *lptr, double *x, double *y);

char *bl_try_make_object_bind(list *lptr, double *x, double *y);
char *bl_try_make_intersection_binding(list *lptr, double *x, double *y);
char *bl_try_make_vertical_binding(list *lptr, double *x, double *y);

int bl_is_create_binding(figure *target, double x, double y);
double bl_get_coords_dif(double c1, double c2);

int bl_get_binding_possibility_point(figure *point, double x, double y);
int bl_get_binding_possibility_line(figure *line, double x, double y);
int bl_get_binding_possibility_rect(figure *rect, double x, double y);
int bl_get_binding_possibility_circle(figure *circle, double x, double y);

void bl_make_binding(figure *fptr, double *x, double *y);
void bl_make_binding_point(figure *point, double *x, double *y);
void bl_make_binding_line(figure *line, double *x, double *y);
void bl_make_binding_rect(figure *rect, double *x, double *y);
void bl_make_binding_circle(figure *circle, double *x, double *y);

char *bl_make_vertical_bind_if_possible(list *node, double *x, double *y);

#endif
