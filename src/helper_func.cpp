#include <arpa/inet.h>
#include <cstddef>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

int sendall(int s, char *buf, int *len)
{
	int total = 0; // how many bytes we've sent
	int bytesleft = *len; // how many we have left to send
	int n;
	while(total < *len) {
 		n = send(s, buf+total, bytesleft, 0);
 		if (n == -1) { break; }
 		total += n;
 		bytesleft -= n;
	}

	*len = total; // return number actually sent here

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