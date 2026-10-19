/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/09 16:09:23 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/17 12:54:29 by jul           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <iostream>
#include <cctype>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <optional>

struct LocationConfig
{
	std::string path; // for URL route (like /images or /cgi-bin)
	std::optional<std::filesystem::path> root; //overrides global root directory if set
	std::vector<std::string> allowed_methods; // http allowed methods {"GET", "POST"}
	bool autoIndex = false; // true if directory listing is allowed
	std::optional<std::string> index_file; // specific index file for this route
	std::optional<std::string> redirect_url; // URL to redirect requests to
	std::map<std::string, std::string> cgi_handlers; // Maps extenion for interpreter (like .py -> /usr/bin/python3)
};

struct ServerConfig
{
	int port = 8080; // Port number the sever listens on
	std::string host_ip = "127.0.0.1";
	std::vector<std::string> server_names; // something.com
	std::optional<std::filesystem::path> root; // global root for server
	std::optional<std::string> index_file;
	size_t client_max_body = 1048576; // Default 1MB limit
	std::map<int, std::string> error_pages; // like error code 404 or path: /error.404.html
	std::vector<LocationConfig> locations;
};

std::vector<std::string> tokenize(const std::filesystem::path& file_path);