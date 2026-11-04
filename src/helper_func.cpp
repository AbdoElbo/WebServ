#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int sendall(int s, char *buf, int len) // TO DO: check if len implementation is correct
{
	int total = 0; // how many bytes we've sent
	int bytesleft = len; // how many we have left to send
	int n;
	while(total < len) {
 		n = send(s, buf+total, bytesleft, 0);
 		if (n == -1) { break; }
 		total += n;
 		bytesleft -= n;
	}

	len = total; // return number actually sent here

	return n==-1?-1:0; // return -1 on failure, 0 on success
}

const char *inet_ntop2(void *addr, char *buf, size_t size)
{
    struct sockaddr_storage *sas = (sockaddr_storage *)addr;
    struct sockaddr_in *sa4;
    struct sockaddr_in6 *sa6;
    void *src;

    switch (sas->ss_family) {
        case AF_INET:
            sa4 = (sockaddr_in *)addr;
            src = &(sa4->sin_addr);
            break;
        case AF_INET6:
            sa6 = (sockaddr_in6 *)addr;
            src = &(sa6->sin6_addr);
            break;
        default:
            return NULL;
    }

    return inet_ntop(sas->ss_family, src, buf, size);
}

void	write_pipe(int fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		throw std::runtime_error("test");
	}
	close(fd);
}

void	listen_pipe(int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		throw std::runtime_error("test");
	}
	close(fd);
}

int	pid_ok(int pid)
{
	if (pid == -1)
	{
		perror("Fork");
		exit(errno);
	}
	if (pid == 0)
		return (1);
	return (0);
}

std::string parse_cgi_response(int fd)
{
	std::string cgi_response;
	char buff[10000];
	int amnt_read;

	while ((amnt_read = read(fd, buff, 10000)) != 0)
	{
		cgi_response += buff;
	}

	if (cgi_response.empty()) // TODO: fix this function it currently send back garbage
		return (cgi_response);

	size_t header_end = cgi_response.find("\r\n\r\n");
	std::string header_section;
	std::string body_section;

	if (header_end != std::string::npos)
	{
		header_section = cgi_response.substr(0, header_end);
		body_section = cgi_response.substr(header_end + 4);
	}
	else
	{
		header_end = cgi_response.find("\n\n");
		if (header_end != std::string::npos)
		{
			header_section = cgi_response.substr(0, header_end);
			body_section = cgi_response.substr(header_end + 2);
		}
		else
		{
			header_section = cgi_response; // no body found, only headers/request line
		}
	}

	if (!body_section.empty())
	{
		const char *to_insert = "Content-Length: ";
		cgi_response.insert(0, to_insert);
		size_t found = cgi_response.find(to_insert);
		cgi_response.insert(found + strlen(to_insert), std::to_string(body_section.length())+ "\n");
	}
	const char * status_code_line = "HTTP/1.1 200 OK\n";
	cgi_response.insert(0,status_code_line);
	return cgi_response;
}