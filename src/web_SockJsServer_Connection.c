/* $CORTO_GENERATED
 *
 * web_SockJsServer_Connection.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "web.h"

/* $header() */
#include "mongoose.h"
/* $end */

corto_void _web_SockJsServer_Connection_send(web_SockJsServer_Connection this, corto_string msg) {
/* $begin(corto/web/SockJsServer/Connection/send) */
	int escapedLength;
	corto_string sockJsMsg;

	/* Escape & pack message in SockJS header */
	escapedLength = stresc(NULL, 0, msg);
	sockJsMsg = corto_alloc(escapedLength + strlen("a[\"\"]") + 1);
	sprintf(sockJsMsg, "a[\"%*s\"]", escapedLength, " ");
	stresc(sockJsMsg + 3, escapedLength, msg);

	if (this->conn) {
		mg_websocket_printf((struct mg_connection *)this->conn, WEBSOCKET_OPCODE_TEXT, sockJsMsg);
	}

	corto_dealloc(sockJsMsg);
/* $end */
}
