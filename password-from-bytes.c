/*
 * Copyright (c) 2017 Guenther Brunthaler. All rights reserved.
 *
 * This source file is free software.
 * Distribution is permitted under the terms of the GPLv3.
 */

#include "dim_sdbrke8ae851uitgzm4nv3ea2.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

static char const alphabet[]= {
     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'
   , 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
   , 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'
   , 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
   , '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};

static char *number;

static void cleanup() {
   free(number);
}

static void die(char const *msg) {
   (void)fprintf(stderr, msg);
   (void)fputc('\n', stderr);
   exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
   assert(DIM(alphabet) == 62);
   atexit(cleanup);
   return EXIT_SUCCESS;
}
