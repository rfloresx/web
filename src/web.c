/* $CORTO_GENERATED
 *
 * web.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

corto_string _web_random(corto_uint16 n) {
/* $begin(::corto::web::random) */
	static char *alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static char *alphanum = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
	corto_uint16 i;
	corto_string result = corto_alloc(n + 1);

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
    CORTO_UNUSED(argc);
    CORTO_UNUSED(argv);
    return 0;
/* $end */
}
