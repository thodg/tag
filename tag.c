/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#include <stdio.h>
#include "tag.h"

#define SEARCH_SIZE 4096

static int tag_cmp (const char *buf1, const char *buf2, unsigned count);
static long tag_file_size (FILE *fp);

int tag_cmp (const char *buf1, const char *buf2, unsigned count)
{
  unsigned i = 0;
  while (i < count) {
    if (buf1[i] < buf2[i])
      return -1;
    if (buf1[i] > buf2[i])
      return 1;
    i++;
  }
  return 0;
}

int tag_search (const char *buf, const char *tag, unsigned len,
                unsigned start)
{
  unsigned end = TAG_SEARCH_LEN - len;
  while (start < end) {
    if (tag_cmp(buf + start, tag, len) == 0)
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
    start = tag_search(buf, TAG_START, TAG_START_LEN, 0);
    if (start >= 0) {
      pos += start;
      fseek(fp, pos, SEEK_SET);
      return pos;
    }
    pos += TAG_SEARCH_LEN - TAG_START_LEN;
  }
  return -1;
}

int tag_data_read (FILE *fp, s_tag_data *tag_data)
{
  size_t br = fread(tag_data->byte, 1, TAG_SEARCH_LEN, fp);
  int end;
  if (br < TAG_START_LEN + TAG_END_LEN)
    return -1;
  if (tag_cmp(TAG_START, tag_data->byte, TAG_START_LEN))
    return -1;
  end = tag_search(tag_data->byte, TAG_END, TAG_END_LEN, TAG_START_LEN);
  if (end < 0)
    return -1;
  tag_data->len = end + TAG_END_LEN;
  return 0;
}

int tag_data_from_file (FILE *fp, s_tag_data *tag_data)
{
  if (!fp)
    return -1;
  if (tag_data_seek(fp) < 0)
    return -1;
  return tag_data_read(fp, tag_data);
}

int tag_data_from_path (const char *path, s_tag_data *tag_data)
{
  FILE *fp = fopen(path, "r");
  int result;
  if (!fp)
    return -1;
  if (tag_data_seek(fp) < 0)
    return -1;
  result = tag_data_read(fp, tag_data);
  fclose(fp);
  return result;
}

int tag_data_write (FILE *fp, s_tag_data *tag_data)
{
  if (tag_data->len > TAG_SEARCH_LEN)
    return -1;
  if (fwrite(tag_data->byte, tag_data->len, 1, fp) != 1)
    return -1;
  return 0;
}

int tag_data_to_file (FILE *fp, s_tag_data *tag_data)
{
  if (!fp)
    return -1;
  if (fseek(fp, 0, SEEK_END) < 0)
    return -1;
  return tag_data_write(fp, tag_data);
}

int tag_data_to_path (const char *path, s_tag_data *tag_data)
{
  FILE *fp = fopen(path, "a");
  int result;
  if (!fp)
    return -1;
  result = tag_data_write(fp, tag_data);
  fclose(fp);
  return result;
}
