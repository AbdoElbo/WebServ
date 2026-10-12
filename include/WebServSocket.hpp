#include "exception"
#include "string"
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <vector>
#ifndef WEBSERVSOCKET_HPP
#define WEBSERVSOCKET_HPP

class WebServSocket
{
	public:
		WebServSocket(const char *PORT);
		WebServSocket(const WebServSocket& other);
		WebServSocket& operator=(const WebServSocket& other);
		~WebServSocket();

		void set_listening_socket();
		void server_poll();
		void process_connections();
	
	private:
		std::vector<pollfd> pfds;
		const char* PORT_NMBR;
		int listening_fd;
		int fd_size = 5;
		int fd_count = 0;
		void handle_client_data(std::vector<pollfd>::iterator& pfd_i);
		void handle_new_connection();
		void del_from_pfds(std::vector<pollfd>::iterator& pfd_i);
		void add_to_pfds(int fd);
	
		class GetAddrInfoError : public std::exception {
			private:
				std::string message;
    		public:
				GetAddrInfoError(std::string message) : message("GetAddrInfoError: pollserver: " + message) {}
	
        		const char* what() const noexcept override {
        			return ((message.c_str()));
				}
    	};
    	class ListeningError : public std::exception {
    		public:
        		const char* what() const noexcept override {
        		return "Error getting listening socket";
    	};
	}; 
};	

#endif // WEB_SERV_SOCKET_HPP