#include <stdio.h>

#include "../text/text.h"
#include "../figure/figure.h"
#include "../list/list.h"
#include "../fd_format/fd_format.h"
#include "../st/st.h"

st_name("t2");

int main(void) {
	st_start();
	st_descr("Simple write figure");

	FILE *tf;
	list *lptr = NULL;
	figure *fptr = NULL;

	// open file
	tf = fopen("figure.fd", "w");

	// create sample figure
	fptr = figure_new_rect_pp(45, 80, 200, 200);

	// create node
	lptr = list_init_node(NULL);
	list_set_data(lptr, fptr);
	lptr->dt = OT_FIGURE;

	st_step("call fd_write_object_stream");
	fdl_write_object_stream(tf, lptr);

	// free memory
	figure_free(fptr);
	list_free_node(lptr);

	// close file
	fclose(tf);

	st_end();
	return 0;
}
