#ifndef __FLAT_DRAW_FIGURE_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_FIGURE_LIBRARY_HEADER_FILE_INCLUDED__

#include "data/list.h"

typedef struct {
	double x;
	double y;

	double a1;
	double a2;
	double a3;

	int type;

	int visible;

	int lay;

	char id[16];
} figure;

enum FG_TYPES {
	FG_TYPE_POINT = 0,
	FG_TYPE_LINE_PP,
	FG_TYPE_LINE_LA,
	FG_TYPE_RECT_PP,
	FG_TYPE_RECT_WH,
	FG_TYPE_CIRCLE,
	FG_TYPE_ARC,
	FG_TYPE_NONE
};

enum VISIBLE_MODES {
	VM_HIDE = 0,
	VM_SHOW,
	VM_PREVIEW,
	VM_PROJECTION,
	VM_SELECTED
};

figure *figure_new(double type, double x, double y, double a1, double a2, double a3);
figure *figure_new_point(double x, double y);
figure *figure_new_line_pp(double x1, double y1, double x2, double y2);
figure *figure_new_rect_pp(double x, double y, double w, double h);
figure *figure_new_circle(double x, double y, double r);
figure *figure_new_arc(double xc, double yc, double r, double angle1, double angle2);

void figure_free(figure *fptr);
void figure_free_list(list *lptr);

figure *figure_rect_decompose(figure *rect);

void figure_fill(figure *fptr, double x, double y, double a1, double a2, double type);

int figure_get_current_lay();
char *figure_get_type(int type);
void figure_set_visible_by_lay_list(list *lptr, int lay, int vm_mode);
void figure_set_current_lay(int new_lay);

void figure_copy(figure *dst, figure *src);

int figure_is_line(figure *fptr);
int figure_is_projection_lay_list(list *lptr, int lay);

#endif
