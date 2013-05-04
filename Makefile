
LIBGIT2_PATH ?= ~/coding/build/libgit2/lib
DEPS = $(shell PKG_CONFIG_PATH=$(LIBGIT2_PATH)/pkgconfig pkg-config --cflags --libs libgit2)

CC ?=gcc
CFLAGS += -Wall -Werror -O2 $(DEPS)

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o git $(OBJECTS)

clean:
	rm -f $(OBJECTS) git
