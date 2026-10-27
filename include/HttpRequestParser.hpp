/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequestParser.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/19 15:57:21 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/27 16:02:54 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpRequest.hpp"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <string>

class HttpRequestParser
{
	public:
		static HttpRequest parse(const std::string &raw_data); // Main function to parse a raw HTTP request string
	private:
		static void parse_request_line(const std::string &line, HttpRequest &request);
		static void split_uri(HttpRequest &request);
		static void parse_header(const std::string &line, HttpRequest &request);
		static void parse_query_str(const std::string &query_str, HttpRequest &request);
		static void validate_request(const HttpRequest &request);
};
