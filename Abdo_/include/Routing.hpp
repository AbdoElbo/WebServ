/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Routing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouaz <aelbouaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:28:09 by gekko             #+#    #+#             */
/*   Updated: 2026/10/28 16:25:22 by aelbouaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include "Config.hpp"

#define NR "\033[0;31m"
#define NG "\033[0;32m"
#define NY "\033[0;33m"
#define NB "\033[0;34m"
#define NP "\033[0;35m"
#define BR "\033[1;31m"
#define BG "\033[1;32m"
#define BY "\033[1;33m"
#define BB "\033[1;34m"
#define BP "\033[1;35m"
#define GW "\033[0;39m"
#define RESET "\033[0m"

// THIS IS A PLACEHOLDER FOR THE REQUEST STRUCT
struct Request
{
    std::string method;                           // "POST"
    std::string path;                             // "/uploads/cat.png"
    std::string query;                            // "size=large"
    std::string version;                          // "HTTP/1.1"
    std::map<std::string, std::string> headers;   // {"Host": "localhost:8080", ...}
    std::string body;                             // the raw bytes after the headers
};


struct Response
{
	int			status;
	std::string	reason;
	std::map<std::string, std::string> headers;
	std::string	body;
};

std::string	reasonPhrase(int code);
std::string	buildErrorPage(int code, const std::string& reason);
Response	makeError(int code);
Response	HandleRequest(const Request &req, const ServerConfig &SrvrConf);
