#ifndef __FLAT_DRAW_CAD_SHARED_OBJECT_MANAGING_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CAD_SHARED_OBJECT_MANAGING_LIBRARY_HEADER_FILE_INCLUDED__

void sl_open_file(char *name);
void sl_close();

void *sl_get_function(char *name);

#endif
