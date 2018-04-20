/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#ifndef TAG_H
#define TAG_H

#include <stdio.h>

#define TAG_START "\0\0\n<tag>\n"
#define TAG_START_LEN 9

#define TAG_END "</tag>\n\0\0"
#define TAG_END_LEN 9

#define TAG_SEARCH_LEN 2048

/* 1. TAG DATA : read and write tag data region from and to file. */
/* Size is limited to 2048 bytes including start and end markers. */
/* Content is thus limited to 2030 bytes. Not a short story. Only */
/* inline metadata about current file, better at start of stream. */

typedef struct tag_data {
  unsigned short len;
  char byte[TAG_SEARCH_LEN];
} s_tag_data;

/* search tag in buffer */
int tag_data_search (const char *buf, const char *tag, unsigned len,
                     unsigned start);

/* seek forward in file until start tag is found */
long tag_data_seek (FILE *fp);

/* read tag data at current position */
int tag_data_read (FILE *fp, s_tag_data *td);

/* read tag data from open file starting at end */
int tag_data_from_file (FILE *fp, s_tag_data *td);

/* open file and read tag data starting at end */
int tag_data_from_path (const char *path, s_tag_data *td);

/* write tag data at current position */
int tag_data_write (FILE *fp, s_tag_data *td);

/* write tag data at end of file */
int tag_data_to_file (FILE *fp, s_tag_data *td);

/* open file and write tag data at end of file */
int tag_data_to_path (const char *path, s_tag_data *td);

/* write buffer to s_tag_data */
void tag_data_mem (s_tag_data *td, const char *buf, unsigned len);

/* write string to s_tag_data */
void tag_data_str (s_tag_data *td, const char *str);

/* write start tag to s_tag_data */
void tag_data_start (s_tag_data *td);

/* write end tag to s_tag_data */
void tag_data_end (s_tag_data *td);

/* write key value tag to s_tag_data */
void tag_data_tag (s_tag_data *td, const char *key, const char *value);

/* read next key value from s_tag_data, return position */
int tag_read (s_tag_data *td, int start,
              char *key, unsigned key_size,
              char *value, unsigned value_size);

/* find position of key in s_tag_data */
int tag_find (s_tag_data *td, int start, const char *key);

/* get value of key in s_tag_data */
int tag_get (s_tag_data *td, int start, const char *key,
             char *value, unsigned value_size);

/* 2. TAG : structure tag data region from and to actual tags */
/* actually tag data is xml enclosed in TAG_START and TAG_END */
/* known tags are : artist, album, genre, track, title, year. */


#endif /* TAG_H */
