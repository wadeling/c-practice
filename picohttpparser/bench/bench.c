
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "picohttpparser.h"

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
    "Cache-Control: max-age=0\r\n\r\n";
static const size_t data_len = sizeof(data) - 1;


int bench(int iter_count, int silent) {
    const char *method;
    size_t method_len;
    const char *path;
    size_t path_len;
    int minor_version;
    struct phr_header headers[32];
    size_t num_headers;

    int i;
    int err;
//    int ret;
    struct timeval start;
    struct timeval end;

    if (!silent) {
      err = gettimeofday(&start, NULL);
      assert(err == 0);
    }

    fprintf(stderr, "req_len=%d\n", (int) data_len);
    for (i = 0; i < iter_count; i++) {
//    for (i = 0; i < 1; i++) {
        num_headers = sizeof(headers) / sizeof(headers[0]);
        phr_parse_request(data, data_len, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers,0);
//        assert(ret == data_len);
//        printf("request is %d bytes long\n", ret);
//        printf("method is %.*s\n", (int)method_len, method);
//        printf("path is %.*s\n", (int)path_len, path);
//        printf("HTTP version is 1.%d\n", minor_version);
//        printf("headers:\n");
//        for (i = 0; i != num_headers; ++i) {
//            printf("%.*s: %.*s\n", (int)headers[i].name_len, headers[i].name,
//                   (int)headers[i].value_len, headers[i].value);
//        }
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

      fprintf(stdout, "%.2f mb | %.2f mb/s | %d iterator count | %.2f req/sec | %.2f us/req | %.2f s\n",
          (double) total / (1024 * 1024),
          bw / (1024 * 1024),
  		iter_count,
          (double) iter_count / elapsed,
          (elapsed / (double) iter_count)*1000*1000 ,
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
