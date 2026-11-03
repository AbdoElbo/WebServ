/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ast <jvan-ast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 15:49:00 by jukerste          #+#    #+#             */
/*   Updated: 2026/11/03 16:58:18 by jvan-ast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServSocket.hpp"

int main()
{
	WebServSocket Server("9034");
	Server.set_listening_socket();

	puts("pollserver: waiting for connections...");

    Server.server_poll();
    // Test 1: A fully valid HTTP/1.1 request with query parameters and body

    return 0;
}
