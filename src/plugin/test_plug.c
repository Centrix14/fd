#include "../fd_core.h"

void plug_main(list *msg_list) {
	int *plug_off_flag = NULL;

	pl_set_msg_list(msg_list);

	printf("+++ This plugin gets arguments via pechkin library\n");
	printf("+++ geometry_buff exists?: %d\n", pl_is_exist("msg:geometry_buffer"));
}
