#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>


#include <netinet/in.h>


int main(void) {

	//create socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM, 0);

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons (9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(network_socket, (struct sockaddr * ) &server_address, sizeof(server_address));

	//check for error with connection

	if(connection_status == -1) {
		printf("There was an error, can't make a connection to the remote socket\n");
	}

	//receive data from the server
	char server_response[256];
	recv(network_socket, &server_response, sizeof(server_response), 0);

	//print out the servers response
	printf("The server data: %s\n", server_response);
	close(network_socket);
	return 0;
}