#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "../figure/figure.h"
#include "../dbg.h"

list *list_init_node(list *parent) {
	list *nptr = malloc(sizeof(list));

	if (!nptr) {
		perror(__func__);
		exit(0);
	}

	nptr = nptr;
	nptr->prev = parent;
	nptr->next = NULL;
	nptr->data = NULL;
	nptr->dt = OT_FIGURE;

	return nptr;
}

void list_free_node(list *node) {
	if (node) {
#ifdef DBG
		printf("*** free: %p\n", node);
#endif
		free(node);
	}
}

void list_add_node(list *parent) {
	list *nptr = parent;

	while (nptr->next)
		nptr = nptr->next;

	nptr->next = list_init_node(parent);
	nptr->next->prev = nptr;
}

void list_free_list(list *node) {
	if (node->next)
		list_free_list(node->next);
	list_free_node(node);
}

void list_set_data(list *node, void *new_data) {
	node->data = new_data;
}

void list_show(list *root) {
	list *nptr = root;

	while (nptr) {
		printf("addr = %p data = %s\n", nptr, (char*)nptr->data);

		nptr = nptr->next;
	}
}

list *list_get_last(list *root) {
	list *nptr = root;

	while (nptr->next)
		nptr = nptr->next;
	return nptr;
}

void list_crawl(list *root, void (*func)(list*)) {
	if (root->next)
		list_crawl(root->next, func);
	(*func)(root);
}

void *list_get_data(list *lptr) {
	if (lptr && lptr->data)
		return lptr->data;
	return NULL;
}

void list_dump_node(list *lptr) {
	FILE *file = fopen("dump", "w");
	figure *fptr = NULL;

	fptr = list_get_data(lptr);
	if (!fptr)
		return ;
	
	fprintf(stdout, "id = %s\tme = %p\tprev = %p\tnext = %p\tvm = %d (need 4)\n", fptr->id, lptr, lptr->prev, lptr->next, fptr->visible);

	fclose(file);
}
