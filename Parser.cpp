/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parser.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/16 15:10:14 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/16 17:23:08 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

std::vector<ServerConfig> parse_server(const std::vector<std::string> &tokens, size_t &i)
{
	ServerConfig server;
	
	if (i >= tokens.size() || tokens[i] != "{")
		throw std::runtime_error("Error: expected '{' after keyword server");
	i++;

	while (i < tokens.size() || tokens[i] != "}")
	{
		const std::string &token = tokens[i];
		if (token == "listen")
		{
		}
	}
}
