/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouaz <aelbouaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:27:08 by gekko             #+#    #+#             */
/*   Updated: 2026/10/28 16:51:24 by aelbouaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Routing.hpp"

const LocationConfig* findLocation(const ServerConfig &SrvrConf, std::string Path)
{
	const LocationConfig *best = nullptr;

	for (long unsigned i = 0; i < SrvrConf.locations.size(); i++)
	{
		const LocationConfig &temp = SrvrConf.locations[i];
		// line underneath checks if PATH starts with temp.path
		// for example:
		//	Path = /uploads/folder1/sup/cat.png
		// temp.path = /uploads/folder1/
		// so up to temp.path.size(), Path does match temp.path, thus Location does exist
		if (Path.compare(0, temp.path.size(), temp.path) == 0)
		{
			if (best == nullptr || temp.path.size() > best->path.size())
				best = &temp;
		}
	}
	return best;
}

int	methodIsImplemented(std::string method)
{
	if (method != "GET" && method != "DELETE" && method != "POST")
		return 0;
	return 1;
}

int	methodIsAllowed(const LocationConfig *Loc, std::string method)
{
	for (std::size_t i = 0; i < Loc->allowed_methods.size(); i++)
	{
		if (method == Loc->allowed_methods[i])
			return 1;
	}
	return 0;
}

Response HandleRequest(const Request &req, const ServerConfig &SrvrConf)
{
	const LocationConfig* Loc = findLocation(SrvrConf, req.path);
	if (!Loc)
		return makeError(404); // Path doesn't exist thus 404
	if (!methodIsImplemented(req.method))
		return makeError(501); // Method Not Implemented
	if (!methodIsAllowed(Loc, req.method))
		return makeError(405); // Method Not Allowed
	// STILL NEED TO CHECK FOR THE Body Size with an actual Big Payload
	if (SrvrConf.client_max_body < req.body.size())
		return makeError(413); // Max Body Exceeded
	return makeError(200); // SUCCESS
}
