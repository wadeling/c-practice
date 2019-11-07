#include <stdio.h>
#include <string.h>
#include "llhttp.h"

int handle_on_message_begin(llhttp_t* type) {
    printf("msg begin,type %d, method  %d\r\n",type->type,type->method);
    return 0;
}

int main(int argc, char** argv) {
    llhttp_t parser;
    llhttp_settings_t settings;

    /* Initialize user callbacks and settings */
    llhttp_settings_init(&settings);

    /* Set user callback */
//    settings.on_message_complete = handle_on_message_complete;
    settings.on_message_begin = handle_on_message_begin;

    /* Initialize the parser in HTTP_BOTH mode, meaning that it will select between
     * HTTP_REQUEST and HTTP_RESPONSE parsing automatically while reading the first
     * input.
     */
    llhttp_init(&parser, HTTP_REQUEST, &settings);

    /* Parse request! */
    const char* request = "GET / HTTP/1.1\r\n\r\n";
    int request_len = strlen(request);

//    llhttp_pause(&parser); // will pause excute
    
    enum llhttp_errno err = llhttp_execute(&parser, request, request_len);
    if (err == HPE_OK) {
        /* Successfully parsed! */
        printf("parsed ok.\r\n");
    } else {
        fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                parser.reason);
    }


    printf(" llhttp_get_errno(&parser) %d\r\n",llhttp_get_errno(&parser));

    const char* error_pos = llhttp_get_error_pos(&parser);
    if (error_pos != NULL) {
        printf("get err pos \r\n");
    } else {
        printf("no err pos \r\n");
    }

	printf("end");

	return 0;
}
