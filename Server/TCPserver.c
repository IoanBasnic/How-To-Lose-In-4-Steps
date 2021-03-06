#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>



char * encryptMessageServer(char * string, int key)
{


	char *message;
	memcpy(message, string, sizeof(string));
	char aux;

	
	for(int i = 0; message[i] != '\0'; i++)
	{
		aux = message[i];
		
		if(aux >= '0' && aux <= '9')
		{	
			
			aux = aux + key;
			
			if(aux > '9')
			{

				aux = aux - '9' + '0' - 1;
			}

			message[i] = aux;

		}

		else if(aux =='-')
		{
			aux = aux + key;
			
			if(aux > '-')
			{
				aux = aux + '-' - 1;
			}

			message[i] = aux;
			
		}
	}

	
	return message;
}

char * decryptMessageServer(char * string, int key)
{

	
	char *message = malloc(4*sizeof(char * ));
	strcpy(message, string);
	char aux;

	
	for(int i = 0; message[i] != '\0'; i++)
	{
		aux = message[i];
		
		if(aux >= 'a' && aux <= 'z')
		{	
			
			aux = aux - key;
			
			if(aux < 'a')
			{

				aux = aux + 'z' - 'a' + 1;
			}

			message[i] = aux;

		}

		else if(aux >= 'A' && aux <= 'Z')
		{
			aux = aux - key;
			if(aux < 'A')
			{
				aux = aux + 'Z' - 'A' + 1;
			}

			message[i] = aux;
			
		}
	}

	
	return message;
}


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

	listen(server_socket, 2);

	int client_fd[2];

	//IANI STUFF - TEST
	char server_response[256];
	//
	for (int i = 0; i < 2; i++){
		client_fd[i] = accept(server_socket, NULL, NULL);
		int message = i;
		send(client_fd[i], &message, sizeof(message), 0);
	}

	while(1) {
		int message = 0;
		recv(client_fd[0], &message, sizeof(message), 0);
		printf("%d\n", message);
	}

	//  while(i) {
 	
	// 	//IANI STUFF - TEST
	//  	//recv(client_fd, &server_response, sizeof(server_response), 0);
	// 	//printf("The client data: %s\n", server_response);
	// 	//

	// 	//send(client_fd, server_message, sizeof(server_message), 0);
	// 	int message[3] = {i, i, i};
	// 	send(client_fd[i], message, sizeof(message), 0);
	//  }

	//close socket
	pclose(server_socket);

	return 0;
}