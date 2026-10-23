/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouaz <aelbouaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:27:08 by gekko             #+#    #+#             */
/*   Updated: 2026/10/23 17:55:47 by aelbouaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Routing.hpp"

std::string reasonPhrase(int code)
{
	switch (code)
	{
		//NORMAL..
		case 200: return "OK";
		case 201: return "Created";
		case 204: return "No Content";
		case 301: return "Moved Permanently";
		// ERRORS..
		case 400: return "Bad Request";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 413: return "Payload Too Large";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 504: return "Gateway Timeout";
		default:  return "Error";
	}
}

std::string buildErrorPage(int code, const std::string& reason)
{
	return "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\">"
		"<title>" + std::to_string(code) + " " + reason + "</title>"
		"<style>body{font-family:sans-serif;text-align:center;margin-top:15vh;color:#333}"
		"h1{font-size:4rem;margin:0}</style></head>"
		"<body><h1>" + std::to_string(code) + "</h1><h2>" + reason + "</h2></body></html>";
}

Response makeError(int code)
{
	Response res;

	res.status = code;
	res.reason = reasonPhrase(code);
	res.headers["Content-Type"] = "text/html";
	res.body = buildErrorPage(code, res.reason);
	return res;
}
