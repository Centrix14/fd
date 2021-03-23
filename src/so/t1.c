#include "../st.h/st.h"
#include "so.h"

st_name("t1");
st_debug_start(1);

int main() {
	st_start();

	void (*fn)(char*) = NULL;

	st_call(sl_open_file, "test.so");

	fn = sl_get_function("test_main");
	(*fn)("msg from t1.c");

	st_call(sl_close);

	st_end();
	return 0;
}
