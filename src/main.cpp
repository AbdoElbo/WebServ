/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ast <jvan-ast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 15:49:00 by jukerste          #+#    #+#             */
/*   Updated: 2026/11/02 17:56:23 by jvan-ast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "WebServSocket.hpp"

void run_test( const std::string& raw_request)
{
        HttpRequest req = HttpRequestParser::parse(raw_request);
        
        // std::cout << "--- HEADERS (" << req.headers.size() << ") ---\n";
        // for (std::map<std::string, std::string>::const_iterator it = req.headers.begin(); it != req.headers.end(); ++it) {
        //     std::cout << "  [" << it->first << "] => \"" << it->second << "\"\n";
        // }

        // std::cout << "--- QUERY PARAMETERS (" << req.query_parameters.size() << ") ---\n";
        // for (std::map<std::string, std::string>::const_iterator it = req.query_parameters.begin(); it != req.query_parameters.end(); ++it) {
        //     std::cout << "  [" << it->first << "] => \"" << it->second << "\"\n";
        // }

        std::cout << "--- BODY ---\n";
        // std::cout << (req.body.empty() ? "(empty)" : req.body) << "\n";
    std::cout << "========================================\n\n" << std::endl;
}

int main()
{
    std::string valid_request = 
        "POST /images/upload.php?size=large&format=jpg HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0 (Codam Student)\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 29\r\n"
        "\r\n"
        "filename=cat.jpg&action=save";

    run_test(valid_request);


	WebServSocket Server("9034");
	Server.set_listening_socket();

	puts("pollserver: waiting for connections...");

    Server.server_poll();
    // Test 1: A fully valid HTTP/1.1 request with query parameters and body

    return 0;
}
