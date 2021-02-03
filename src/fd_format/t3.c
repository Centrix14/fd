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

	fdl_target_file("figure.fd");
	fdl_read_file(NULL);

	st_end();
	return 0;
}
