#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cola.h>

const char *cmd = "cola";

static int usage(int code)
{
	FILE *f = (code) ? stderr : stdout;

	fprintf(f, "%s: Usage\n", cmd);
	fprintf(f, "\t$ %s create [-f] <fn>\n", cmd);
	fprintf(f, "\t$ %s query <fn> <key>\n", cmd);
	fprintf(f, "\t$ %s insert <fn> <key>\n", cmd);
	fprintf(f, "\t$ %s help\n", cmd);
	fprintf(f, "\n");

	return code;
}

static int do_create(int argc, char **argv)
{
	struct _cola *c;
	const char *fn;
	int force = 0;
	int i = 1;

	switch(argc) {
	case 3:
		if ( !strcmp(argv[1], "-f") ) {
			force = 1;
			i++;
		}else
			return usage(EXIT_FAILURE);
	case 2:
		fn = argv[i];
		break;
	case 1:
	default:
		return usage(EXIT_FAILURE);
	}

	c = cola_creat(fn, force);
	if ( NULL == c )
		return EXIT_FAILURE;

	cola_close(c);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	unsigned int i;
	static const struct {
		const char * const cmd;
		int (*fn)(int argc, char **argv);
	}fn[] = {
		{"create", do_create},
	};

	if ( argc > 0 )
		cmd = argv[0];
	if ( argc < 2 )
		return usage(EXIT_FAILURE);

	for(i = 0; i < sizeof(fn)/sizeof(*fn); i++) {
		if ( !strcmp(fn[i].cmd, argv[1]) ) {
			return fn[i].fn(argc - 1, argv + 1);
		}
	}

	return usage(EXIT_FAILURE);
}