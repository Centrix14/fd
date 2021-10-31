#ifndef __FLAT_DRAW_CAD_PLUG_IN_LOAD_LIBRARY_HEADER_FILE_INCLUDED
#define __FLAT_DRAW_CAD_PLUG_IN_LOAD_LIBRARY_HEADER_FILE_INCLUDED

void pil_load_plugins(list *msg_list);
void pil_load_list(char *filename, list *msg_list);
void pil_load_plugin(char *filename, list *msg_list);
void pil_call_plugin(char *file_name, char *func_name, list *msg_list);

#endif
