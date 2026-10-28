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

	std::cout << "----Method: " << method << " | Path: " << path << " ----\n";
	if (res.status == 200)
		std::cout <<BB<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";
	else
		std::cout <<BR<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";
}

static void runMaxBody(const ServerConfig &server, const std::string &method, const std::string &path, bool Bigger)
{
	Request req;
	req.method = method;
	req.path = path;
	req.version = "HTTP/1.1";
	if (Bigger)
		req.body.append(2048576, 's');

	Response res = HandleRequest(req, server);

	std::cout << "----Method: " << method << " | Path: " << path << " ----\n";
	if (res.status == 200)
		std::cout <<BB<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";
	else
		std::cout <<BR<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";
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

	std::cout <<BG<< "--------------< Testing path Existance >--------------" <<RESET<< std::endl;
	run(server, "GET", "/"); // exists
	run(server, "GET", "spu/HelloWorld.html"); // doesn't exist

		std::cout <<BG<< "--------------< Testing None Implemented Methods >--------------" <<RESET<< std::endl;
	run(server, "GET", "/"); // Get works
	run(server, "PATCH", "/"); // Patch shouldn't Work

	std::cout <<BG<< "--------------< Testing Allowed Methods >--------------" <<RESET<< std::endl;
	run(server, "GET", "/"); // Get works
	run(server, "DELETE", "/about.html"); // Delete shouldn't Work

	std::cout <<BG<< "--------------< Testing Big Payloads >--------------" <<RESET<< std::endl;
	runMaxBody(server, "GET", "/", false); // Get works
	runMaxBody(server, "GET", "/", true); // Delete shouldn't Work

	return 0;
}


// g++ -std=c++17 -Wall -Wextra MyTest.cpp Router.cpp ErrorPage.cpp -o my_test
