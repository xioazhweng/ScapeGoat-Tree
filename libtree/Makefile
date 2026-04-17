WARNINGS = off

ifeq ($(WARNINGS), on)
	CFLAGS += -Wall -Wextra -Werror -Wconversion
endif
all: libtree.so

libtree.so: *.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^
clean:
	rm -f libtree.so
