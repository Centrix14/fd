#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags.h"

#include "../pechkin/pl.h"

char help_msg[] = \
	"\nUsage: fd <flags>\n"\
	"Flags:\n"\
	"      q        -- special debugging flag, just prints `Done`, if all good\n"\
	"      h        -- prints help\n"\
	"      v        -- prints FlatDraw version\n"\
	"      plug-off -- switches off plug-ins loading\n"\
	"      links    -- prints useful links";

void fl_parse(int argc, char *argv[]) {
	int first = 0, second = 0; // first, second chars of each flag
	char *flag = NULL;

	for (int i = 0; i < argc; i++) {
		flag = argv[i];
		first = *flag;
		second = flag[1];

		if (first != '-')
			continue;
		if (second != '-')
			fl_parse_letter(flag[1]);
		else
			fl_parse_str(&flag[2]);
	}
}

void fl_parse_letter(int lett) {
	void (*funcs[])() = {fl_flag_q, fl_flag_h, fl_flag_v};
	char single_flags[] = "qhv";

	for (int i = 0; i < strlen(single_flags); i++) {
		if (single_flags[i] == lett) {
			(*funcs[i])();

			return ;
		}
	}

	printf("*** Unknown flag: `-%c`\n", lett);
}

void fl_parse_str(char *str) {
	void (*funcs[])() = {fl_flag_plug_off, fl_flag_links};
	char *flags[] = {"plug-off", "links", NULL};

	for (int i = 0; flags[i]; i++) {
		if (!strcmp(flags[i], str)) {
			(*funcs[i])();

			return ;
		}
	}

	printf("*** Unknown flag: `--%s`\n", str);
}

void fl_flag_q() {
	puts("*** Done");

	exit(0);
}

void fl_flag_h() {
	printf("*** FlatDraw 0.17 by Centrix\n");
	puts(help_msg);

	exit(0);
}

void fl_flag_v() {
	printf("*** FlatDraw 0.17 by Centrix\n");

	exit(0);
}

void fl_flag_plug_off() {
	int state = 1;

	pl_send("msg:plug_off", &state, sizeof(int));
}

void fl_flag_links() {
	puts("Page on github.com:   https://github.com/Centrix14/fd");
	puts("Realises page:        https://github.com/Centrix14/fd/releases");
	puts("Source documentation: https://github.com/Centrix14/fd/blob/master/doc/fd_doc.md");
	puts("User manual:          https://github.com/Centrix14/fd/blob/master/doc/user_manual.md");

	exit(0);
}
