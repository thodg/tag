/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#include <stdio.h>
#include <string.h>
#include "tag.h"

#define SEARCH_SIZE 4096

static long tag_file_size (FILE *fp);

int tag_data_search (const char *buf, const char *tag, unsigned len,
                     unsigned start)
{
  unsigned end = TAG_SEARCH_LEN - len;
  while (start < end) {
    if (memcmp(buf + start, tag, len) == 0)
      return start;
    start++;
  }
  return -1;
}

long tag_file_size (FILE *fp)
{
  fseek(fp, 0, SEEK_END);
  return ftell(fp);
}

long tag_data_seek (FILE *fp)
{
  char buf[TAG_SEARCH_LEN];
  long pos = ftell(fp);
  long end = tag_file_size(fp);
  while (pos < end) {
    fseek(fp, pos, SEEK_SET);
    size_t br = fread(buf, 1, TAG_SEARCH_LEN, fp);
    int start;
    if (br < TAG_START_LEN)
      return -1;
    start = tag_data_search(buf, TAG_START, TAG_START_LEN, 0);
    if (start >= 0) {
      pos += start;
      fseek(fp, pos, SEEK_SET);
      return pos;
    }
    pos += TAG_SEARCH_LEN - TAG_START_LEN;
  }
  return -1;
}

int tag_data_read (FILE *fp, s_tag_data *td)
{
  size_t br = fread(td->byte, 1, TAG_SEARCH_LEN, fp);
  int end;
  if (br < TAG_START_LEN + TAG_END_LEN)
    return -1;
  if (memcmp(TAG_START, td->byte, TAG_START_LEN))
    return -1;
  end = tag_data_search(td->byte, TAG_END, TAG_END_LEN,
                        TAG_START_LEN);
  if (end < 0)
    return -1;
  td->len = end + TAG_END_LEN;
  return 0;
}

int tag_data_from_file (FILE *fp, s_tag_data *td)
{
  if (!fp)
    return -1;
  if (fseek(fp, -TAG_SEARCH_LEN, SEEK_END) < 0)
    return -1;
  if (tag_data_seek(fp) < 0) {
    fseek(fp, 0, SEEK_SET);
    if (tag_data_seek(fp) < 0)
      return -1;
  }
  return tag_data_read(fp, td);
}

int tag_data_from_path (const char *path, s_tag_data *td)
{
  FILE *fp = fopen(path, "r");
  int result = -1;
  if (!fp)
    goto end;
  if (fseek(fp, -TAG_SEARCH_LEN, SEEK_END) < 0)
    goto end;
  if (tag_data_seek(fp) < 0) {
    fseek(fp, 0, SEEK_SET);
    if (tag_data_seek(fp) < 0)
      goto end;
  }
  result = tag_data_read(fp, td);
 end:
  fclose(fp);
  return result;
}

int tag_data_write (FILE *fp, s_tag_data *td)
{
  if (td->len > TAG_SEARCH_LEN)
    return -1;
  if (fwrite(td->byte, td->len, 1, fp) != 1)
    return -1;
  return 0;
}

int tag_data_to_file (FILE *fp, s_tag_data *td)
{
  if (!fp)
    return -1;
  if (fseek(fp, 0, SEEK_END) < 0)
    return -1;
  return tag_data_write(fp, td);
}

int tag_data_to_path (const char *path, s_tag_data *td)
{
  FILE *fp = fopen(path, "a");
  int result;
  if (!fp)
    return -1;
  result = tag_data_write(fp, td);
  fclose(fp);
  return result;
}

void tag_data_mem (s_tag_data *td, const char *buf, unsigned len)
{
  memcpy(td->byte + td->len, buf, len);
  td->len += len;
}

void tag_data_str (s_tag_data *td, const char *str)
{
  unsigned len = strlen(str);
  memcpy(td->byte + td->len, str, len);
  td->len += len;
}

void tag_data_tag (s_tag_data *td, const char *key,
                   const char *value)
{
  tag_data_str(td, " <");
  tag_data_str(td, key);
  tag_data_str(td, ">");
  tag_data_str(td, value);
  tag_data_str(td, "</");
  tag_data_str(td, key);
  tag_data_str(td, ">\n");
}

void tag_data_start (s_tag_data *td)
{
  td->len = 0;
  tag_data_mem(td, TAG_START, TAG_START_LEN);
}

void tag_data_end (s_tag_data *td) {
  tag_data_mem(td, TAG_END, TAG_END_LEN);
}
