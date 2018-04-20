/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include "tag.h"

void tag_too_large (s_tag_data *td)
{
  if (tag_data_write(stdout, td) < 0)
    err(1, "stdout");
}

void tag_ended (FILE *tag_fp, s_tag_data *td)
{
  if (tag_data_write(tag_fp, td) < 0)
    err(1, "tag output");
}

void tag_started (FILE *tag_fp)
{
  s_tag_data td;
  int c;
  unsigned end_len = 0;
  tag_data_start(&td);
  while ((c = getchar()) != EOF) {
    td.byte[td.len] = c;
    td.len++;
    if (td.len == TAG_SEARCH_LEN) {
      tag_too_large(&td);
      return;
    }
    if (c == TAG_END[end_len]) {
      end_len++;
      if (end_len == TAG_END_LEN) {
        tag_ended(tag_fp, &td);
        return;
      }
    }
    else
      end_len = 0;
  }
}

long tag_demux (FILE *tag_fp)
{
  unsigned start_len = 0;
  int c;
  while ((c = getchar()) != EOF) {
    if (c == TAG_START[start_len]) {
      start_len++;
      if (start_len == TAG_START_LEN) {
        tag_started(tag_fp);
        start_len = 0;
      }
    }
    else {
      if (start_len > 0) {
        fwrite(TAG_START, start_len, 1, stdout);
        start_len = 0;
      }
      putchar(c);
    }
  }
  return feof(stdin) ? 0 : errno;
}

int main () {
  FILE *tag_fp = fdopen(3, "w");
  long res = tag_demux(tag_fp);
  fflush(stdout);
  fclose(tag_fp);
  if (res) {
    errno = res;
    err(1, "stdin");
  }
  return 0;
}
