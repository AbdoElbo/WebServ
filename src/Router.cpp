/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gekko <gekko@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:27:08 by gekko             #+#    #+#             */
/*   Updated: 2026/10/30 18:07:13 by gekko            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Routing.hpp"

static bool locationMatches(const std::string &locPath, const std::string &urlPath)
{
	// 1. the URL must start with the location path
	if (urlPath.compare(0, locPath.size(), locPath) != 0)
		return false;

	// 2. then the match must end on a segment boundary
	if (urlPath.size() == locPath.size())
		return true;                       // exact match: /uploads == /uploads
	if (locPath.back() == '/')
		return true;                       // location is "/", so everything under it matches
	return urlPath[locPath.size()] == '/'; // next char must be '/', so /uploads/x yes, /uploadsss no
}

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
		if (locationMatches(temp.path, Path))
		{
			if (best == nullptr || temp.path.size() > best->path.size())
				best = &temp;
		}
	}
	return best;
}

Response makeRedirect(const std::string &target)
{
	Response res;
	res.status = 301;
	res.reason = reasonPhrase(301);
	res.headers["Location"] = target;
	return res;
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

int	CheckFileExistance(const LocationConfig* Loc, const Request &req)
{
	std::string FileName = req.path.substr(Loc->path.size() + 1);
	std::filesystem::path p = std::filesystem::path(Loc->path) / FileName;
	if (std::filesystem::exists(p))
	{
		std::cout <<BY<< "File " << FileName << " EXISTS in " << Loc->path <<RESET<< std::endl;
		return 1;
	}
	std::cout <<BY<< "File " << FileName << " does NOOTTTT exist in " << Loc->path <<RESET<< std::endl;
	return 0;
}

Response HandleRequest(const Request &req, const ServerConfig &SrvrConf)
{
	const LocationConfig* Loc = findLocation(SrvrConf, req.path);
	if (!Loc)
		return makeError(404); // Path doesn't exist thus 404
	if (Loc->redirect_url)
		return makeRedirect(*Loc->redirect_url);
	if (!methodIsImplemented(req.method))
		return makeError(501); // Method Not Implemented
	if (!methodIsAllowed(Loc, req.method))
		return makeError(405); // Method Not Allowed
	if (SrvrConf.client_max_body < req.body.size())
		return makeError(413); // Max Body Exceeded
	if (!CheckFileExistance(Loc, req))
		return makeError(404); // File doesn't exist
	return makeError(200); // SUCCESS
}
