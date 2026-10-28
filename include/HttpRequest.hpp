/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequest.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/19 15:07:49 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/27 15:07:35 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

struct HttpRequest
{
	std::string method; // for "GET", "POST", "DELETE"
	std::string uri; // for the full path. example: "/images/cat.jpg?size=large"
	std::string path; // for just the path part: "/images/cat.jpg"
	std::string query_str; // just for the query path part: "size=large"
	std::string http_version; // "HTTP/1.1"
	std::map<std::string, std::string> headers; // Headers mapped case-insensitively or standard strings ( headers["Host"] = "example.com")
	std::map<std::string, std::string> query_parameters;
	std::string body; // The payload data (for POST/PUT requests)
	bool is_complete = false; // helper states for parsing progress (handy for handling chunked or large bodies later)
};
