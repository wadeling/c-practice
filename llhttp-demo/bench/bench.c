#include "llhttp.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

/* 8 gb */
static const int64_t kBytes = 8LL << 30;

static const char data[] =
    "POST /joyent/http-parser HTTP/1.1\r\n"
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
static const size_t data_len = sizeof(data) - 1;

static int on_info(llhttp_t* p) {
  return 0;
}


static int on_data(llhttp_t* p, const char *at, size_t length) {
  return 0;
}

static llhttp_settings_t settings = {
  .on_message_begin = on_info,
  .on_headers_complete = on_info,
  .on_message_complete = on_info,
  .on_header_field = on_data,
  .on_header_value = on_data,
  .on_url = on_data,
  .on_status = on_data,
  .on_body = on_data
};

int bench(int iter_count, int silent) {
  llhttp_t parser;
  int i;
  int err;
  struct timeval start;
  struct timeval end;

  //llhttp_settings_init(&settings);

  if (!silent) {
    err = gettimeofday(&start, NULL);
    assert(err == 0);
  }

  fprintf(stderr, "req_len=%d\n", (int) data_len);
  for (i = 0; i < iter_count; i++) {
    llhttp_init(&parser, HTTP_REQUEST,&settings);

    enum llhttp_errno err = llhttp_execute(&parser, data, data_len);
    if (err == HPE_OK) {
        /* Successfully parsed! */
//            printf("parsed ok.\r\n");
    } else {
        fprintf(stderr, "%d Parse error: %s %s\n",i, llhttp_errno_name(err),
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

  if (!silent) {
    double elapsed;
    double bw;
    double total;

    err = gettimeofday(&end, NULL);
    assert(err == 0);

    fprintf(stdout, "Benchmark result:\n");

    elapsed = (double) (end.tv_sec - start.tv_sec) +
              (end.tv_usec - start.tv_usec) * 1e-6f;

    total = (double) iter_count * data_len;
    bw = (double) total / elapsed;

    fprintf(stdout, "%.2f mb | %.2f mb/s |%d iterator count | %.2f req/sec | %f us/req | %.2f s\n",
        (double) total / (1024 * 1024),
        bw / (1024 * 1024),
        iter_count,
        (double) iter_count / elapsed ,
        (elapsed / (double)(iter_count))*1000*1000,
        elapsed);

    fflush(stdout);
  }

  return 0;
}

int main(int argc, char** argv) {
  int64_t iterations;

  iterations = kBytes / (int64_t) data_len;
  if (argc == 2 && strcmp(argv[1], "infinite") == 0) {
    for (;;)
      bench(iterations, 1);
    return 0;
  } else {
    return bench(iterations, 0);
  }
}
