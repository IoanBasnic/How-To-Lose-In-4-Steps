#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int board[N][M] = {0};
int column_full = 1, line, column;
int error_index = 7;

int is_board_full()
{
    int i = N-1;
    int count = 0;
    
    for(int j=0; j<M-1; j++)
    {
        if(board[i][j] != 0)
        {
            count++;
        }
    }
    
    if(count == M)
    {
        return 1;
    }
    
    return 0;
}

void pick_position(int player)
{
  for(int i=N-1; i>=0; i--)
  {
  	if(board[i][column] == 0)
  	{
      board[i][column] = player; 
      column_full = 0;
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
                return player;
              }
            }
        }
 
    for(i=N-4;i>=0;i--)
    {
        for(j=0;j<M;j++)
        {
            if(board[i][j] != 0 && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j] && board[i][j]==board[i+3][j])
            {
              return player;
            }
        }    
    }   
 
    for(i=0;i<N-3;i++)
    {
        for(j=0;j<M-3;j++)
        {
            if(board[i][j] != 0 && board[i][j]==board[i+1][j+1] && board[i][j]==board[i+2][j+2] && board[i][j]==board[i+3][j+3])
            {
              return player;
            }
        }
    }
    
    for(i=0;i<N-3;i++)
    {
        for(j=3;j<M;j++)
        {
            if(board[i][j] != 0 && board[i][j]==board[i+1][j-1] && board[i][j]==board[i+2][j-2] && board[i][j]==board[i+3][j-3])
            {
              return player;
            }
        }
    }

    return 0;
}

void input_validation(char *input)
{
  int valid_choice = 0;
  char choice;
  int j;
  
  if(strlen(input) < 1) {
  	error_index = 0;
  	return;
  }

  for(int i = 0; input[i+1] != '\0'; i++)
  {
    j = i;
    if(isspace(input[i]) && !isspace(input[j+1]))
    {
     	error_index = 1;
        return;
     }
  }
 
    if(strlen(input) != 1){
    	error_index = 2;
    	return;
    }
    
   sscanf(input, "%c", &choice);

   if(isdigit(choice))
   {
     error_index = 3;
     return;
   }
   
   if((choice == 'A') || (choice == 'B') || (choice == 'C') || (choice == 'D') || 
   	  (choice == 'E') || (choice == 'F') || (choice == 'G'))
   {
   	valid_choice = 1;
   }

   if(valid_choice != 1)
   {
     error_index = 4;
     return;
   }

   switch(choice)
   {
     case 'A': column = 0;
               break;
     case 'B': column = 1;
               break;
     case 'C': column = 2;
               break;
     case 'D': column = 3;
               break;
     case 'E': column = 4;
               break;
     case 'F': column = 5;
               break;
     case 'G': column = 6;
               break;
     default:  return;
               break;                

   }
}


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
        
        /* if not player turn -> error_index = 6
         * input_validation(message);
         * if(!is_board_full())
         * {
         *   pick_position(player //1 - 2);
         *   if(column_full)
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
         * 
         * 
         */
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
