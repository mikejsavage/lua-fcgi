A minimal [FastCGI](http://www.fastcgi.com/) library.


Requirements
------------

lua >= 5.1, libfcgi


Usage
-----

`example.lua`

	local fcgi = require( "fcgi" )
	
	while fcgi.accept() do
		fcgi.print( "Content-Type: text/plain; charset=utf-8\r\n\r\n" )
		fcgi.print( "uri\t" .. fcgi.getenv( "DOCUMENT_URI" ) .. "\n" )
		fcgi.print( "agent\t" .. fcgi.getenv( "HTTP_USER_AGENT" ) .. "\n" )
		fcgi.print( "query\t" .. fcgi.getenv( "QUERY_STRING" ) .. "\n" )
		fcgi.print( "post\t" .. fcgi.post() .. "\n" )
		fcgi.print( "env\n" )
		fcgi.dumpenv()
	end

`nginx.conf`

	location / {
		include fastcgi_params;
		fastcgi_pass 127.0.0.1:9000;
	}

Then

	$ spawn-fcgi example.lua -p 9000
	$ curl localhost/path?get -d post
	uri	/path
	agent	curl/7.34.0
	query	get
	post	post
	env
	FCGI_ROLE=RESPONDER
	QUERY_STRING=get
	...
