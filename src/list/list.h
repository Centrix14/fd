/**
 * \file
 * \brief The header file of the library of doubly linked lists.
 */

#ifndef __FLAT_DRAW_DINAMIC_LIST_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_DINAMIC_LIST_LIBRARY_HEADER_FILE_INCLUDED__

/// Enumeration of object types.

typedef enum {
	OT_FIGURE = 0, ///< figure object
	OT_TEXT ///< text object
} OBJ_TYPE;

/// The structure of the list item.

typedef struct __list__ {
	struct __list__ *prev; ///< pointer to previous node
	struct __list__ *next; ///< pointer to next node

	void *data; ///< node data
	void *opt; ///< node options

	OBJ_TYPE dt; ///< data type
} list;

list *list_init_node(list *parent);
void list_add_node(list *parent);
void list_set_data(list *node, void *new_data);
void list_show(list *root);
void list_crawl(list *root, void (*func)(list*));

void list_free_node(list *node);
void list_free_list(list *node);

list *list_get_last(list *root);
void *list_get_data(list *lptr);

#endif
