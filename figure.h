#ifndef __FLAT_DRAW_FIGURE_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_FIGURE_LIBRARY_HEADER_FILE_INCLUDED__

#include "data/list.h"

typedef struct {
	double x;
	double y;

	double a1;
	double a2;

	int type;

	int visible;

	int lay;
} figure;

enum FG_TYPES {
	FG_TYPE_POINT = 0,
	FG_TYPE_LINE_PP,
	FG_TYPE_LINE_LA,
	FG_TYPE_RECT_PP,
	FG_TYPE_RECT_WH,
	FG_TYPE_CIRCLE,
	FG_TYPE_ARC
};

enum VISIBLE_MODES {
	VM_HIDE = 0,
	VM_SHOW,
	VM_PREVIEW,
	VM_PROJECTION
};

figure *figure_new(int type, int x, int y, int a1, int a2);
figure *figure_new_point(int x, int y);
figure *figure_new_line_pp(int x1, int y1, int x2, int y2);
figure *figure_new_rect_pp(int x, int y, int w, int h);
figure *figure_new_circle(int x, int y, int r);
figure *figure_new_arc(double xc, double yc, double r, double angle);

void figure_free(figure *fptr);
void figure_free_list(list *lptr);

figure *figure_rect_decompose(figure *rect);

void figure_fill(figure *fptr, double x, double y, double a1, double a2, double type);

void figure_set_current_lay(int new_lay);
int figure_get_current_lay();

void figure_copy(figure *dst, figure *src);

int figure_is_line(figure *fptr);
int figure_is_projection_lay_list(list *lptr, int lay);

void figure_set_visible_by_lay_list(list *lptr, int lay, int vm_mode);

#endif
