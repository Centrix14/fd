#include <stdio.h>

#include "list.h"
#include "../st/st.h"

int main() {
	st_name("t2");
	st_start();
	st_descr("Append test");

	list *lptr = NULL;

	st_step("list_init_node");
	lptr = list_init_node(NULL);

	st_step("list_add_node");
	list_add_node(lptr);

	st_step("list_free_list");
	list_free_list(lptr);

	st_end();
	return 0;
}
