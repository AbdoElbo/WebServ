/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/11/10 16:26:48 by jukerste      #+#    #+#                 */
/*   Updated: 2026/11/10 17:28:42 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Routing.hpp"
#include "WebServSocket.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "Parser.hpp"


int main(int argc, char **argv)
{
	std::string config_path;
	if (argc > 1)
		config_path = argv[1];
	else
		config_path = "webserv.config";
	
	try
	{
		std::vector<std::string> tokens = tokenize(config_path);
		std::vector<ServerConfig> servers = parse_config(tokens);
		std::cout << "Successfully parsed " << servers.size() << " server block(s) from " << config_path << "!\n";
		
		std::string port_str = std::to_string(servers[0].port);
		WebServSocket Server(port_str.data());
		Server.set_listening_socket();
		std::cout << "Initializing server on port: " << port_str << "\n";
		std::puts("pollserver: waiting for connections...");
		Server.server_poll();
	}
	
	catch(const std::exception &e)
	{
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
