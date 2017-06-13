base62
======

Abstract
--------

The base62 utility converts binary cryptographic key material of
arbitrary size into the shortest possible alphanumeric password
of equivalent strength.

The conversion is reversible (although this utility does not
implement it) and no entropy of the binary input will be lost.

The implementation is super-fast for usual key-sizes because it
avoids expensive operations like multiplication or division.


Example
-------

In order to generate a cryptographically strong 256 bit random
password under Linux/BSD:

	$ dd if=/dev/random bs=1 count=32 2> /dev/null | ./base62

where 32 is the byte size of the key (when assuming 8 bits per
byte, 32 bytes will be made of 256 bits).

For 128 bit passwords, use

	$ dd if=/dev/random bs=1 count=16 2> /dev/null | ./base62

or generally, for $n-bit passwords (with $n being a multiple of 8):

	$ n=512 # Generate a password that many bits strong.
	$ dd if=/dev/random bs=1 count=`expr $n / 8` 2> /dev/null | ./base62

Note that /dev/random generates high-quality passwords with full
entropy.

If the password quality is less important, it is also possible to
use /dev/urandom instead which has the advantage that it will not
block until enough entropy is available. However, as a
consequence of this, passwords created from /dev/urandom have
unknown and therefore possibly low entropy.

**Important**: Never use anything else than bs=1 when using "dd"
to read from a character device such as /dev/random!

Even though it sometimes works with a larger block sizes, there
are situations where less pseudorandom data is actually returned
than has been requested, making your keys weaker than expected.


Keywords
--------

base62, base-62, alphanumeric, password-generation,
password-generator, human-readable password, entropy-preserving.


How to build
------------

base62 is written in portable C and can be compiled with any
C-compiler (assuming it supports the C89 standard or better).

Under UNIX, Linux, Cygwin or MinGW it is simple: Just run

	$ make

to compile and build the program. If you want to build with
specific compiler flags, you can invoke 'make' like this instead:

	$ make CFLAGS="-D NDEBUG -O2 -s"

You might also want to to install the built executable, so that
it can be invoked from anywhere:

	$ sudo cp base62 /usr/local/bin/ \
	  && sudo chmod +x /usr/local/bin/base62


Differences from base64
-----------------------

First of all, a different alphabet is used, which consists of the
following characters (in that order) for the base-62 digits:

*  A through Z
*  a through z
*  0 through 9

Second, there is no byte padding using the "="-character. The
output is as short as possible, with unused bits in the last
digit set to '0' before encoding.

Third, long output lines are not broken into shorter lines.
base62 always outputs a single password on a line, no matter how
long the line will become.

base62 does not have a "-d" option for decoding base-62 encoded
data back to a byte stream. This is not normally needed, because
the passwords will be directly processed by the encryption
algorithms as they are.


Cryptographic security
---------------------

The utility does not try to mlock() its data while processing or
"burn" variables after use, because such functionality is not
provided by the C standard and the reliability of such features
is debatable even if it is available, such as in POSIX.

Make therefore sure that either no swapping is enabled, or that
your swap space is encrypted as well. Because otherwise the
memory image of the running process could be swapped out to disk
unprotected, where it could be found by an attacker later.

Of course, all this only matters if you are actually using base62
for converting confidential key material.


Caveats
-------

The algorithm used to convert the input data into a base-62
number requires running time which increases approximately
quadratically with the length of the generated output.

While this is quite fast for all symmetric or asymmetric key
sizes that make any sense, it does not scale well for keys of
absurdly large bit sizes.


License Information
-------------------

Copyright (c) 2017 Guenther Brunthaler. All rights reserved.

This program is free software.  
Distribution is permitted under the terms of the GPLv3.
