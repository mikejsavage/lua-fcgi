package = "fcgi"
version = "1.1-2"

source = {
	url = "git://github.com/mikejsavage/lua-fcgi.git",
	tag = "v1.1-2",
}

description = {
	summary = "A Lua module for writing FastCGI applications",
	homepage = "http://github.com/mikejsavage/lua-fcgi",
	license = "MIT",
	maintainer = "Mike Savage",
}

dependencies = {
	"lua >= 5.1",
}

build = {
	type = "make",

	install_pass = false,

	build_variables = {
		LUA_INCDIR = "$(LUA_INCDIR)",
		LUA_LIBDIR = "$(LUA_LIBDIR)",
	},

	install = {
		lib = {
			[ "fcgi" ] = "fcgi.so",
		},
	},
}
