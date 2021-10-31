#include <stdio.h>

#include "../fd_core.h"

void plug_main(list *msg_list) {
	
}

void __plug_call_windows() {
	printf("Hello!\n");
}

void __plug_call_nix() {
	
}

void plug_call(list *msg_list) {
	int sys_code = -1;

	pl_set_msg_list(msg_list);

	sys_code = ul_get_system_code();
	if (sys_code == UL_OSC_WINDOWS)
		__plug_call_windows();
	else
		__plug_call_nix();
}
