/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#include <error.h>
#include <stdio.h>
#include "tag.h"

void tag_started (FILE *tag_fp, s_tag_data *td)
{
}

void tag_demux (FILE *tag_fp)
{
  s_tag_data td;
  int c;
  td.len = 0;
  while ((c = getchar()) != EOF) {
    if (c == TAG_START[td.len]) {
      td.len++;
      if (td.len == TAG_START_LEN) {
        tag_started(tag_fp, &td);
        td.len = 0;
      }
      continue;
    }
    td.len = 0;
    
}

int main () {
  FILE *tag_fp = fdopen(3, "w");
  tag_demux(tag_fp);
  if (feof(stdin))
    return 0;
  err(1, "stdin");
  return 1;
}
