/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/13 17:17:26 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/13 17:38:27 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int main() 
{
    try 
	{
        std::vector<std::string> tokens = tokenize("webserv.config");

        std::cout << "SUCCESS! Total tokens found: " << tokens.size() << "\n\n";
        std::cout << "Printing tokens:\n";
        for (size_t i = 0; i < tokens.size(); ++i) 
		{
            std::cout << "[" << i << "] -> " << tokens[i] << "\n";
        }
	} 
	catch (const std::exception& e) 
	{
        std::cerr << e.what() << "\n";
    }
    return 0;
}