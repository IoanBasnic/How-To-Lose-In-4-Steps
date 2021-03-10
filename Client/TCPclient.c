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

int server_socket;

errors server_errors[] = {
		[0] = {"Invalid input length"},
		[1] = {"Invalid character"},
		[2] = {"Invalid move"},
		[3] = {"The other player's server is down"},
		[4] = {"Not this player's turn"},
		[5] = {"Something unexpected went wrong"}
		};

void display_board()
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
				if (disc[j][k] != 0)
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

void clear_screen()
{
	system("clear");
}


void show_message_error(int input) // needs to be discused
{
	if(input >= 0 && input < 7)
		printf("%s\n", server_errors[input-1].error_name);
	else
		printf("Something wrong with the err number\n");
}

void print_wining_player(int input)
{
	switch(input)
	{
		case 1:
			printf("Game status: Player 1 won!\n");
			break;
		case 2:
			printf("Game status: Player 2 won!\n");
			break;
		case 3:
			printf("Game status: Tie!\n");
			break;
		case 4:
			printf("Game status: Some unexpected error occured!\n");
			break;
		default:
			printf("Something wrong with the input number\n");
	}

	sleep(3);
	close(server_socket);
	exit(0);
}

void apply_changes(char *server_input) //e.g. of input "11-1", where 11 is the position and "-1" (or "1") is the player
{
	disc[server_input[0] - '0'][server_input[1] - '0'] = server_input[3] - '0';
}

//ENCRYPT &  DECRYPT MESSAGE

char *encrypt_message_client(char *string, int key)
{

	char *message = malloc(4 * sizeof(char *));
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

char *decrypt_message_client(char *string, int key)
{

	char *message = malloc(10 * sizeof(char *));
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
	if ((server_input[7] - '0') == 0)
	{
		if ((server_input[5] - '0') == 0)
		{
			clear_screen();
			apply_changes(server_input);
			display_board();
		}
		else
		{
			show_message_error(server_input[5] - '0');
		}
	}
	else
	{
		clear_screen();
		apply_changes(server_input);
		display_board();
		print_wining_player(server_input[7] - '0');
	}

	return server_input[3] - '0';
}

int main(void)
{
	printf("Connecting to the server. Please wait...\n");
	//create socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	int option = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option));

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

	//if this is ever equal to 0, the connection to the server stopped
	int response_code;
	int whatPlayerAmI;
	char choice[256];
	char response[256];
	char *response_encrypt = malloc(10 * sizeof(char *));
	response_code = recv(server_socket, &whatPlayerAmI, sizeof(whatPlayerAmI), 0);
	printf("You are playing as player %d!\n", whatPlayerAmI + 1); // whatPlayerAmI = 0 or 1

	int checkErr = 0;
	if(whatPlayerAmI == 1)
	{
		display_board();
		printf("Please wait for player 1 move\n");
		response_code = recv(server_socket,  &response, 9, 0);
		response[8] = '\0';
		strcpy(response, decrypt_message_client(response, 4));
		
	}

	while (whatPlayerAmI == 0 && checkErr == 0)
	{
		display_board();
		printf("What column(A-G) would you like to start with?\n");
		//CHECK IF INPUT IS MORE THAN 1 CHAR
		scanf("%s", choice);
		while (strlen(choice) > 1)
		{
			printf("%s\n", server_errors[0].error_name);
			scanf("%s\n", choice);
		}

		memcpy(response_encrypt, encrypt_message_client(choice, 4), 1);
		char final_response = response_encrypt[0];
		send(server_socket, &final_response, sizeof(final_response), 0);
		response_code = recv(server_socket,  &response, 9, 0);
		response[8] = '\0';
		strcpy(response, decrypt_message_client(response, 4));

		if((response[5] - '0') != 0)
		{
			show_message_error(response[5] - '0');
		}
		else
		{
			checkErr = 1;
		}
	}
	
	int whoseTurn = menu(response);
	checkErr = 0;
	
	while (response_code)
	{	
		if(( whoseTurn - 1 ) == whatPlayerAmI) //00-1/2-0-0 //0 - 1 (client 1 sau client 2)
		{
			printf("Please wait for other player move\n");
			response_code = recv(server_socket, &response, sizeof(response), 0);
			response[8] = '\0';
			strcpy(response, decrypt_message_client(response, 4));
			whoseTurn = menu(response);
		}
		else
		{
			while(checkErr == 0)
			{
				printf("Choose a column (A-G):\n");
				//CHECK IF INPUT IS MORE THAN 1 CHAR
				scanf("%s", choice);
				memcpy(response_encrypt, encrypt_message_client(choice, 4), 1);
				char final_response = response_encrypt[0];
				send(server_socket, &final_response, sizeof(final_response), 0);
				response_code = recv(server_socket,  &response, 9, 0);
				response[8] = '\0';
				strcpy(response, decrypt_message_client(response, 4));

				if((response[5] - '0') != 0)
				{
					show_message_error(response[5] - '0');
				}
				else
				{
					checkErr = 1;
				}
			}
			whoseTurn = menu(response);
			checkErr = 0;
		}
		
	}

	sleep(3);
	close(server_socket);
	return 0;
}