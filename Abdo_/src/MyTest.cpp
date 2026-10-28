// MY PERSONAL TEST. Not part of the project build.
#include <iostream>
#include "../include/Routing.hpp"

void printHeader(const std::string& text)
{
	std::size_t totalWidth = 75;
	const std::string decorated = "< " + text + " >";

	if (decorated.size() >= totalWidth)
	{
		std::cout << decorated << '\n';
		return;
	}

	const std::size_t remaining = totalWidth - decorated.size();
	const std::size_t left = remaining / 2;
	const std::size_t right = remaining - left;  // gets the extra dash if odd

	std::cout <<BG<< std::string(left, '-') << decorated << std::string(right, '-') <<RESET<< '\n';
}

static void run(const ServerConfig &server, const std::string &method, const std::string &path)
{
	Request req;
	req.method = method;
	req.path = path;
	req.version = "HTTP/1.1";

	Response res = HandleRequest(req, server);

	std::cout << "----Method: " << method << " | Path: " << path << " ----\n";
	if (res.status < 302)
		std::cout <<BB<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";
	else
		std::cout <<BR<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";

	// Next lines are just for Redirection
	auto it = res.headers.find("Location");
	if (it != res.headers.end())
		std::cout << "Location: " << it->second << "\n";
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
	if (res.status < 302)
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

	LocationConfig imagesLoc;
	imagesLoc.path = "/images";
	imagesLoc.allowed_methods = {"GET", "POST"};

	LocationConfig redir;
	redir.path = "/Hellooo";
	redir.redirect_url = "/images";
	redir.allowed_methods = {"GET", "POST"};

	ServerConfig server;
	server.root = "./www";
	server.locations = {rootLoc, imagesLoc, uploadLoc, redir};

	printHeader("Testing path Existance");
	run(server, "GET", "/"); // exists
	run(server, "GET", "spu/HelloWorld.html"); // doesn't exist

	printHeader("Testing None Implemented Methods");
	run(server, "GET", "/"); // Get works
	run(server, "PATCH", "/"); // Patch shouldn't Work

	printHeader("Testing Allowed Methods");
	run(server, "GET", "/"); // Get works
	run(server, "DELETE", "/about.html"); // Delete shouldn't Work

	printHeader("Testing Big Payloads");
	runMaxBody(server, "GET", "/", false); // Get works
	runMaxBody(server, "GET", "/", true); // Delete shouldn't Work

	printHeader("REDIRECT");
	run(server, "GET", "/Hellooo");

	printHeader("");
	return 0;
}


// g++ -std=c++17 -Wall -Wextra MyTest.cpp Router.cpp ErrorPage.cpp -o my_test
