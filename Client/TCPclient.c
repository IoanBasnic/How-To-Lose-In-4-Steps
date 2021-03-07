#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <unistd.h>

#define ANSI_COLOR_RED "\x1B[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_WHITE "\x1b[47m"
#define ANSI_COLOR_RESET "\x1b[0m"
// 0 - nothing, 1 - player 1, 2 - player 2

int disc[6][7] = {
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
};

typedef struct
{
	char error_name[256];
} errors;

errors server_errors[] = {
		[0] = {"Please choose a column"},
		[1] = {"Please choose only one column"},
		[2] = {"Please insert only one character"},
		[3] = {"Please insert only letters, digits are not accepted"},
		[4] = {"Please choose a letter only from A to G"},
		[5] = {"This column is full, you can't place any piece"},
		[6] = {"Please wait your turn"}};

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

	for (int i = 0, j = 0; i < 13; i++)
	{
		if (i % 2 == 0)
		{
			printf("  +---+---+---+---+---+---+---+\n");

			if (i == 12)
				printf("    A   B   C   D   E   F   G  \n");
		}

		else
		{
			printf("%d ", j + 1);
			for (int k = 0; k < 7; k++)
			{
				printf("|");
				if (disc[j][k] != 0) //testing poate nu printam 0/1/2
				{

					if (disc[j][k] == 1)
						printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_YELLOW " " ANSI_COLOR_RESET, " O");
					if (disc[j][k] == 2)
						printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RED " " ANSI_COLOR_RESET, " O");
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
	for (int i = 0; i < 1; i++)
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
	disc[server_input[0] - '1'][server_input[1] - '1'] = server_input[3] - '0';
}

//ENCRYPT &  DECRYPT MESSAGE

char *encryptMessageClient(char *string, int key)
{

	char *message = malloc(4 * sizeof(char *));
	;
	memcpy(message, string, strlen(string));
	char aux;

	for (int i = 0; message[i] != '\0'; i++)
	{
		aux = message[i];

		if (aux >= 'a' && aux <= 'z')
		{

			aux = aux + key;

			if (aux > 'z')
			{

				aux = aux - 'z' + 'a' - 1;
			}

			message[i] = aux;
		}

		else if (aux >= 'A' && aux <= 'Z')
		{
			aux = aux + key;
			if (aux > 'Z')
			{
				aux = aux - 'Z' + 'A' - 1;
			}

			message[i] = aux;
		}
	}

	return (char *)message;
}

char *decryptMessageClient(char *string, int key)
{

	char *message = malloc(4 * sizeof(char *));
	strcpy(message, string);
	char aux;

	for (int i = 0; message[i] != '\0'; i++)
	{
		aux = message[i];

		if (aux >= '0' && aux <= '9')
		{

			aux = aux - key;

			if (aux < '0')
			{

				aux = aux + '9' - '0' + 1;
			}

			message[i] = aux;
		}

		else
		{
			aux = aux - key;

			if (aux > '-')
			{
				aux = aux - '-' + 1;
			}

			message[i] = aux;
		}
	}

	return message;
}

int menu(char *server_input) //prints text and board game and apply the changes
{

	if ((server_input[5] - '0') == 0)
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

int main(void)
{

	// printf("%s\n", encryptMessage("11-1-0", 4));
	// printf("%s\n", decryptMessage("55]5]4", 4));

	int error = 0;
	socklen_t len = sizeof(error);

	// "32-1/2-"
	//******* TEST

	//displayBoard();
	printf("Connecting to the server. Please wait...\n");
	//****** /TEST
	//create socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	int option = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

	//check for error with connection

	if (connection_status == -1)
	{
		printf("There was an error, can't make a connection to the remote socket\n");
		exit(EXIT_FAILURE);
	}

	//receive data from the server
	char server_response[256] = "None";
	char MyResponse[256];

	char message = 0;

	//if this is ever equal to 0, the connection to the server stopped
	int response_code;
	int whatPlayerAmI;
	char choice[256];
	char *response = (char *)malloc(sizeof(char) * 8);
	char *response_encrypt = (char *)malloc(sizeof(char) * 8);
	response_code = recv(server_socket, &whatPlayerAmI, sizeof(whatPlayerAmI), 0);
	printf("You are playing as player %d!\n", whatPlayerAmI + 1); // whatPlayerAmI = 0 or 1
	int howManyBytesSent = 0;
	if (whatPlayerAmI == 0)
	{
		displayBoard();
		printf("What column(A-G) would you like to start with?\n");

		//CHECK IF INPUT IS MORE THAN 1 CHAR
		scanf("%s", choice);

		while (strlen(choice) > 1)
		{
			printf("%s\t %s\n", server_errors[2].error_name, server_errors[4].error_name);
			scanf("%s\n", choice);
		}

		memcpy(response_encrypt, encryptMessageClient(choice, 4), 1);
		char final_response = response_encrypt[0];
		printf("%c\n", final_response);
		send(server_socket, &final_response, sizeof(final_response), 0);

		response_code = recv(server_socket, &response, sizeof(response), 0);
	}
	

	int whoseTurn;

	while (response_code != 0)
	{
		response_code = recv(server_socket, &response, sizeof(response), 0);
		//whoseTurn = menu(response);

		// if(whatPlayerAmI == whoseTurn){
		//
		// 	send
		//  response_code = recv(server_socket, &response, sizeof(response), 0);
		// }

		
	}

	//print out the servers response
	printf("The server data: %s\n", server_response);
	close(server_socket);
	return 0;
}