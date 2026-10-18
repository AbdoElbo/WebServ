/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/13 17:17:26 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/18 04:19:07 by jul           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Parser.hpp"

int main() 
{
    std::vector<std::string> tokens;

    // Step 1: Tokenize
    try 
    {
        tokens = tokenize("webserv.config");

        std::cout << "SUCCESS! Total tokens found: " << tokens.size() << "\n\n";
        std::cout << "Printing tokens:\n";
        for (size_t i = 0; i < tokens.size(); ++i) 
        {
            std::cout << "[" << i << "] -> " << tokens[i] << "\n";
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Tokenizer Error: " << e.what() << "\n";
        return 1;
    }

    // Step 2: Parse
    try 
    {
        std::vector<ServerConfig> servers = parse_config(tokens);
        std::cout << "\nSUCCESS! Parsed " << servers.size() << " server block(s).\n\n";

        for (size_t s = 0; s < servers.size(); ++s) {
            std::cout << "Server #" << s + 1 << ":\n";
            std::cout << "  - Port: " << servers[s].port << "\n";
            std::cout << "  - Host: " << servers[s].host_ip << "\n";
            
            std::cout << "  - Server Names: ";
            for (const auto& name : servers[s].server_names)
                std::cout << name << " ";
            std::cout << "\n";

            if (servers[s].root)
                std::cout << "  - Root: " << servers[s].root->string() << "\n";
            if (servers[s].index_file)
                std::cout << "  - Index: " << *servers[s].index_file << "\n";
                
            std::cout << "  - Max Body Size: " << servers[s].client_max_body << "\n";

            std::cout << "  - Locations (" << servers[s].locations.size() << " found):\n";
            for (const auto& loc : servers[s].locations) {
                std::cout << "    * Path: " << loc.path << "\n";
                if (loc.root)
                    std::cout << "      Root: " << loc.root->string() << "\n";
                std::cout << "      Autoindex: " << (loc.autoIndex ? "on" : "off") << "\n";
                std::cout << "      Methods: ";
                for (const auto& m : loc.allowed_methods)
                    std::cout << m << " ";
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Parser Exception caught: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
