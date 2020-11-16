#ifndef __FLAT_DRAW_FORMAT_PROCESSING_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_FORMAT_PROCESSING_LIBRARY_HEADER_FILE_INCLUDED__

#define FD_FORMAT_DELIM " \n"

void fdl_target_file(char *name);

void fdl_write_figure_stream(FILE *stream, figure *fptr);
void fdl_write_from_list(list *lptr);

void fdl_read_string(figure *fptr, char *str);
void fdl_read_file(list *lptr);

#endif
