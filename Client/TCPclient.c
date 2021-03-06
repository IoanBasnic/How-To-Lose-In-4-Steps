#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>


#include <netinet/in.h>


#define ANSI_COLOR_RED  "\x1B[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_WHITE    "\x1b[47m"
#define ANSI_COLOR_RESET   "\x1b[0m"
// 0 - nothing, 1 - player 1, 2 - player 2

int disc[6][7] = {
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
};


// Client2Server
// [A-H]

// Server2Client
// [1-7][1-6]-[1-2]-[0-?]-[0-1-2-3]
// [row][column]-[player]-[encoded error]-[game finished(0 - not finished, 1 - player 1 won, 2 - player 2 won, 3 - tie)]

// +---+---+---+---+---+---+---+
// |   |   |   |   |   |   |   |
// +---+---+---+---+---+---+---+
// |   |   |   |   |   |   |   |
// +---+---+---+---+---+---+---+
// |   |   |   |   |   |   |   |
// +---+---+---+---+---+---+---+
// |   |   |   |   |   |   |   |
// +---+---+---+---+---+---+---+
// |   |   |   |   |   |   |   |
// +---+---+---+---+---+---+---+
// |   |   |   |   |   |   |   |
// +---+---+---+---+---+---+---+


void displayBoard()
{

	for(int i = 0, j = 0; i < 13; i++)
	{
		if(i % 2 == 0)
		{
			printf("  +---+---+---+---+---+---+---+\n");

			if(i == 12)
				printf("    A   B   C   D   E   F   G  \n");
		}

		else
		{
			printf("%d ", j+1);
			for(int k = 0; k < 7; k++)
			{
				printf("|");
				if(disc[j][k] != 0)	//testing poate nu printam 0/1/2
				{
					
					if(disc[j][k] == 1)
						printf(ANSI_COLOR_YELLOW"%s"ANSI_COLOR_YELLOW" "ANSI_COLOR_RESET, " O");
					if(disc[j][k] == 2)
						printf(ANSI_COLOR_RED"%s"ANSI_COLOR_RED" "ANSI_COLOR_RESET, " O");
				}
				else
					printf("   ");
				
			}
			printf("|\n");
			j++;
		}
	}
}

void clearScreen() 
{
	for(int i = 0; i < 1; i++)
	{
		//printf("\n");
		system("clear");
	}
}

void ShowMessageError(char *server_input) // needs to be discused
{

}

void applyChanges(char *server_input) //e.g. of input "11-1", where 11 is the position and "-1" (or "1") is the player
{
	//printf("%d %d %d", server_input[0] - '0', server_input[1] - '0', server_input[3] - '0');
	disc[ server_input[0] - '1'][ server_input[1] - '1'] =  server_input[3] - '0';
}




//ENCRYPT &  DECRYPT MESSAGE

char * encryptMessageClient(char * string, int key)
{


	char *message;
	memcpy(message, string, sizeof(string));
	char aux;

	
	for(int i = 0; message[i] != '\0'; i++)
	{
		aux = message[i];
		
		if(aux >= 'a' && aux <= 'z')
		{	
			
			aux = aux + key;
			
			if(aux > 'z')
			{

				aux = aux - 'z' + 'a' - 1;
			}

			message[i] = aux;

		}

		else if(aux >= 'A' && aux <= 'Z')
		{
			aux = aux + key;
			if(aux > 'Z')
			{
				aux = aux - 'Z' + 'A' - 1;
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




char * decryptMessageClient(char * string, int key)
{

	
	char *message = malloc(4*sizeof(char * ));
	strcpy(message, string);
	char aux;

	
	for(int i = 0; message[i] != '\0'; i++)
	{
		aux = message[i];
		
		if(aux >= '0' && aux <= '9')
		{	
			
			aux = aux - key;
			
			if(aux < '0')
			{

				aux = aux + '9' - '0' + 1;
			}

			message[i] = aux;

		}

		else
		{
			aux = aux - key;
			
			if(aux > '-')
			{
				aux = aux - '-' + 1;
			}

			message[i] = aux;
			
		}
	}

	
	return message;
}

void menu(char *server_input) //prints text and board game and apply the changes
{

	if( (server_input[5] - '0') == 0)
	{
		clearScreen();
		applyChanges(server_input);
		displayBoard();
	}
	else
	{
		ShowMessageError(server_input);	
	}
}

int main(void) {


	
	// printf("%s\n", encryptMessage("11-1-0", 4));
	// printf("%s\n", decryptMessage("55]5]4", 4));
	
	int error = 0;
	socklen_t len = sizeof (error);

		// "32-1/2-"
	//******* TEST

	//displayBoard();
	printf("Connecting to the server. Please wait...\n");
	//****** /TEST
	//create socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons (9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(server_socket, (struct sockaddr * ) &server_address, sizeof(server_address));

	//check for error with connection

	if(connection_status == -1) {
		printf("There was an error, can't make a connection to the remote socket\n");
		exit(EXIT_FAILURE);
	}

	//receive data from the server
	char server_response[256] = "None";
	char MyResponse[256];
	int server_fd;

	int message = 0;

	server_fd = accept(server_socket, NULL, NULL);
	//displayBoard();

	//if this is ever equal to 0, the connection to the server stopped
	int response_code;

	while(response_code != 0)
	{
		// menu(MyResponse);

		response_code = recv(server_socket, &message, sizeof(message), 0);
		printf("%d\n", message);

	}

	//print out the servers response
	printf("The server data: %s\n", server_response);
	pclose(&server_socket);
	return 0;
}