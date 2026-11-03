#include "WebServSocket.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/poll.h>
#include <unistd.h>
#include "helper_func.h"


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

void WebServSocket::handle_cgi_response(std::map<int,int>::iterator resp)
{
	char buff[10000];
	int amnt_read;
	send(resp->second, "HTTP/1.1 200 OK\nContent-Length: 135\n",36,0);
	while ((amnt_read = read(resp->first, buff, 10000)) != 0)
	{
		sendall(resp->second, buff, &amnt_read);
	}
	std::vector<pollfd>::iterator found = std::find_if(pfds.begin(), pfds.end(), [&](pollfd& x){
		if (x.fd == resp->first)
			return (1);
		return (0);
	});
	if (found != pfds.end())
		del_from_pfds(found - pfds.begin());
	cgi_map_fds.erase(resp);
}

void WebServSocket::process_connections()
{
	std::map<int,int>::iterator resp;

    for(int it = 0; it < fd_count; it++) {

        // Check if someone's ready to read
		if (pfds[it].revents & (POLLIN | POLLHUP)) {
            // We got one!!
			if (pfds[it].fd == listening_fd) 
			{
				 // If we're the listener, it's a new connection
				handle_new_connection();
            } 
			else if ((resp = cgi_map_fds.find(pfds[it].fd)) != cgi_map_fds.end())
			{
				handle_cgi_response(resp);
			}
			else 
			{
                // Otherwise we're just a regular client
                handle_client_request(it);
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

int	WebServSocket::response_cgi(int fd)
{
	int	pid;
	int cgi_in[2];
	int cgi_out[2];
	int stdout_fd = dup(STDOUT_FILENO);

	pipe(cgi_in);
	pipe(cgi_out);
	write_pipe(cgi_in[1]);
	add_to_pfds(cgi_out[0]);
	cgi_map_fds.insert({cgi_out[0], fd});
	pid = fork();
	if (pid_ok(pid))
	{
		close(cgi_in[1]);
		listen_pipe(cgi_in[0]);
		write_pipe(cgi_out[1]);
		// const char *command[] = {"php-cgi", "-q","html_pages/tester.php", "status=200", NULL};
		// execve(const_cast<char *>("/usr/bin/php-cgi"), const_cast<char **>(command), NULL);
		// const char *command[] = {"cat","html_pages/index.txt", NULL};
		// execve(const_cast<char *>("/usr/bin/cat"), const_cast<char **>(command), NULL);
		const char *command[] = {"./html_pages/a.out","html_pages/index.txt", NULL};
		execve(const_cast<char *>(command[0]), const_cast<char **>(command), NULL);
	}
	close(cgi_out[1]);
	close(cgi_in[0]);
	close(cgi_in[1]);
	write_pipe(stdout_fd);
	return (pid);
}

void WebServSocket::handle_client_request(int &pfd_i)
{
    char buf[1000];    // Buffer for request data
    int nbytes = recv(pfds[pfd_i].fd, buf, sizeof buf, 0);
    int sender_fd = pfds[pfd_i].fd;

    if (nbytes <= 0) { // Got error or connection closed by client
        if (nbytes == 0) {
            // Connection closed
            std::cout << "pollserver: socket" << sender_fd << "hung up\n" ;
        } else {
			std::cerr << pfds[pfd_i].fd << "\n";
            perror("recv");
        }
        close(pfds[pfd_i].fd); // Bye!
        del_from_pfds(pfd_i);
		--pfd_i;
    } else { // We got some good data from a client

        std::cout << "pollserver: recv from fd " << sender_fd << ": " << std::string_view(buf, nbytes);;
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