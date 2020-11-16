#ifndef __FLAT_DRAW_DINAMIC_LIST_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_DINAMIC_LIST_LIBRARY_HEADER_FILE_INCLUDED__

typedef struct __list__ {
	struct __list__ *prev;
	struct __list__ *next;
	void *data;
} list;

list *list_init_node(list *parent);
void list_free_node(list *node);
void list_add_node(list *parent);
void list_free_list(list *node);
void list_set_data(list *node, void *new_data);
void list_show(list *root);
list *list_get_last(list *root);
void list_crawl(list *root, void (*func)(list*));
void *list_get_data(list *lptr);
void list_dump_node(list *lptr);

#endif
