#include <iostream>
#include "Routing.hpp"
#include "WebServSocket.hpp"


int main()
{

	WebServSocket Server("9034");
	Server.set_listening_socket();

	puts("pollserver: waiting for connections...");
	Server.server_poll();
	return 0;
}
