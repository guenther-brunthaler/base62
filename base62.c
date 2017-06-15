/*
 * Version 2017.166
 *
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

#ifndef DATA_DEPENDENT_BRANCHES
   /* User configuration option:
    *
    * Add "-D DATA_DEPENDENT_BRANCHES=1" to your CFLAGS when building this
    * program if you want a faster but less secure version.
    *
    * On my machine, this makes the utility faster by about 23 % (when built
    * with -O3). YMMV.
    *
    * Unfortunately, this feature also enables data-dependent code paths,
    * which means an attacker measuring the run time or power consumption of
    * the program may deduce some information about the bit pattern of your
    * secret binary key from which you want to derive an alphanumeric
    * password.
    *
    * Therefore, this feature is disabled by default. Also, the difference
    * will barely be noticable unless your key is dozens of thousands of bits
    * long. */
   #define DATA_DEPENDENT_BRANCHES 0
#endif

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

/* Bit position of MSB for type "unsigned int". */
#define UNSIGNED_MSB_POS (sizeof(unsigned) * CHAR_BIT - 1)

/* Bit mask corresponding to UNSIGNED_MSB_POS. */
#define UNSIGNED_MSB_VAL (1u << UNSIGNED_MSB_POS)

/* return (int)x < 0 ? UINT_MAX : 0; */
#define NEG_MASK(x) (~((x) >> UNSIGNED_MSB_POS) + 1)

/* return x < y ? UINT_MAX : 0; */
#define LT_MASK(x, y) NEG_MASK(x - y)

/* return x != 0 ? UINT_MAX : 0; */
/* Warning: Evalualtes <x> multiple times! */
#define NZERO_MASK_MULTI_EVAL(x) (LT_MASK(x, 0) | LT_MASK(0, x))

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
         unsigned i, acc;
         #if DATA_DEPENDENT_BRANCHES
            acc= !!(byte & bit);
         #else
            acc= byte & bit;
            acc= NZERO_MASK_MULTI_EVAL(acc) & 1;
         #endif
         for (i= 0; i < length; ++i) {
            #if DATA_DEPENDENT_BRANCHES
               if ((acc+= number[i] << 1) >= RADIX) {
                  acc-= RADIX;
                  assert(acc < RADIX);
                  number[i]= (char)(unsigned char)acc;
                  assert(number[i] == acc);
                  acc= 1;
               } else {
                  assert(acc < RADIX);
                  number[i]= (char)(unsigned char)acc;
                  assert(number[i] == acc);
                  acc= 0;
               }
            #else
               unsigned c=
                  RADIX - 1 - (acc+= number[i] << 1) & UNSIGNED_MSB_VAL
               ;
               acc-= (c= NZERO_MASK_MULTI_EVAL(c)) & RADIX;
               assert(acc < RADIX);
               number[i]= (char)(unsigned char)acc;
               assert(number[i] == acc);
               acc= c & 1;
            #endif
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
