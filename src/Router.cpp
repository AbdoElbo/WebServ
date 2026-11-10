/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouaz <aelbouaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:27:08 by gekko             #+#    #+#             */
/*   Updated: 2026/11/10 14:08:27 by aelbouaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Routing.hpp"

static bool locationMatches(const std::string &locPath, const std::string &urlPath)
{
	// the URL must start with the location path
	if (urlPath.compare(0, locPath.size(), locPath) != 0)
		return false;

	// then the match must end on a segment boundary
	if (urlPath.size() == locPath.size())
		return true;   // exact match: /uploads == /uploads
	if (locPath.back() == '/')
		return true;  // location is "/", so everything under it matches
	return urlPath[locPath.size()] == '/'; // next char must be '/', so /uploads/x yes, /uploadsss no
}

static const LocationConfig* findLocation(const ServerConfig &SrvrConf, std::string Path)
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

static Response makeRedirect(const std::string &target)
{
	Response res;
	res.status = 301;
	res.reason = reasonPhrase(301);
	res.headers["Location"] = target;
	return res;
}

static int	methodIsImplemented(std::string method)
{
	if (method != "GET" && method != "DELETE" && method != "POST")
		return 0;
	return 1;
}

static int	methodIsAllowed(const LocationConfig *Loc, std::string method)
{
	for (std::size_t i = 0; i < Loc->allowed_methods.size(); i++)
	{
		if (method == Loc->allowed_methods[i])
			return 1;
	}
	return 0;
}

static std::string getMimeType(const std::filesystem::path &p)
{
	std::string ext = p.extension().string();
	if (ext == ".html" || ext == ".htm")	return "text/html";
	if (ext == ".css")	return "text/css";
	if (ext == ".js")	return "application/javascript";
	if (ext == ".png")	return "image/png";
	if (ext == ".jpg" || ext == ".jpeg")	return "image/jpeg";
	if (ext == ".gif")	return "image/gif";
	if (ext == ".txt")	return "text/plain";
	return "application/octet-stream";
}

Response returnRequestedPage(const std::filesystem::path &p)
{
	Response res;

	std::ifstream in(p, std::ios::binary);
	if (!in.is_open())
		return makeError(403);

	std::stringstream buffer;
	buffer << in.rdbuf();
	res.status = 200;
	res.reason = "OK";
	res.headers["Content-Type"] = getMimeType(p);
	res.body = buffer.str();
	return res;
}

static Response	CheckFileExistance(const ServerConfig &SrvrConf , const Request &req)
{
	std::string realpath = req.path;
	while (!realpath.empty() && realpath[0] == '/')
		realpath.erase(0, 1);
	std::filesystem::path p = std::filesystem::path(SrvrConf.root) / realpath;
	if (std::filesystem::exists(p))
	{
		if (std::filesystem::is_regular_file(p))
			return returnRequestedPage(p);
		else if (std::filesystem::is_directory(p))
			return makeError(403);
	}
	return makeError(404);
}

static bool	hasDotDot(const std::string &path)
{
	std::istringstream	in(path);
	std::string			segment;

	while (std::getline(in, segment, '/'))
	{
		if (segment == "..")
			return true;
	}
	return false;
}

Response HandleRequest(const Request &req, const ServerConfig &SrvrConf)
{
	if (hasDotDot(req.path))
		return makeError(403); // Sure thing Buddy
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
	return CheckFileExistance(SrvrConf, req);
}
