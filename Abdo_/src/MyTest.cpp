// MY PERSONAL TEST. Not part of the project build.
#include <iostream>
#include "../include/Routing.hpp"

static void run(const ServerConfig &server, const std::string &method, const std::string &path)
{
	Request req;
	req.method = method;
	req.path = path;
	req.version = "HTTP/1.1";

	Response res = HandleRequest(req, server);

	std::cout << "---- " << method << " " << path << " ----\n";
	std::cout << res.status << " " << res.reason << "\n";
	for (const auto &h : res.headers)
		std::cout << h.first << ": " << h.second << "\n";
	std::cout << "\n" << res.body << "\n\n";
}

int main()
{
	LocationConfig rootLoc;
	rootLoc.path = "/";
	rootLoc.allowed_methods = {"GET"};

	LocationConfig uploadLoc;
	uploadLoc.path = "/uploads";
	uploadLoc.allowed_methods = {"GET", "POST", "DELETE"};

	ServerConfig server;
	server.root = "./www";
	server.locations = {rootLoc, uploadLoc};

	run(server, "GET", "/about.html");
	run(server, "DELETE", "/uploads/cat.png");
	run(server, "DELETE", "/about.html");
	return 0;
}


// g++ -std=c++17 -Wall -Wextra MyTest.cpp Router.cpp ErrorPage.cpp -o my_test
