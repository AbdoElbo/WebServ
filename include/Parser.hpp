/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parser.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jukerste <jukerste@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/10/16 15:04:33 by jukerste      #+#    #+#                 */
/*   Updated: 2026/10/18 04:13:12 by jul           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include "Config.hpp"

std::vector<ServerConfig>	parse_config(const std::vector<std::string> &tokens);
