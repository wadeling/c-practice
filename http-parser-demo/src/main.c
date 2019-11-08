#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "http_parser.h"


int main(int argc, char** argv) {
    http_parser *parser = malloc(sizeof(http_parser));
    http_parser_settings settings;

    /* Set user callback */
//    settings.on_message_complete = handle_on_message_complete;
//    settings.on_message_begin = handle_on_message_begin;

    http_parser_init(parser, HTTP_BOTH);

    /* Parse request! */
    const char* request = "GET / HTTP/1.1\r\n"
                         "Host: 127.0.0.1:9211\r\n"
                         "User-Agent: fortio.org/fortio-1.3.2-pre\r\n"
                         "X-Id: 1\r\n";
    int request_len = strlen(request);

    time_t start,end;
    uint64_t num = (uint64_t)1 << 0;
    printf("test num %ld\r\n",num);
    start = clock();
    for (uint64_t i = 0; i < num; ++i) {
        http_parser_init(&parser, HTTP_BOTH);
        ssize_t rc = http_parser_execute((&parser, request, request_len);
        if (HTTP_PARSER_ERRNO(parser) != HPE_OK && HTTP_PARSER_ERRNO(parser) != HPE_PAUSED) {
            printf("parse err: %s\r\n",http_errno_name(HTTP_PARSER_ERRNO(parser)));
            break;
        }
    }

    end = clock();
    double total_time = difftime(end,start)/CLOCKS_PER_SEC;
    double parsed_time = total_time/(double)(num);
    printf("take time %f s, every parse action take time %f us\r\n",total_time,parsed_time*1000*1000);

	printf("end");

	return 0;
}
