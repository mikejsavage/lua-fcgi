CFLAGS = -O2 -fPIC -Wall
LDFLAGS = -shared -lfcgi

ifdef LUA_INCDIR
	CFLAGS += -I$(LUA_INCDIR)
endif

ifdef LUA_LIBDIR
	LIBS += -L$(LUA_LIBDIR)
endif

all: fcgi.so

fcgi.so: src/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f src/main.o fcgi.so
