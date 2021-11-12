#include <stdio.h>

#include "../fd_core.h"
#include "../st.h/st.h"

st_debug_start(1);
st_name("t1");

int main(void) {
	st_start();
	st_descr("Check gel_reflect_coord");

	printf("b(5; 5)\tt(3; 5)\n");
	printf("r(%g; 5)\t7\n", gel_reflect_coord(5, 3));

	st_end();
	return 0;
}
