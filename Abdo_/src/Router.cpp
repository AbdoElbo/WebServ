/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouaz <aelbouaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:27:08 by gekko             #+#    #+#             */
/*   Updated: 2026/10/23 17:53:21 by aelbouaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Routing.hpp"

Response HandleRequest(const Request &req, const ServerConfig &SrvrConf)
{
	(void)SrvrConf;
	Response res;

	res.status = 200;
	res.reason = reasonPhrase(200);
	res.headers["Content-Type"] = "text/plain";
	res.body = "You asked for " + req.method + " " + req.path;
	return res;
}
