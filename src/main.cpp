// MY PERSONAL TEST. Not part of the project build.
#include <iostream>
#include "Routing.hpp"
#include "WebServSocket.hpp"

// static void run(const ServerConfig &server, const std::string &method, const std::string &path)
// {
// 	Request req;
// 	req.method = method;
// 	req.path = path;
// 	req.version = "HTTP/1.1";

// 	Response res = HandleRequest(req, server);

// 	std::cout << "----Method: " << method << " | Path: " << path << " ----\n";
// 	if (res.status < 302)
// 		std::cout <<BB<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";
// 	else
// 		std::cout <<BR<< "### "<< res.status << " " << res.reason <<" ###"<<RESET<< "\n\n";

// 	// Next lines are just for Redirection
// 	auto it = res.headers.find("Location");
// 	if (it != res.headers.end())
// 		std::cout << "Location: " << it->second << "\n";
// }

int main()
{
	ServerConfig srv = makeTestConfig();

	WebServSocket Server("9034");
	Server.set_listening_socket();

	puts("pollserver: waiting for connections...");
	Server.server_poll();
	return 0;
}
