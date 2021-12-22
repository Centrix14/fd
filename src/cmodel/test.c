#include "../fd_core.h"

#include "cmodel.h"

void init(list *msg_list) {
	set_msg_list(msg_list);
}

void model() {
	rect_xy(100, 100, 400, 400);

	info();
}
