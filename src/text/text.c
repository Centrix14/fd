#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../fd_core.h"

text *tl_new(char *font, int size, int r, int g, int b) {
	text *tptr = malloc(sizeof(text));

	if (!tptr) {
		el_call_error(ET_FAIL_TO_CREATE_TEXT_OBJECT);

		return NULL;
	}

	tptr->font = malloc(strlen(font)+1);
	if (!tptr->font) {
		el_call_error(ET_FAIL_TO_ALLOCATE_MEMORY_FOR_FONT);

		return NULL;
	}
	strcpy(tptr->font, font);

	tptr->lay = 0;
	tptr->pr_mode = 0;
	tptr->size = size;
	tptr->color_r = r;
	tptr->color_g = g;
	tptr->color_b = b;

	return tptr;
}

void tl_add_buffer(text *tptr, char *buf) {
	tptr->buffer = malloc(strlen(buf)+1);

	if (!tptr->buffer) {
		el_call_error(ET_FAIL_TO_ALLOCATE_MEMORY_FOR_TEXT_BUFFER);

		return ;
	}

	strcpy(tptr->buffer, buf);
}

void tl_free(text *tptr) {
	if (tptr) {
		free(tptr->font);
		free(tptr);
	}
}

void tl_free_buffer(text *tptr) {
	if (tptr->buffer)
		free(tptr->buffer);
}

char *tl_get_info(text *tptr) {
	static char buf[256] = "";

	sprintf(buf, "T(%g; %g)\nh = %d\n", tptr->x, tptr->y, tptr->size);

	return buf;
}
