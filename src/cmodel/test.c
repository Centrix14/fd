#include "../fd_core.h"

#include "cmodel.h"

void init(list *msg_list) {
	set_msg_list(msg_list);
}

void model() {
	double xy[] = {2, 2};
	int rgb[] = {255, 255, 255};

	label(xy, rgb, 16, "Corbel", "Hello, world!");

	info();
}

int main() {
	list *geometry_buffer = NULL;

	geometry_buffer = list_init_node(NULL);
	pl_send("msg:geometry_buffer", &geometry_buffer, sizeof(list*));

	init(pl_get_msg_list());
	model();
	return 0;
}
