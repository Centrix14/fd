#include <stdio.h>
#include <stdlib.h>

#include "../fd_core.h"

void ol_check_options(list *node) {
	if (node && !node->opt)
		ol_add_options(node);
}

void ol_add_options(list *node) {
	options *new_opt = NULL;

	new_opt = malloc(sizeof(options));
	if (!new_opt) {
		el_call_error(ET_CREATE_OPTIONS_FAIL);

		return ;
	}

	new_opt->r = OPT_FIELD_UNSET;
	new_opt->g = OPT_FIELD_UNSET;
	new_opt->b = OPT_FIELD_UNSET;

	new_opt->tag = NULL;
	new_opt->group = NULL;

	node->opt = (void*)new_opt;
}

void ol_free_node(list *node) {
	options *opt;

	if (!node || !node->opt)
		return ;

	opt = node->opt;
	if (opt->tag)
		free(opt->tag);
	if (opt->group)
		free(opt->group);
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
		el_call_error(ET_CREATE_OPTIONS_FAIL);

		return ;
	}

	opt->r = r;
	opt->g = g;
	opt->b = b;
}

int ol_is_color_set(list *node) {
	options *opt = NULL;

	opt = ol_get_opt(node);
	if (!opt)
		return 0;

	if (opt->r == OPT_FIELD_UNSET || opt->g == OPT_FIELD_UNSET || opt->b == OPT_FIELD_UNSET)
		return 0;
	return 1;
}
