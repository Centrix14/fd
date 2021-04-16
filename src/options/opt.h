#ifndef __FLAT_DRAW_CAD_MULTI_OBJECT_OPTIONS_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CAD_MULTI_OBJECT_OPTIONS_LIBRARY_HEADER_FILE_INCLUDED__

#include "../list/list.h"

typedef struct {
	int r;
	int g;
	int b;

	char *group;
	char *tag;
} options;

void ol_check_options(list *node);
void ol_add_options(list *node);
void ol_free_node(list *node);

#endif
