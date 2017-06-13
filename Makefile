TARGETS = base62

.PHONY: all clean

all: $(TARGETS)

clean:
	-rm $(TARGETS)

AUG_CPPFLAGS = $(CPPFLAGS) -I .
AUG_CFLAGS = $(AUG_CPPFLAGS) $(CFLAGS)

.c:
	$(CC) $(AUG_CFLAGS) $(LDFLAGS) -o $@ $<
