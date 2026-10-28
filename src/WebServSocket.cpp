#include "WebServSocket.hpp"
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <fstream>
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

WebServSocket::WebServSocket(const char *PORT): PORT_NMBR(PORT), fd_count(0)
{
}

WebServSocket::~WebServSocket()
{

}

void WebServSocket::server_poll(){
	for(;;) {
        int poll_count = poll(pfds.data(), fd_count, -1);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }
        // Run through connections looking for data to read
        process_connections();
    }
}


void WebServSocket::process_connections()
{
	
    for(int it = 0; it < fd_count; it++) {

        // Check if someone's ready to read
        if (pfds[it].revents & (POLLIN | POLLHUP)) {
            // We got one!!

            if (pfds[it].fd == listening_fd) {
                // If we're the listener, it's a new connection
                handle_new_connection();
            } else {
                // Otherwise we're just a regular client
                handle_client_data(it);
            }
        }
    }
}


void WebServSocket::del_from_pfds(int pfd_i)
{
	pfds[pfd_i] = pfds[fd_count-1];

	pfds.erase(pfds.end() - 1); // here might go some stuff wrong;
    (fd_count)--;
}

void	write_pipe(int fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		throw std::runtime_error("test");
	}
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

// std::stringstream string;
// 	std::ifstream f("html_pages/startpage.html");
// 	if (!f.is_open()) {
//         std::cerr << "Error opening the file!";
// 	}
// 	std::string buffer;
//     // Read each line from the file
// 	string << "HTTP/1.1 200 OK\n"
// 			<< "Content-Type: text/html\nContent-Length: 471\n\n";
// 	while (std::getline(f, buffer))
//         string << buffer << std::endl;;
// 	string.seekg(0, std::ios::end);
// 	int len = string.tellg();	

// 		sendall(newfd, string.str().data(), &len);

static int	response_cgi(int fd)
{
	int	pid;
	// (void) fd;
	pid = fork();
	if (pid_ok(pid))
	{
		write_pipe(fd);
		// const char *command[] = {"php-cgi","html_pages/tester.php", "status=200", NULL};
		// execve(const_cast<char *>("/usr/bin/php-cgi"), const_cast<char **>(command), NULL);
		const char *command[] = {"cat","html_pages/index.txt", NULL};
		execve(const_cast<char *>("/usr/bin/cat"), const_cast<char **>(command), NULL);
	}
	return (pid);
}

void WebServSocket::handle_client_data(int &pfd_i)
{
    char buf[256];    // Buffer for request data
    int nbytes = recv(pfds[pfd_i].fd, buf, sizeof buf, 0);
    int sender_fd = pfds[pfd_i].fd;

    if (nbytes <= 0) { // Got error or connection closed by client
        if (nbytes == 0) {
            // Connection closed
            std::cout << "pollserver: socket" << sender_fd << "hung up\n" ;
        } else {
            perror("recv");
        }
        close(pfds[pfd_i].fd); // Bye!
        del_from_pfds(pfd_i);
		--pfd_i;
    } else { // We got some good data from a client

        std::cout << "pollserver: recv from fd " << sender_fd << ": " << buf;
        // Send to everyone!
        response_cgi(sender_fd);
    }
}

void WebServSocket::add_to_pfds(int fd)
{
	pollfd new_pfd;
    new_pfd.fd = fd;
    new_pfd.events = POLLIN; // Check ready-to-read
    new_pfd.revents = 0;

	pfds.push_back(new_pfd);
    (fd_count)++;
}


void WebServSocket::handle_new_connection(){
	struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;
    int newfd;  // Newly accept()ed socket descriptor
    char remoteIP[INET6_ADDRSTRLEN];

    addrlen = sizeof remoteaddr;
    newfd = accept(listening_fd, (struct sockaddr *)&remoteaddr,
            &addrlen);

    if (newfd == -1) {
        perror("accept");
    } else {
        add_to_pfds(newfd);
		std:: cout 	<<	"pollserver: new connection from" 
					<< inet_ntop2(&remoteaddr, remoteIP, sizeof remoteIP)
					<<	" in on socket " 
					<< newfd << "\n"; 
    }
}

void WebServSocket::set_listening_socket()
{
    int listener;     // Listening socket descriptor
    int yes=1;        // For setsockopt() SO_REUSEADDR, below
    int rv;

    addrinfo hints, *ai, *p;
	pollfd new_listener;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT_NMBR, &hints, &ai)) != 0) {
		throw GetAddrInfoError(gai_strerror(rv));
        // fprintf(stderr, "pollserver: %s\n", gai_strerror(rv));
        // exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        throw ListeningError();
    }

    freeaddrinfo(ai); // All done with this

    // Listen
    if (listen(listener, 10) == -1) {
        throw ListeningError();
    }

    this->listening_fd = listener;

	new_listener.fd = listener;
	new_listener.events = POLLIN;
	pfds.push_back(new_listener);
	fd_count++;
}