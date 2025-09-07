#ifndef RBX_HTTP_HTTP_TYPES_H_
#define RBX_HTTP_HTTP_TYPES_H_

#include <string>
#include <vector>

namespace rbx { namespace http {

	// HTTP request method.
	enum http_method
	{
		HTTP_GET,
		HTTP_POST,
		HTTP_PUT,
		HTTP_DELETE,
	};

	// Http request headers.
	struct http_header
	{
		std::string name;
		std::string value;
	};

	// Http request.
	struct http_request
	{
		http_method				 method;
		std::string				 url;
		std::string				 body;
		std::vector<http_header> headers;
	};

	// Http response.
	struct http_response
	{
		long		status_code;
		std::string body;
	};

}}	// namespace rbx::http

#endif	// RBX_HTTP_HTTP_TYPES_H_