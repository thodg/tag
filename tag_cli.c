/* Copyright 2018 Thomas de Grivel <thoxdg@gmail.com> 0614550127 */

#include <stdio.h>
#include "tag.h"

int usage (const char *argv0)
{
  fprintf(stderr, "Usage: %s OPTIONS FILE ...\n", argv0);
  return 1;
}

int main (int argc, char *argv[])
{
  if (argc < 4)
    return usage(argv[0]);
  (void) argv;
  return 0;
}
