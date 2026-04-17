LIBTABLE_DIR = ./libtree
LIB_DIR = ./lib
SUBDIRS = $(LIBTABLE_DIR) $(LIB_DIR)
TARGET = lab
WARNINGS = off


CFLAGS += -linput -L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR)
CFLAGS += -lreadline -lcgraph -lgvc
CFLAGS += -ltree -L$(LIBTABLE_DIR) -Wl,-rpath=$(LIBTABLE_DIR)

RELEASE_CFLAGS = -O2
DEBUG_CFLAGS = -g -O0 -DDEBUG

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

SHARED_OBJS = $(LIBTABLE_DIR)/libtree.so $(LIB_DIR)/libinput.so

.PHONY: all clean debug graph_full $(SUBDIRS)

all: clean
all: CFLAGS += $(RELEASE_CFLAGS)
all: $(TARGET)

debug: CFLAGS += $(DEBUG_CFLAGS)
debug: export EXTRA_CFLAGS = $(DEBUG_CFLAGS)
debug: clean $(SHARED_OBJS) $(OBJS)
	$(CC) $(OBJS) $(MODE_CFLAGS) $(CFLAGS) -o $(TARGET)

graph_full: CFLAGS += -DGRAPH_FULL
graph_full: export EXTRA_CFLAGS = -DGRAPH_FULL
graph_full: clean $(SHARED_OBJS) $(OBJS)
	$(CC) $(OBJS) $(MODE_CFLAGS) -ltree $(CFLAGS) -o $(TARGET)

$(TARGET): $(SHARED_OBJS) $(OBJS)
	$(CC) $(OBJS) $(MODE_CFLAGS) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBTABLE_DIR)/libtree.so:
	$(MAKE) -C $(LIBTABLE_DIR) CFLAGS="$(EXTRA_CFLAGS)"

$(LIB_DIR)/libinput.so:
	$(MAKE) -C $(LIB_DIR) CFLAGS="$(EXTRA_CFLAGS)"


clean:
	rm -f $(TARGET) $(OBJS)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
