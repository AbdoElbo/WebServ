/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequestParser.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/19 15:57:21 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/19 17:35:54 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpRequest.hpp"
#include <algorithm>
#include <stdexcept>
#include <sstream>

class HttpRequestParser
{
	public:
		static HttpRequest parse(const std::string &raw_data); // Main function to parse a raw HTTP request string
	private:
		static void parse_request_line(const std::string &line, HttpRequest &request);
		static void split_uri(HttpRequest &request);
		static void parse_header(const std::string &line, HttpRequest &request);
};
