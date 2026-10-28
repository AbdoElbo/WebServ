/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Tokenizer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/13 16:26:58 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/13 17:40:24 by jukerste      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::vector<std::string> tokenize(const std::filesystem::path& file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open())
		throw::std::runtime_error("Error: Could not open config file: " + file_path.string());
	
	// read the entire file into a string stream
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();

	std::vector<std::string> tokens;
	std::string current_token;
	bool in_comment = false;

	for (size_t i = 0; i < content.length(); ++i) 
	{
        char c = content[i];
        if (in_comment) 
		{
            if (c == '\n') 
                in_comment = false;
            continue ;
        }
		if (c == '#')
		{
			if (!current_token.empty())
			{
				tokens.push_back(current_token);
				current_token.clear();
			}
			in_comment = true;
			continue ;
		}
		if (c == '{' || c == '}' || c == ';')
		{
			if (!current_token.empty())
			{
				tokens.push_back(current_token);
				current_token.clear();
			}
			tokens.push_back(std::string(1, c));
			continue ;
		}
		if (std::isspace(static_cast<unsigned char>(c)))
		{
			if (!current_token.empty())
			{
				tokens.push_back(current_token);
				current_token.clear();
			}
			continue ;
		}
		current_token = current_token + c;
	}
	
	// catch any trailing tokens at the end of the file
	if (!current_token.empty())
		tokens.push_back(current_token);
	return (tokens);
}
