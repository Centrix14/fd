/**
 * \file
 * \brief This file contains a library of doubly linked lists used by FD to store geometry.
 *
 * The file contains a description of all the minimal functionality for working with
 * doubly linked lists. The functions, as far as the developer knows, are safe and stable,
 * they can be used to store a variety of data.
 */

#include <stdio.h>
#include <stdlib.h>

#include "../fd_core.h"

st_debug_start(0);

/**
 * \brief Initializes a new list item.
 * \param[parent] parent of new element
 */

list *list_init_node(list *parent) {
	list *nptr = malloc(sizeof(list));

	if (!nptr) {
		el_call_error(ET_FAIL_TO_CREATE_LIST_NODE);

		return NULL;
	}

	nptr->prev = parent;
	nptr->next = NULL;
	nptr->data = NULL;
	nptr->dt = OT_FIGURE;
	nptr->opt = NULL;

	return nptr;
}

/**
 * \brief Frees up the memory reserved for the list item.
 * \param[node] target element
 */

void list_free_node(list *node) {
	if (node) {
		st_debug {
			printf("*** free: %p\n", node);
		}

		free(node);
	}
}

/**
 * \brief Create new node and add it to the end of list.
 * \param[parent] target list
 */

void list_add_node(list *parent) {
	list *nptr = parent;

	while (nptr->next)
		nptr = nptr->next;

	nptr->next = list_init_node(parent);
	nptr->next->prev = nptr;
}

/**
 * \brief Frees up the memory reserved for the list.
 * \param[node] Begining of list
 */

void list_free_list(list *node) {
	if (node->next)
		list_free_list(node->next);
	list_free_node(node);
}

/**
 * \brief Changes data field of given node.
 * \param[node] target node.
 * \param[new_data] target data
 */

void list_set_data(list *node, void *new_data) {
	node->data = new_data;
}

/**
 * \brief Prints list
 * \param[root] begining of the target list
 */

void list_show(list *root) {
	list *nptr = root;

	while (nptr) {
		printf("addr = %p data = %s\n", nptr, (char*)nptr->data);

		nptr = nptr->next;
	}
}

/**
 * \brief Returns last element of the list
 * \param[root] begining of the target list
 */

list *list_get_last(list *root) {
	list *nptr = root;

	while (nptr->next)
		nptr = nptr->next;
	return nptr;
}

/**
 * \brief Standart list iterator
 * \param[root] target list
 * \param[func] A function called for each list item.
 */

void list_crawl(list *root, void (*func)(list*)) {
	if (root->next)
		list_crawl(root->next, func);
	(*func)(root);
}

/**
 * \brief Returns the value of the data field of the list item.
 * \param[lptr] target list
 */

void *list_get_data(list *lptr) {
	if (lptr && lptr->data)
		return lptr->data;
	return NULL;
}
