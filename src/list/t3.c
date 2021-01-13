#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "../st/st.h"

void free_data(list *lptr);

int main() {
	st_name("t3");
	st_start();
	st_descr("Append / fill test");

	list *lptr = NULL, *iter = NULL;
	char c[] = "!";

	lptr = list_init_node(NULL);

	st_step("First loop: append / fill");
	for (int i = 0; i < 10; i++, c[0]++) {
		list_add_node(lptr);

		iter = list_get_last(lptr);
		list_set_data(iter, (char*)strdup(c));
	}

	st_step("Second loop: printing");
	iter = lptr;
	for (int i = 0; i < 10; i++) {
		if (iter->data)
			puts((char*)iter->data);

		iter = iter->next;
	}

	list_crawl(lptr, free_data);
	list_free_list(lptr);

	st_end();
	return 0;
}

void free_data(list *lptr) {
	if (lptr->data)
		free(lptr->data);
}
