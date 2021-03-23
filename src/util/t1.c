#include <stdio.h>

#include "../st.h/st.h"
#include "util.h"

st_name("t1");
st_debug_start(1);

int main() {
	st_start();

	printf("home_path = %s\n", ul_get_home_path());

	st_end();
	return 0;
}
