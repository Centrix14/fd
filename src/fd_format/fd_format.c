#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../list/list.h"
#include "../figure/figure.h"
#include "../fd_format/fd_format.h"
#include "../text/text.h"
#include "../util/util.h"

static char target_file[256] = "\0";

void fdl_target_file(char *name) {
	strcpy(target_file, name);
}

void fdl_write_object_stream(FILE *stream, list *lptr) {
	figure *fptr;
	text *tptr;

	fprintf(stream, "%d ", (int)lptr->dt);
	if (lptr->dt == OT_FIGURE) {
		fptr = (figure*)lptr->data;

		fprintf(stream, "%d %d %d ", fptr->type, fptr->visible, fptr->lay);
		fprintf(stream, "%f %f %f %f\n", fptr->x, fptr->y, fptr->a1, fptr->a2);
	}
	else if (lptr->dt == OT_TEXT) {
		tptr = (text*)lptr->data;

		fprintf(stream, "%d %d ", tptr->visible, tptr->size);
		fprintf(stream, "%f %f ", tptr->x, tptr->y);
		fprintf(stream, "%d %d %d\n", tptr->color_r, tptr->color_g, tptr->color_b);
		fprintf(stream, "%s\n", tptr->font);
		fprintf(stream, "%s\n/end", tptr->buffer);
	}
}

void fdl_write_from_list(list *lptr) {
	FILE *file = fopen(target_file, "w");
	list *node = lptr;

	if (!file) {
		perror(__func__);
		return ;
	}

	while (node) {
		if (node->data)
			fdl_write_object_stream(file, node);

		node = node->next;
	}

	fclose(file);
}

/*void fdl_read_string(figure *fptr, char *str) {
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
}*/

void fdl_read_file(list *lptr) {
	FILE *file = fopen(target_file, "r");
	int op_code = 0;

	if (!file) {
		perror(__func__);
		return ;
	}

	fscanf(file, "%d", &op_code);
	while (!feof(file)) {
		if (op_code == (int)OT_FIGURE)
			fdl_pars_figure(file, lptr);
		else if (op_code == (int)OT_TEXT)
			fdl_pars_text(file, lptr);
	}

	fclose(file);
}

void fdl_pars_figure(FILE *stream, list *lptr) {
	figure *fptr;
	list *last;

	// just create figure
	fptr = figure_new_point(0, 0);

	// fill it from file
	fscanf(stream, "%d %d %d", &fptr->type, &fptr->visible, &fptr->lay);
	fscanf(stream, "%lf %lf %lf %lf", &fptr->x, &fptr->y, &fptr->a1, &fptr->a2);

	// create new node
	list_add_node(lptr);

	last = list_get_last(lptr);
	list_set_data(last, fptr);
}

void fdl_pars_text(FILE *stream, list *lptr) {
	char str[256] = "", buffer[2048] = "";
	text *tptr;
	list *last;

	tptr = tl_new("Arial", 20, 255, 255, 255);

	fscanf(stream, "%d %d ", &tptr->visible, &tptr->size);
	fscanf(stream, "%lf %lf ", &tptr->x, &tptr->y);
	fscanf(stream, "%d %d %d\n", &tptr->color_r, &tptr->color_g, &tptr->color_b);
	fscanf(stream, "%s\n", tptr->font);

	fgets(str, 256, stream);
	while (strcmp(str, FD_FORMAT_END)) {
		strcat(buffer, str);

		fgets(str, 256, stream);
	}

	tl_add_buffer(tptr, buffer);
}
