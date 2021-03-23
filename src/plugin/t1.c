#include <stdio.h>
#include <gtk/gtk.h>

#include "../st.h/st.h"
#include "../list/list.h"
#include "plugin.h"

st_name("t1");
st_debug_start(1);

int main() {
	st_start();

	pil_load_plugins(NULL, NULL);

	st_end();
	return 0;
}
