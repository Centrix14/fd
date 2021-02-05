#ifndef __FLAT_DRAW_FORMAT_PROCESSING_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_FORMAT_PROCESSING_LIBRARY_HEADER_FILE_INCLUDED__

#define FD_FORMAT_END '\1'

void fdl_target_file(char *name);

void fdl_write_object_stream(FILE *stream, list *lptr);
void fdl_write_from_list(list *lptr);

void fdl_read_string(figure *fptr, char *str);
void fdl_read_file(list *lptr);

void fdl_pars_figure(FILE *stream, list *lptr);
void fdl_pars_text(FILE *stream, list *lptr);

#endif
