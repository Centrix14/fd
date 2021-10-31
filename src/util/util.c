#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <gtk/gtk.h>

#include "util.h"
#include "../list/list.h"
#include "../figure/figure.h"
#include "../multi_obj/multi_obj.h"
#include "../text/text.h"
#include "../options/opt.h"

// util function for mapping values
double ul_map(double x, double in_min, double in_max, double out_min, double out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// util function for extracting rgb from string
void ul_get_colors(int *arr, GtkWidget *color_entry) {
	char col_str[256] = "", *col_token = NULL;
	int i = 0;

	strcpy(col_str, (char*)gtk_entry_get_text(GTK_ENTRY(color_entry)));
	col_token = strtok(col_str, " ");
	while (col_token) {
		arr[i++] = atoi(col_token);

		col_token = strtok(NULL, " ");
	}

	for (; i < 3; i++)
		arr[i] = 0;
}

// util function for font names parsing
void ul_pars_font(char *str, char *font_name, char *font_size) {
	char *tok = NULL;

	tok = strtok(str, " ");
	while (tok) {
		if (isdigit((int)*tok))
			strcat(font_size, tok);
		else {
			strcat(font_name, tok);
			strcat(font_name, " ");
		}

		tok = strtok(NULL, " ");
	}
}

char *ul_get_home_path() {
	return getenv("HOME");
}

char *ul_get_file_from_home(char *path) {
	char *home = NULL, *delim = NULL;

	delim = ul_get_path_delim();
	home = ul_get_home_path();
	if (home) {
		strcat(home, delim);
		strcat(home, path);

		return home;
	}

	return NULL;
}

char *ul_get_system_name() {
	return SYSTEM;
}

char *ul_get_path_delim() {
	char *sys = NULL;

	sys = ul_get_system_name();
	if (!strcmp(sys, UL_WINDOWS))
		return "\\";
	return "/";
}

int ul_is_exist(char *filename) {
	FILE *fptr = fopen(filename, "r");
	int out = 0;

	if (fptr)
		out = 1;
	fclose(fptr);

	return out;
}

char *ul_remove_char(char *str, int c) {
	static char buf[256] = "";
	int j = 0;

	for (int i = 0; str[i]; i++)
		if (str[i] != c)
			buf[j++] = str[i];
	buf[j] = 0;
	return buf;
}

void ul_pars_coords(char *str, int count, int *arr) {
	char *token = NULL;

	token = strtok(str, " ");
	for (int i = 0; i < count && token; i++) {
		arr[i] = atof(token);

		token = strtok(NULL, " ");
	}
}

list *ul_get_selected_node(list *buf) {
	list *lptr = NULL;
	multi_obj *mo = NULL;

	lptr = buf;
	while (lptr) {
		mo = mol_extract(lptr);
		if (mo && mo->visible == VM_SELECTED)
			return lptr;

		lptr = lptr->next;
	}

	return NULL;
}

int ul_pars_layer_str(char *str) {
	char *tok = NULL;

	tok = strtok(str, " ");
	tok = strtok(NULL, " ");

	return atoi(tok);
}

size_t __ul_get_data_size(list *node) {
	switch (node->dt) {
		case OT_FIGURE:
			return sizeof(figure);
		break;

		case OT_TEXT:
			return sizeof(text);
		break;
	}

	return 0;
}

void __ul_copy_to_dest(list *dest, list *src) {
	list *last = NULL;
	size_t size = 0;

	// add node
	list_add_node(dest);
	last = list_get_last(dest);
	
	// copy
	if (src->opt) {
		last->opt = (options*)malloc(sizeof(options));
		memcpy(last->opt, src->opt, sizeof(options));
	}
	if (src->data) {
		size = __ul_get_data_size(src);
		last->data = malloc(size);
		memcpy(last->data, src->data, size);

		last->dt = src->dt;
	}
}

list *ul_get_selected_node_multiple(list *src, int (*check)(list*)) {
	list *lptr = NULL, *dest = NULL;
	int res = 0;

	// init list of results
	dest = list_init_node(NULL);

	// main loop
	lptr = src;
	while (lptr) {
		res = (*check)(lptr);
		if (res)
			__ul_copy_to_dest(dest, lptr);

		lptr = lptr->next;
	}

	return dest;
}

char *ul_get_file_ext(char *name) {
	static char ext[256] = "";
	int i = 0, j = 0;

	i = strlen(name) - 1;
	for (; i; i--) {
		if (name[i] == '.')
			break;
		ext[j++] = name[i];
	}

	if (!i)
		return NULL;
	return ext;
}

void ul_str_reverse(char *str) {
	int len = 0;

	len = strlen(str);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len - 1 - i; j++) {
			str[j] ^= str[j+1];
			str[j+1] ^= str[j];
			str[j] ^= str[j+1];
		}
	}
}

char *ul_get_full_path(char *path, char *file) {
	static char result[256] = "";
	int len = 0;

	len = strlen(path);
	if (path[len-1] == '/')
		sprintf(result, "%s%s", path, file);
	else
		sprintf(result, "%s/%s", path, file);
	return result;
}

int ul_get_system_code() {
	return SYSTEM_CODE;
}
