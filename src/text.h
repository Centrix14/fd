#ifndef __FLAT_DRAW_TEXT_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_TEXT_LIBRARY_HEADER_FILE_INCLUDED__

typedef struct {
	char *buffer;

	char *font;

	int size;

	int color_r;
	int color_g;
	int color_b;

	int visible;

	double x;
	double y;
} text;

text *tl_new(char *font, int size, int r, int g, int b);

void tl_free(text *tptr);
void tl_free_buffer(text *tptr);

void tl_add_buffer(text *tptr, char *buf);

#endif
