/* web.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#define corto_web_LIB
#include "web.h"

/* ::corto::web::random(uint16 n) */
cx_string _web_random(cx_uint16 n) {
/* $begin(::corto::web::random) */
	static char *alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static char *alphanum = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
	cx_uint16 i;
	cx_string result = cx_alloc(n + 1);

	result[0] = alpha[rand() % strlen(alpha)];
	for (i = 1; i < n; i++) {
		result[i] = alphanum[rand() % strlen(alphanum)];
	}

	result[i] = '\0';

	return result;
/* $end */
}

int webMain(int argc, char* argv[]) {
/* $begin(main) */
    /* Insert code that must be run when component is loaded */
    CX_UNUSED(argc);
    CX_UNUSED(argv);
    return 0;
/* $end */
}
