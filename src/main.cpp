/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/11/10 16:26:48 by jukerste      #+#    #+#                 */
/*   Updated: 2026/11/10 16:27:55 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Routing.hpp"
#include "WebServSocket.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "Parser.hpp"


int main()
{
	WebServSocket Server("9034");
	Server.set_listening_socket();

	puts("pollserver: waiting for connections...");
	Server.server_poll();
	return 0;
}
