#include "lrc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* gcc example.c -llrc */

int main(int argc, char **argv) {

  int k, m, i;
  int size = 8;
  lrc_t *lrc = &(lrc_t){0};
  lrc_buf_t *buf = &(lrc_buf_t){0};

  if (lrc_init_n(lrc, 2, (uint8_t[]){2, 2}, 3) != 0) {
    exit(-1);
  }

  if (lrc_buf_init(buf, lrc, size) != 0) {
    exit(-1);
  }

  strcpy(buf->data[0], "hello");
  strcpy(buf->data[1], "world");
  strcpy(buf->data[2], "lrc");
  strcpy(buf->data[3], "ec");

  if (lrc_encode(lrc, buf) != 0) {
    exit(-1);
  }

  for (k = 0; k < lrc->k; k++) {
    printf("data[%d]: ", k);
    for (i = 0; i < size; i++) {
      printf("%02x ", (uint8_t)buf->data[k][i]);
    }
    printf("\n");
  }

  for (m = 0; m < lrc->m; m++) {
    printf("code[%d]: ", m);
    for (i = 0; i < size; i++) {
      printf("%02x ", (uint8_t)buf->code[m][i]);
    }
    printf("\n");
  }

  int8_t erased[2 + 2 + 3] = {0, 0, 0, 0, 0, 0, 0};

  printf("damaged: %s %s %s %s\n", buf->data[0], buf->data[1], buf->data[2],
         buf->data[3]);

  for (int i = 0; i < (1 << 7); i++) {
    memset(erased, 0, sizeof(erased));
    for (int j = 0; j < 7; j++) {
      if (((i >> j) & 1) == 1) {
        erased[j] = 1;
      }
    }
    if (lrc_decode(lrc, buf, erased) != 0) {
      for (int j = 0; j < 7; j++) {
        if (((i >> j) & 1) == 1) {
          printf("1 ");
        } else {
          printf("0 ");
        }
      }
      printf("\n");
    }
  }
  if (lrc_decode(lrc, buf, erased) != 0) {
    exit(-1);
  }

  printf("reconstructed: %s %s %s %s\n", buf->data[0], buf->data[1],
         buf->data[2], buf->data[3]);

  lrc_destroy(lrc);
  lrc_buf_destroy(buf);
  // char str[] = "Gooood Moooorning Heeelllo Woooorld";
  // char *seg[7] = {0};
  // for (int i = 0; i < 7; i++) {
  //   seg[i] = calloc(1024, 1);
  // }
  // int seg_size[7];
  // lrc_encode_223(str, strlen(str), seg, seg_size);
  // char decode[1024];
  // bzero(decode, 1024);
  // for (int i = 0; i < 7; i++) {
  //   printf("%d\n", seg_size[i]);
  // }
  // int8_t erased[7] = {1, 1, 0, 1, 0, 0, 0};
  // if (lrc_decode_223(seg, seg_size, erased, decode) != -1) {
  //   printf("%s\n", decode);
  // } else {
  //   printf("error\n");
  // }
  return 0;
}

// vim:sw=2:fdl=0
