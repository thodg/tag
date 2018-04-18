/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tag.h"

int g_print = 0;
int g_print_raw = 0;

int usage (const char *argv0)
{
  fprintf(stderr, "Usage: %s OPTIONS FILE ...\n", argv0);
  fputs("Available options are :\n", stderr);
  fputs("  -a  artist name\n"
        "  -d  album title\n"
        "  -g  genre\n"
        "  -n  track number\n"
        "  -t  title\n"
        "  -y  year\n"
        "  -_  KEY=VALUE\n"
        "  -f   format (%adgnty{})\n"
        "  -i   print xml tag data\n"
        "  -r   print raw tag data\n", stderr);
  return 1;
}

void tag_data_kv (s_tag_data *td, char *kv)
{
  char *equal = strchr(kv, '=');
  if (equal) {
    unsigned len = equal - kv;
    char *value = equal + 1;
    tag_data_str(td, " <");
    tag_data_mem(td, kv, len);
    tag_data_str(td, ">");
    tag_data_str(td, value);
    tag_data_str(td, "</");
    tag_data_mem(td, kv, len);
    tag_data_str(td, ">\n");
  }
}

void tag_data_opt (s_tag_data *td, int argc, char *argv[])
{
  int opt;
  tag_data_start(td);
  while ((opt = getopt(argc, argv, "a:d:f:g:in:rt:y:_:")) != -1) {
    switch (opt) {
    case 'a': tag_data_tag(td, "artist", optarg); break;
    case 'd': tag_data_tag(td, "album", optarg); break;
    case 'f': break;
    case 'g': tag_data_tag(td, "genre", optarg); break;
    case 'i': g_print = 1; break;
    case 'n': tag_data_tag(td, "track", optarg); break;
    case 'r': g_print = 1; g_print_raw = 1; break;
    case 't': tag_data_tag(td, "title", optarg); break;
    case 'y': tag_data_tag(td, "year", optarg); break;
    case '_': tag_data_kv(td, optarg); break;
    }
  }
  tag_data_end(td);
}

int main (int argc, char *argv[])
{
  s_tag_data td;
  if (argc < 2)
    return usage(argv[0]);
  tag_data_opt(&td, argc, argv);
  if (g_print) {
    if (g_print_raw)
      fwrite(td.byte, td.len, 1, stdout);
    else
      fputs(td.byte + 3, stdout);
  }
  return 0;
}
