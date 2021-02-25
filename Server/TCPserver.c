#include <stdio.h>
#include <stdlib.h>


#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int main(void) {
	char server_message[256] = "You have reached the server!";

	//create the server socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//bind the socket t our specified IP and port
	bind(server_socket, (struct sockaddr*) & server_address, sizeof(server_address));

	listen(server_socket, 5);

	int client_socket;

	int i = 1;

	//IANI STUFF - TEST
	char server_response[256];
	//

 	//send message
	 while(i) {

	 	
		client_socket = accept(server_socket, NULL, NULL);

		//IANI STUFF - TEST
	 	recv(client_socket, &server_response, sizeof(server_response), 0);
		printf("The client data: %s\n", server_response);
		//

		send(client_socket, server_message, sizeof(server_message), 0);
	 }

	//close socket
	close(server_socket);

	return 0;
}