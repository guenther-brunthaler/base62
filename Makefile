TARGETS = base62
EXECUTABLE_SCRIPTS = pwgen-from-dev_random-as-base62

PREFIX = /usr/local
DESTDIR = /

.PHONY: all clean install uninstall

all: $(TARGETS)

clean:
	-rm $(TARGETS)

AUG_CPPFLAGS = $(CPPFLAGS) -I .
AUG_CFLAGS = $(AUG_CPPFLAGS) $(CFLAGS)

.c:
	$(CC) $(AUG_CFLAGS) $(LDFLAGS) -o $@ $<

install: all
	mkdir -p -- "$(DESTDIR)$(PREFIX)"/bin
	cp -p -- $(TARGETS) $(EXECUTABLE_SCRIPTS) "$(DESTDIR)$(PREFIX)/bin"/
	
uninstall:
	cd -- "$(DESTDIR)$(PREFIX)/bin" \
		&& rm -- $(TARGETS) $(EXECUTABLE_SCRIPTS)
