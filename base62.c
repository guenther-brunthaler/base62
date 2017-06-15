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
   , '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

#define RADIX DIM(alphabet)

static char *number;

static void cleanup() {
   free(number);
}

static void die(char const *msg) {
   (void)fprintf(stderr, msg);
   (void)fputc('\n', stderr);
   exit(EXIT_FAILURE);
}

#define UNSIGNED_MSB_POS (sizeof(unsigned) * CHAR_BIT - 1)
#define UNSIGNED_MSB_VAL (1u << UNSIGNED_MSB_POS)

/* (int)x < 0 ? UINT_MAX : 0 */
static unsigned NEGP(unsigned x) {
   return ~(x >> UNSIGNED_MSB_POS) + 1;
}

/* x < y ? UINT_MAX : 0 */
static unsigned LTP(unsigned x, unsigned y) {
   return NEGP(x - y);
}

/* x == 0 ? 0 : UINT_MAX */
static unsigned NZEROP(unsigned x) {
   return LTP(x, 0) | LTP(0, x);
}

int main(int argc, char **argv) {
   unsigned reserved= 1024, length= 1;
   int byte;
   assert(RADIX == 62);
   atexit(cleanup);
   if (!(number= malloc(reserved))) {
      mem_err: die("Could not allocate space for the password!");
   }
   assert(length == 1 && length <= reserved);
   number[0]= 0;
   assert((char)(1 << CHAR_BIT - 1) > 0);
   while ((byte= getchar()) != EOF) {
      int bit;
      for (bit= 1 << CHAR_BIT - 1; bit; bit>>= 1) {
         unsigned i, acc= NZEROP(byte & bit) & 1;
         for (i= 0; i < length; ++i) {
            unsigned cf;
            acc+= number[i] << 1;
            cf= RADIX - 1 - acc & UNSIGNED_MSB_VAL;
            assert(cf == 0 || cf == UNSIGNED_MSB_VAL);
            assert(cf ? acc >= RADIX : acc < RADIX);
            cf= NZEROP(cf);
            assert(cf == 0 || cf == ~0);
            assert(cf == ~0 ? acc >= RADIX : acc < RADIX);
            acc-= cf & RADIX;
            assert(acc < RADIX);
            number[i]= (char)(unsigned char)acc;
            assert(number[i] == acc);
            acc= cf & 1;
         }
         if (acc) {
            assert(acc == 1);
            if (length >= reserved) {
               size_t nres;
               void *nbuf;
               assert(length == reserved);
               if (!(nbuf= realloc(number, nres= reserved << 1))) {
                  goto mem_err;
               }
               number= nbuf;
               reserved= (unsigned)nres;
               assert((size_t)reserved == nres);
            }
            assert(length < reserved);
            number[length++]= (char)(unsigned char)acc;
            assert(number[length - 1] == acc);
         }
      }
   }
   {
      unsigned i;
      for (i= length; i--; ) {
         assert(number[i] >= 0);
         assert(number[i] < RADIX);
         if (putchar(alphabet[number[i]]) == EOF) goto wr_err;
      }
      if (putchar('\n') == EOF) goto wr_err;
   }
   if (ferror(stdin) || !feof(stdin)) {
      die("Error reading from standard input!");
   }
   if (fflush(stdout)) wr_err: die("Error writing to standard output!");
   return EXIT_SUCCESS;
}
