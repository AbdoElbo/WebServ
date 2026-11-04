/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ast <jvan-ast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 15:07:49 by jukerste          #+#    #+#             */
/*   Updated: 2026/11/04 15:41:27 by jvan-ast         ###   ########.fr       */
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




