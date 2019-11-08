#include <stdio.h>
#include <string.h>
#include <time.h>
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
//    settings.on_message_begin = handle_on_message_begin;

    /* Initialize the parser in HTTP_BOTH mode, meaning that it will select between
     * HTTP_REQUEST and HTTP_RESPONSE parsing automatically while reading the first
     * input.
     */
    llhttp_init(&parser, HTTP_BOTH, &settings);

    /* Parse request! */
//    const char* request = "GET / HTTP/1.1\r\n"
//                         "Host: 127.0.0.1:9211\r\n"
//                         "User-Agent: fortio.org/fortio-1.3.2-pre\r\n"
//                         "X-Id: 1\r\n";
    const char* request = "POST /joyent/http-parser HTTP/1.1\r\n"
    "Host: github.com\r\n"
    "DNT: 1\r\n"
    "Accept-Encoding: gzip, deflate, sdch\r\n"
    "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) "
    "AppleWebKit/537.36 (KHTML, like Gecko) "
    "Chrome/39.0.2171.65 Safari/537.36\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,"
    "image/webp,*/*;q=0.8\r\n"
    "Referer: https://github.com/joyent/http-parser\r\n"
    "Connection: keep-alive\r\n"
    "Transfer-Encoding: chunked\r\n"
    "Cache-Control: max-age=0\r\n\r\nb\r\nhello world\r\n0\r\n";

    int request_len = strlen(request);

    time_t start,end;
    start = clock();
    //uint64_t num = (uint64_t)1 << 1;
    uint64_t num = 16582885;
    printf("test num %llu,request len %d\r\n",num,request_len);
    for (uint64_t i = 0; i < num; ++i) {
        llhttp_init(&parser, HTTP_BOTH, &settings);
        enum llhttp_errno err = llhttp_execute(&parser, request, request_len);
        if (err == HPE_OK) {
            /* Successfully parsed! */
//            printf("parsed ok.\r\n");
        } else {
            fprintf(stderr, "%llu Parse error: %s %s\n",i, llhttp_errno_name(err),
                    parser.reason);
            const char* error_pos = llhttp_get_error_pos(&parser);
            if (error_pos != NULL) {
                printf("get err pos,%s \r\n",error_pos);
            } else {
                printf("no err pos \r\n");
            }
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
