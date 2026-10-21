/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Routing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gekko <gekko@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:28:09 by gekko             #+#    #+#             */
/*   Updated: 2026/10/21 23:58:16 by gekko            ###   ########.fr       */
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
