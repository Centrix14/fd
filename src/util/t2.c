#include <stdio.h>

#include "../st.h/st.h"
#include "util.h"

st_name("t1");
st_debug_start(1);

int main() {
	st_start();

	printf(".vimrc file path: %s\n", ul_get_file_from_home(".vimrc"));

	st_end();
	return 0;
}
