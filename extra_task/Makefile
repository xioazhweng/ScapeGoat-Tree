LIBTABLE_DIR = ../libtree
LIB_DIR = ../lib
SUBDIRS = $(LIBTABLE_DIR) $(LIB_DIR)
TARGET = extra
WARNINGS = off

CFLAGS += -linput -L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR)
CFLAGS += -lreadline -lcgraph -lgvc
TYPE = .a

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

MODE_CFLAGS = -L$(LIBTABLE_DIR) -Wl,-rpath=$(LIBTABLE_DIR)

.PHONY: all clean $(SUBDIRS)

ifeq ($(WARNINGS), on)
	CFLAGS += -Wall -Wextra -Werror -Wconversion
endif

all: $(TARGET)

SHARED_OBJS = $(LIBTABLE_DIR)/libtree.so
SHARED_OBJS += $(LIB_DIR)/libinput.so

$(TARGET):  $(SHARED_OBJS) $(OBJS)
	$(CC) $(OBJS) $(MODE_CFLAGS) -ltree $(CFLAGS) -o $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBTABLE_DIR)/libtree.so: $(LIBTABLE_DIR)/*.c
	$(MAKE) -C $(LIBTABLE_DIR)

$(LIB_DIR)/libinput.so: $(LIB_DIR)/*.c
	$(MAKE) -C $(LIB_DIR)

clean :
	rm $(TARGET) $(OBJS)
