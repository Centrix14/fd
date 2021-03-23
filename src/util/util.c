#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <gtk/gtk.h>

#include "../list/list.h"
#include "util.h"

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
