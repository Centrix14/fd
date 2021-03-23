#include <stdio.h>

#include "../st.h/st.h"
#include "util.h"

st_name("t3");
st_debug_start(1);

int main() {
	st_start();

	printf("Is \"unexist.txt\" exist? %d\n", ul_is_exist("unexist.txt\n"));

	st_end();
	return 0;
}
