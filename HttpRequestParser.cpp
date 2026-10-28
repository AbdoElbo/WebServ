/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequestParser.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/19 16:04:23 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/28 16:10:10 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

std::string HttpRequestParser::decode_chunked_body(const std::string &raw_body)
{
	std::string decoded = "";
	size_t pos = 0;

	while (pos < raw_body.length())
	{
		// 1. Find the end of the chunk size line
		size_t end_of_line = raw_body.find("\r\n", pos);
		if (end_of_line == std::string::npos)
			throw std::runtime_error("400 Bad Request: Malformed chunked body");
		
		std::string line_size = raw_body.substr(pos, end_of_line - pos);
		pos = end_of_line + 2; // skip /r/n

		// Handle optional chunk extensions (like "4;some-extension")
		size_t semicolon_pos = line_size.find(';');
		if (semicolon_pos != std::string::npos)
			line_size = line_size.substr(0, semicolon_pos);
		
		// 2. Convert hex size string to a numeric size_t
		size_t chunk_size = 0;
		std::stringstream source;
		source << std::hex << line_size;
		source >> chunk_size;

		if (source.fail())
			throw std::runtime_error("400 Bad Request: Invalid hex chunk size");
		// 3. If chunk size is 0, we've hit the final chunk terminator
		if (chunk_size == 0)
			break ;
		
		// 4. Extract the chunk data
		if (pos + chunk_size > raw_body.length())
			throw std::runtime_error("400 Bad Request: Chunk size exceeds data length");
		
		decoded.append(raw_body, pos, chunk_size);
		pos = pos + chunk_size;
		
		// 5. Skip the trailing \r\n after the chunk data
		if (pos + 2 < raw_body.length() && raw_body.substr(pos, 2) == "\r\n")
			pos = pos + 2;
		else
			throw std::runtime_error("400 Bad Request: Missing chunk trailing return and newline characters");
	}
	return (decoded);
}

void HttpRequestParser::parse_request_line(const std::string &line, HttpRequest &request)
{
	std::stringstream source(line);
	if (!(source >> request.method >> request.uri >> request.http_version))
		throw std::runtime_error("Error: Malformed request http line: " + line);
}

void HttpRequestParser::split_uri(HttpRequest &request)
{
	size_t qm_pos = request.uri.find('?');
	
	if (qm_pos != std::string::npos)
	{
		request.path = request.uri.substr(0, qm_pos);
		request.query_str = request.uri.substr(qm_pos + 1);
		parse_query_str(request.query_str, request);
	}
	else
	{
		request.path = request.uri;
		request.query_str = "";
	}
}

void HttpRequestParser::parse_header(const std::string &line, HttpRequest &request)
{
	size_t colon_pos = line.find(':');

	if (colon_pos == std::string::npos)
		throw std::runtime_error("400 Bad Request: Malformed header line: " + line);
	
	std::string keyword = line.substr(0, colon_pos);
	std::string value = line.substr(colon_pos + 1);

	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t\r") + 1);
	
	for (size_t i = 0; i < keyword.length(); ++i)
		keyword[i] = std::tolower(keyword[i]);
	
	request.headers[keyword] = value;
}

void HttpRequestParser::parse_query_str(const std::string &query_str, HttpRequest &request)
{
	if (query_str.empty())
		return ;

	std::stringstream input(query_str);
	std::string pair;

	while (std::getline(input, pair, '&'))
	{
		if (pair.empty())
			continue ;
		
		size_t es_pos = pair.find('=');
		std::string keyword;
		std::string value;
		
		if (es_pos != std::string::npos)
		{
			keyword = pair.substr(0, es_pos);
			value = pair.substr(es_pos + 1);
		}
		else
		{
			keyword = pair;
			value = "";
		}
		request.query_parameters[keyword] = value;
	}
}

void HttpRequestParser::validate_request(const HttpRequest &request)
{
	if (request.http_version != "HTTP/1.1" && request.http_version != "HTTP/1.0")
		throw std::runtime_error("505: HTTP version not supported: " + request.http_version);
	
	if (request.headers.find("host") == request.headers.end())
		throw std::runtime_error("400 Bad Request: Missing mandatory 'Host' header");
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
			header_section = raw_data; // no body found, only headers/request line
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
	if (request.headers.find("transfer-encoding") != request.headers.end() && request.headers["transfer-encoding"].find("chunked") != std::string::npos)
		request.body = decode_chunked_body(body_section);
	else
		request.body = body_section;
	
	validate_request(request);
	request.is_complete = true;
	return (request);
}
