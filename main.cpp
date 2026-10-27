/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/19 15:49:00 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/27 16:51:07 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"

void run_test(const std::string& test_name, const std::string& raw_request)
{
    std::cout << "========================================\n";
    std::cout << "TEST: " << test_name << "\n";
    std::cout << "========================================\n";
    
    try {
        HttpRequest req = HttpRequestParser::parse(raw_request);
        
        std::cout << "[SUCCESS] Request parsed and validated!\n";
        std::cout << "Method       : " << req.method << "\n";
        std::cout << "URI          : " << req.uri << "\n";
        std::cout << "Path         : " << req.path << "\n";
        std::cout << "Query String : " << req.query_str << "\n";
        std::cout << "HTTP Version : " << req.http_version << "\n";
        
        std::cout << "--- HEADERS (" << req.headers.size() << ") ---\n";
        for (std::map<std::string, std::string>::const_iterator it = req.headers.begin(); it != req.headers.end(); ++it) {
            std::cout << "  [" << it->first << "] => \"" << it->second << "\"\n";
        }

        std::cout << "--- QUERY PARAMETERS (" << req.query_parameters.size() << ") ---\n";
        for (std::map<std::string, std::string>::const_iterator it = req.query_parameters.begin(); it != req.query_parameters.end(); ++it) {
            std::cout << "  [" << it->first << "] => \"" << it->second << "\"\n";
        }

        std::cout << "--- BODY ---\n";
        std::cout << (req.body.empty() ? "(empty)" : req.body) << "\n";

    } 
    catch (const std::exception& e) 
    {
        std::cerr << "[CAUGHT ERROR]   : " << e.what() << "\n";
    }
    std::cout << "========================================\n\n";
}

int main()
{
    // Test 1: A fully valid HTTP/1.1 request with query parameters and body
    std::string valid_request = 
        "POST /images/upload.php?size=large&format=jpg HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0 (Codam Student)\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 29\r\n"
        "\r\n"
        "filename=cat.jpg&action=save";

    run_test("1. Valid POST Request", valid_request);

    // Test 2: An invalid request (intentionally missing the mandatory 'Host' header)
    std::string invalid_request = 
        "GET /index.html HTTP/1.1\r\n"
        "User-Agent: Mozilla/5.0 (Codam Student)\r\n"
        "\r\n";

    run_test("2. Invalid Request (Missing Host)", invalid_request);

    return 0;
}
