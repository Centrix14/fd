#include <stdio.h>

#include "list.h"
#include "../st/st.h"

int main() {
	st_name("t1");
	st_start();
	st_descr("Base test");

	list *lptr = NULL;

	lptr = list_init_node(NULL);
	list_free_node(lptr);

	st_end();
	return 0;
}
