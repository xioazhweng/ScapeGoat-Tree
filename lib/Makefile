WARNINGS = off

ifeq ($(WARNINGS), on)
	CFLAGS += -Wall -Wextra -Werror -Wconversion
endif


CFLAGS += -lreadline

.PHONY: all clean

all: libinput.so

%.so: %.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

clean:
	rm -f libinput.so
