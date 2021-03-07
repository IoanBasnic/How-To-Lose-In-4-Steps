#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#define N 6
#define M 7
int key = 4;

int **board;
int column_full = 1, line, column = -1;
int error_index = 0;

void initializeBoard(){
	board = (int**)malloc(sizeof(int*)*N);

	for(int i = 0; i < M; i++){
		board[i] = (int*)malloc(sizeof(int) * M);
	}

	for(int i = 0; i < N; i++)
		for(int j = 0; j < M; j++)
			board[i][j] = 0;
}

int is_board_full()
{
    int i = N-1;
    int count = 0;
    
    for(int j=0; j<M; j++)
    {
        if(board[i][j] != 0)
        {
            count++;
        }
    }
    
    if(count == M)
        return 1;

    return 0;
}

int is_column_full(){
	return board[0][column] =! 0;
}

void make_move(int player)
{
  for(int i=N-1; i>=0; i--)
  {
  	if(board[i][column] == 0)
  	{
      board[i][column] = player + 1; 
      line = i;
      return;
  	}
  }
}

int chech_win(int player)
{
	int i,j;

    for(i=N-1;i>=0;i--)
        {
        	for(j=0;j<M-3;j++)
        	{
              if(board[i][j] != 0 && board[i][j]==board[i][j+1] && board[i][j]==board[i][j+2] && board[i][j]==board[i][j+3])
              {
                return 1;
              }
            }
        }
 
    for(i=N-4;i>=0;i--)
    {
        for(j=0;j<M;j++)
        {
            if(board[i][j] != 0 && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j] && board[i][j]==board[i+3][j])
            {
              return 1;
            }
        }    
    }   
 
    for(i=0;i<N-3;i++)
    {
        for(j=0;j<M-3;j++)
        {
            if(board[i][j] != 0 && board[i][j]==board[i+1][j+1] && board[i][j]==board[i+2][j+2] && board[i][j]==board[i+3][j+3])
            {
              return 1;
            }
        }
    }
    
    for(i=0;i<N-3;i++)
    {
        for(j=3;j<M;j++)
        {
            if(board[i][j] != 0 && board[i][j]==board[i+1][j-1] && board[i][j]==board[i+2][j-2] && board[i][j]==board[i+3][j-3])
            {
              return 1;
            }
        }
    }

    return 0;
}

int input_validation(char *input)
{
  char choice;
  
  if(strlen(input) != 1)
  	return 1;
    
   sscanf(input, "%c", &choice);

	if(isdigit(choice))
    	return 2;
   
	if(!((choice == 'A') || (choice == 'B') || (choice == 'C') || (choice == 'D') || 
	  (choice == 'E') || (choice == 'F') || (choice == 'G')))
    	return 2;

   column = choice - 'A';

   if(is_column_full())
   {
		column = -1;
     	return 3;
   }

   return 0;
}


char * encryptMessageServer(char * string)
{
	char *message = malloc(4*sizeof(char * ));;
	memcpy(message, string, strlen(string));
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

char * decryptMessageServer(char * string)
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

	//First we send each client the allocated to number, so they know which client they are
	for (int i = 0; i < 2; i++){
		client_fd[i] = accept(server_socket, NULL, NULL);
		int message = i;
		send(client_fd[i], &message, sizeof(message), 0);
	}

	int responseCode[2] = {1, 1};
	int whoseTurn = 0;
	int otherPlayer = 1;
	initializeBoard();

	while(responseCode[0] != 0 && responseCode[1] != 0) {
		char* messageToSend = "00-0-0-0";
		char* messageToReceive = (char*)malloc(sizeof(char));

		if(is_board_full()){
			messageToSend[5] = (char)(0);
			messageToSend[7] = (char)(3);
			for(int i = 0; i < 2; i++){
				send(client_fd[i], &messageToSend, sizeof(messageToSend), 0);
			}
			pclose(&server_socket);
			return 0;
		}

		//We wait for a valid message from the player
		int error_index = 0;
		do{
			printf("%s\n", messageToSend);
			printf("turn : %d\n", whoseTurn);
			responseCode[whoseTurn] = recv(client_fd[whoseTurn], messageToReceive, sizeof(char), 0);
			printf("got here\n");
			messageToReceive = decryptMessageServer(messageToReceive);
			printf("%c\n", messageToReceive);
			error_index = input_validation(messageToReceive);

			if(error_index){
				messageToSend[3] = (char)(whoseTurn + 1);
				messageToSend[5] = (char)(error_index);

				send(client_fd[whoseTurn], &messageToSend, sizeof(messageToSend), 0);
			}

		} while(error_index != 0);

		make_move(whoseTurn);

		if(chech_win(whoseTurn)){
			messageToSend[7] = (char)(whoseTurn + 1);
			for(int i = 0; i < 2; i++){
				send(client_fd[i], &messageToSend, sizeof(messageToSend), 0);
			}
			pclose(&server_socket);
			return 0;
		}

		if(line == -1 || column == -1){
			messageToSend[5] = (char)(6);
			for(int i = 0; i < 2; i++){
				send(client_fd[i], &messageToSend, sizeof(messageToSend), 0);
			}
			pclose(&server_socket);
			return 0;
		}
		else{

			messageToSend[0] = (char)(line);
			messageToSend[1] = (char)(column);
			messageToSend[3] = (char)(whoseTurn + 1);
			messageToSend[5] = (char)(0);
			messageToSend[7] = (char)(0);
			send(client_fd[otherPlayer], &messageToSend, sizeof(messageToSend), 0);
			send(client_fd[whoseTurn], &messageToSend, sizeof(messageToSend), 0);
					
			column = -1;
			line = -1;

			int aux = whoseTurn;
			whoseTurn = otherPlayer;
			otherPlayer = aux;
		}
        /* if not player turn -> error_index = 6
         * input_validation(message); v
         * if(!is_board_full()) v
         * {
         *   make_move(player //1 - 2);
         *   if(column_full) v
         *   {
               error_index = 5;
               // send to the client
             }
             else
             {
               status_code = chech_win(player) // 0 - 1 - 2
             }
         * }
         *  server_response = line+column-status_code-error_index
         */
	}

	if(responseCode[0] == 0){
		char* message = "00-2-4-4";
		char* encryptedMessage = encryptMessageServer(message);

		send(client_fd[1], &encryptedMessage, sizeof(encryptedMessage), 0);
	}
	else if(responseCode[1] == 0){
		char* message = "00-1-4-4";
		char* encryptedMessage = encryptMessageServer(message);

		send(client_fd[0], &encryptedMessage, sizeof(encryptedMessage), 0);
	}

	pclose(server_socket);

	return 0;
}
