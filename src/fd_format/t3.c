#include <stdio.h>

#include "../figure/figure.h"
#include "../list/list.h"
#include "../figure/figure.h"
#include "../st/st.h"
#include "fd_format.h"

st_name("t3");

int main(void) {
	st_start();
	st_descr("Simple read figure");

	list *node;

	node = list_init_node(NULL);

	fdl_target_file("figure.fd");
	fdl_read_file(node);

	list_free_list(node);

	st_end();
	return 0;
}
