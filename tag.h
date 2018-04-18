/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#ifndef TAG_H
#define TAG_H

#include <stdio.h>

#define TAG_START "\0\0\n<tag>\n"
#define TAG_START_LEN 9

#define TAG_END "\n</tag>\n\0\0"
#define TAG_END_LEN 10

#define TAG_SEARCH_LEN 2048

/* 1. TAG DATA : read and write tag data region from and to file. */
/* TAG DATA is limited to 2048 bytes including start and end markers. */
/* Actual TAG DATA payload can thus be 2029 bytes large. */
/* It is not a short story, only inline metadata about current file. */

typedef struct tag_data {
  unsigned short len;
  char byte[TAG_SEARCH_LEN];
} s_tag_data;

int tag_data_search (const char *buf, const char *tag, unsigned len,
                unsigned start);
long tag_data_seek (FILE *fp);
int tag_data_read (FILE *fp, s_tag_data *tag_data);
int tag_data_from_file (FILE *fp, s_tag_data *tag_data);
int tag_data_from_path (const char *path, s_tag_data *tag_data);
int tag_data_write (FILE *fp, s_tag_data *tag_data);
int tag_data_to_file (FILE *fp, s_tag_data *tag_data);
int tag_data_to_path (const char *path, s_tag_data *tag_data);

/* write to s_tag_data */

void tag_data_mem (s_tag_data *tag_data, const char *buf, unsigned len);
void tag_data_str (s_tag_data *tag_data, const char *str);
void tag_data_start (s_tag_data *tag_data);
void tag_data_end (s_tag_data *tag_data);
void tag_data_tag (s_tag_data *tag_data, const char *key,
                   const char *value);

/* 2. TAG : structure tag data region from and to actual tags */
/* actually tag data is xml enclosed in TAG_START and TAG_END */
/* known tags are : title, track, artist, album, year, genre. */
/* Just use XML library to read and write tags to s_tag_data. */


#endif /* TAG_H */
