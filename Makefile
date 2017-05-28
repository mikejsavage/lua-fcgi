SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

TARGET = fcgi.so

LIBS = -lfcgi
CFLAGS = -std=c99 -O2 -shared -fPIC -D_GNU_SOURCE -Wall -Wextra -Wno-nonnull -Wno-unused-parameter -Wwrite-strings -Wformat=2 -DNDEBUG

ifdef LUA_INCDIR
	CFLAGS += -I$(LUA_INCDIR)
endif

ifdef LUA_LIBDIR
	CFLAGS += -L$(LUA_LIBDIR)
endif

.PHONY: debug test clean

all: $(TARGET)

debug: CFLAGS += -ggdb -UNDEBUG
debug: all

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET)
