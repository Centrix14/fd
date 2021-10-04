#include <gtk/gtk.h>

#include "dialog.h"

int main(void) {
	gtk_init(NULL, NULL);
	
	dial_show_file_choose_dialog(DCF_OPEN);

	gtk_main();

	return 0;
}
