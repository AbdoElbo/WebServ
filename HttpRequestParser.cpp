/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequestParser.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/19 16:04:23 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/27 12:36:02 by jul           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

void HttpRequestParser::parse_request_line(const std::string &line, HttpRequest &request)
{
	std::stringstream source(line);
	if (!(source >> request.method >> request.uri >> request.http_version))
		throw std::runtime_error("Error: Malformed request http line: " + line);
}

void HttpRequestParser:: split_uri(HttpRequest &request)
{
	size_t qm_pos = request.uri.find('?');
	
	if (qm_pos != std::string::npos)
	{
		request.path = request.uri.substr(0, qm_pos);
		request.query_string = request.uri.substr(qm_pos + 1);
	}
	else
	{
		request.path = request.uri;
		request.query_string = "";
	}
}

void HttpRequestParser::parse_header(const std::string &line, HttpRequest &request)
{
	size_t colon_pos = line.find(':');

	if (colon_pos == std::string::npos)
		throw std::runtime_error("Error: Malformed header line: " + line);
	
	std::string keyword = line.substr(0, colon_pos);
	std::string value = line.substr(colon_pos + 1);

	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t\r") + 1);
	
	request.headers[keyword] = value;
}

HttpRequest HttpRequestParser::parse(const std::string &raw_data)
{
	HttpRequest request;
	
	if (raw_data.empty())
		return (request);

	size_t header_end = raw_data.find("\r\n\r\n");
	std::string header_section;
	std::string body_section;

	if (header_end != std::string::npos)
	{
		header_section = raw_data.substr(0, header_end);
		body_section = raw_data.substr(header_end + 4);
	}
	else
	{
		header_end = raw_data.find("\n\n");
		if (header_end != std::string::npos)
		{
			header_section = raw_data.substr(0, header_end);
			body_section = raw_data.substr(header_end + 2);
		}
		else
		{
			header_section = raw_data;	 // no body found, only headers/request line
		}
	}

	std::stringstream source(header_section);
	std::string line;
	bool is_first_line = true;

	while (std::getline(source, line))
	{
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
		if (line.empty())
			continue ;
		if (is_first_line)
		{
			parse_request_line(line, request);
			split_uri(request);
			is_first_line = false;
		}
		else
			parse_header(line, request);
	}
	request.body = body_section;
	request.is_complete = true;

	return (request);
}
