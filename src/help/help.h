#ifndef __FLAT_DRAW_HELP_MESSAGES_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_HELP_MESSAGES_LIBRARY_HEADER_FILE_INCLUDED__

char *hl_get_help(int code);

void hl_set_widget(GtkWidget *label);
void hl_set_help(int code);

enum HELP_CODES {
	HC_MAIN = 0,
	HC_POINT,
	HC_START_POINT,
	HC_END_POINT,
	HC_CENTER_POINT,
	HC_RADIUS_POINT,
	HC_CUROR,
	HC_INS_POINT,
	HC_BASE_POINT
};

#endif
