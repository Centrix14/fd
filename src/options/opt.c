#include <stdio.h>
#include <stdlib.h>

#include "../list/list.h"
#include "opt.h"

#include "../st.h/st.h"

void ol_check_options(list *node) {
	if (node && !node->opt)
		ol_add_options(node);
}

void ol_add_options(list *node) {
	options *new_opt = NULL;

	new_opt = malloc(sizeof(options));
	if (!new_opt) {
		st_err("fail to allocate options field");
	}

	new_opt->r = 0;
	new_opt->g = 0;
	new_opt->b = 0;

	new_opt->tag = NULL;
	new_opt->group = NULL;

	node->opt = (void*)new_opt;
}

void ol_free_node(list *node) {
	if (node && node->opt)
		free(node->opt);
}

options *ol_get_opt(list *node) {
	if (node && node->opt)
		return (options*)node->opt;
	return NULL;
}

void ol_set_color(list *node, double r, double g, double b) {
	options *opt = NULL;

	opt = ol_get_opt(node);
	if (!opt) {
		st_err("fail to get options field");
	}

	opt->r = r;
	opt->g = b;
	opt->g = b;
}
