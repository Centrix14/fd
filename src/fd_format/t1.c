#include <stdio.h>

#include "../text/text.h"
#include "../figure/figure.h"
#include "../list/list.h"
#include "../fd_format/fd_format.h"
#include "../st/st.h"

st_name("t1");

int main(void) {
	st_start();
	st_descr("Simple write text");

	FILE *tf;
	list *lptr = NULL;
	text *tptr = NULL;

	// open file
	tf = fopen("text.fd", "w");

	// create sample text
	tptr = tl_new("Arial", 14, 255, 255, 255);

	tl_add_buffer(tptr, "This is test .fd file.\nThe new line test");
	tptr->visible = VM_SHOW;
	tptr->x = 15;
	tptr->y = 15;

	// create node
	lptr = list_init_node(NULL);
	list_set_data(lptr, tptr);
	lptr->dt = OT_TEXT;

	st_step("call fd_write_object_stream");
	fdl_write_object_stream(tf, lptr);

	// free memory
	tl_free_buffer(tptr);
	tl_free(tptr);
	list_free_node(lptr);

	// close file
	fclose(tf);

	st_end();
	return 0;
}
