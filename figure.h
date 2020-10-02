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
	FG_TYPE_RECT_PP
};

enum VISIBLE_MODES {
	VM_HIDE = 0,
	VM_SHOW,
	VM_PREVIEW
};

figure *figure_new(int type, int x, int y, int a1, int a2);
figure *figure_new_point(int x, int y);
figure *figure_new_line_pp(int x1, int y1, int x2, int y2);
figure *figure_new_rect_pp(int x, int y, int w, int h);

void figure_free(figure *fptr);
void figure_free_list(list *lptr);

figure *figure_rect_decompose(figure *rect);

void figure_fill(figure *fptr, double x, double y, double a1, double a2, double type);

void figure_set_current_lay(int new_lay);
int figure_get_current_lay();

#endif
