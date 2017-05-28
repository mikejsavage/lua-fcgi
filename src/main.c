/*
 * Copyright (c) 2015, Michael Savage <mike@mikejsavage.co.uk>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <fcgi_stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) ) 
#endif

extern char ** environ;

static int luafcgi_accept( lua_State * L ) {
	int rv = FCGI_Accept();

	lua_pushboolean( L, rv == 0 );

	return 1;
}

static int luafcgi_getenv( lua_State * L ) {
	const char * key = luaL_checkstring( L, 1 );
	char * value = getenv( key );

	lua_pushstring( L, value );

	return 1;
}

static int luafcgi_print( lua_State * L ) {
	size_t len;
	const char * str = lua_tolstring( L, 1, &len );

	size_t rv = fwrite( ( char * ) str, 1, len, stdout );

	if( rv < len ) {
		lua_pushstring( L, strerror( errno ) );
		return lua_error( L );
	}

	return 0;
}

static int luafcgi_dumpenv( lua_State * L ) {
	for( char ** var = environ; *var != NULL; var++ ) {
		printf( "%s\n", *var );
	}

	return 0;
}

static int luafcgi_post( lua_State * L ) {
	char * content_length = getenv( "CONTENT_LENGTH" );
	unsigned long len = strtoul( content_length, NULL, 10 );
	
	if( len == ULONG_MAX && errno == ERANGE ) {
		lua_pushstring( L, strerror( errno ) );
		return lua_error( L );
	}

	if( len == 0 ) {
		lua_pushliteral( L, "" );
	}
	else {
		char * post_data = malloc( len );

		if( post_data == NULL ) {
			lua_pushliteral( L, "malloc failed" );
			return lua_error( L );
		}

		size_t bytes = fread( post_data, 1, len, stdin );

		if( bytes < len ) {
			free( post_data );
			lua_pushstring( L, strerror( ferror( stdin ) ) );
			return lua_error( L );
		}

		lua_pushlstring( L, post_data, len );

		free( post_data );
	}

	return 1;
}

static const struct luaL_Reg luafcgi_lib[] = {
	{ "accept", luafcgi_accept },
	{ "getenv", luafcgi_getenv },
	{ "dumpenv", luafcgi_dumpenv },
	{ "print", luafcgi_print },
	{ "post", luafcgi_post },
	{ NULL, NULL },
};

LUALIB_API int luaopen_fcgi( lua_State * L ) {
	luaL_newlib( L, luafcgi_lib );

	return 1;
}
