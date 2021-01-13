#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../list/list.h"
#include "../figure/figure.h"
#include "../fd_format/fd_format.h"

static char target_file[256] = "\0";

void fdl_target_file(char *name) {
	strcpy(target_file, name);
}

void fdl_write_figure_stream(FILE *stream, figure *fptr) {
	fprintf(stream, "%d %d %d ", fptr->type, fptr->visible, fptr->lay);
	fprintf(stream, "%f %f %f %f\n", fptr->x, fptr->y, fptr->a1, fptr->a2);
}

void fdl_write_from_list(list *lptr) {
	FILE *file = fopen(target_file, "w");
	list *node = lptr;
	figure *fig;

	if (!file) {
		perror(__func__);
		return ;
	}

	while (node) {
		fig = (figure*)node->data;
		if (fig)
			fdl_write_figure_stream(file, fig);

		node = node->next;
	}

	fclose(file);
}

void fdl_read_string(figure *fptr, char *str) {
	char *tok = NULL;

	tok = strtok(str, FD_FORMAT_DELIM);
	if (tok)
		fptr->type = atoi(tok);

	tok = strtok(NULL, FD_FORMAT_DELIM);
	if (tok)
		fptr->visible = atoi(tok);

	tok = strtok(NULL, FD_FORMAT_DELIM);
	if (tok)
		fptr->lay = atoi(tok);

	tok = strtok(NULL, FD_FORMAT_DELIM);
	if (tok)
		fptr->x = atof(tok);

	tok = strtok(NULL, FD_FORMAT_DELIM);
	if (tok)
		fptr->y = atof(tok);

	tok = strtok(NULL, FD_FORMAT_DELIM);
	if (tok)
		fptr->a1 = atof(tok);

	tok = strtok(NULL, FD_FORMAT_DELIM);
	if (tok)
		fptr->a2 = atof(tok);
}

void fdl_read_file(list *lptr) {
	FILE *file = fopen(target_file, "r");
	char str[256] = "\0";
	figure *fptr;
	list *last;

	if (!file) {
		perror(__func__);
		return ;
	}

	fgets(str, 256, file);
	while (!feof(file)) {
		list_add_node(lptr);
		
		last = list_get_last(lptr);
		fptr = figure_new_point(0, 0);

		fdl_read_string(fptr, str);

		list_set_data(last, fptr);

		fgets(str, 256, file);
	}

	fclose(file);
}
