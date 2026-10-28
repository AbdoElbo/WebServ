/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parser.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/16 15:10:14 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/18 04:08:02 by jul           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

LocationConfig parse_location(const std::vector<std::string> &tokens, size_t &i)
{
	LocationConfig location;

	if (i >= tokens.size())
		throw std::runtime_error("Error: expected location path");
	
	location.path = tokens[i];
	i++;

	if (i >= tokens.size() || tokens[i] != "{")
		throw std::runtime_error("Error: expected '{' after location path");
	i++;

	while (i < tokens.size() && tokens[i] != "}")
	{
		const std::string &token = tokens[i];
		
		if (token == "root")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != ";")
				throw std::runtime_error("Error: expected ';' after keyword root");
			location.root = tokens[i + 1];
			i = i + 3;
		}
		else if (token == "allowed_methods")
		{
			i++;
			while (i < tokens.size() && tokens[i] != ";")
			{
				location.allowed_methods.push_back(tokens[i]);
				i++;
			}
			if (i >= tokens.size() || tokens[i] != ";")
				throw std::runtime_error("Error: expected ';' after allowed_methods");
			i++;
		}
		else if (token == "autoindex")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != ";")
				throw std::runtime_error("Error: expected ';' after autoindex");
			
			std::string value = tokens[i + 1];
			if (value == "on")
				location.autoIndex = true;
			else if (value == "off")
				location.autoIndex = false;
			else
				throw std::runtime_error("Error: autoindex must be 'on' or 'off' ");
			i = i + 3;
		}
		else
			throw std::runtime_error("Error: unknown directive in location block: " + token);
	}
	if (i >= tokens.size() || tokens[i] != "}")
		throw std::runtime_error("Error: expected closing '}' for location block");
	i++;
	
	return (location);
}

ServerConfig parse_server(const std::vector<std::string> &tokens, size_t &i)
{
	ServerConfig server;
	
	if (i >= tokens.size() || tokens[i] != "{")
		throw std::runtime_error("Error: expected '{' after keyword 'server'");
	i++;

	while (i < tokens.size() && tokens[i] != "}")
	{
		const std::string &token = tokens[i];
		
		if (token == "listen")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != ";")
				throw std::runtime_error("Error: expected ';' after listen");
			server.port = std::stoi(tokens[i + 1]);
			i = i + 3;
		}
		else if (token == "host")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != ";")
				throw std::runtime_error("Error: Expected ';' after keyword host");
			server.host_ip = tokens[i + 1];
			i = i + 3;
		}
		else if (token == "server_names")
		{
			i++;
			while (i < tokens.size() && tokens[i] != ";")
			{
				server.server_names.push_back(tokens[i]);
				i++;
			}
			if (i >= tokens.size() || tokens[i] != ";")
				throw std::runtime_error("Error: expected ';' after server_names");
			i++;
		}
		else if (token == "root")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != ";")
				throw std::runtime_error("Error: expected ';' after keyword root");
			server.root = tokens[i + 1];
			i = i + 3;
		}
		else if (token == "index")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != ";")
				throw std::runtime_error("Error: expected ';' after keyword index");
			server.index_file = tokens[i + 1];
			i = i + 3;
		}
		else if (token == "client_max_body_size")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != ";")
				throw std::runtime_error("Error: expected ';' after keyword client_max_body_size");
			server.client_max_body = std::stoul(tokens[i + 1]);
			i = i + 3;
		}
		else if (token == "location")
		{
			i++;
			server.locations.push_back(parse_location(tokens, i));
		}
		else
			throw std::runtime_error("Error: unknown directive or unexpected token: " + token);
	}
	if (i >= tokens.size() || tokens[i] != "}")
	{
		throw std::runtime_error("Error: expected a closing '}' for server block");		
	}
	i++;
	return (server);
}

std::vector<ServerConfig> parse_config(const std::vector<std::string> &tokens)
{
	std::vector<ServerConfig> servers;
	size_t i = 0;

	while (i < tokens.size())
	{
		if (tokens[i] == "server")
		{
			i++;
			servers.push_back(parse_server(tokens, i));
		}
		else
			throw std::runtime_error("Error: unexpected token: " + tokens[i]);
	}
	return (servers);
}
