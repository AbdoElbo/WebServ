#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP
#include "http_request.hpp"


class HttpResponse
{
	public:
		HttpResponse(HttpRequest request);
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);
		~HttpResponse();

	private:
		http_method method; // e.g. get or post
		std::string target; // target hostname + route or souce.
		std::string protocol; // http 1.0 or http 1.1 or http 2.

		std::string content_type; // encoding for body probably for us only html
		std::string content_length; 
		std::string body; //if the message has a body, they describe the form of the message data and any encoding applied
};

#endif // HTTP_RESPONSE_HPP