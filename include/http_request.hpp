#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

enum http_method{
	GET,
	POST,
} ;


enum

class HttpRequest
{
	public:
		HttpRequest();
		HttpRequest(const HttpRequest& other);
		HttpRequest& operator=(const HttpRequest& other);
		~HttpRequest();

	private:
		http_method method; // e.g. get or post
		std::string target; // target hostname + route or souce.
		std::string protocol; // http 1.0 or http 1.1 or http 2.

		//only needed for post or put request also known as representation header.
		std::string content_type; // 
		std::string content_length;
		std::string body; // large body of data

		bool complete; // is the request complete
};

#endif // HTTP_REQUEST_HPP