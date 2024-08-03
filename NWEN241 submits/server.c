/**
 * Skeleton file for server.c
 * 
 * You are free to modify this file to implement the server specifications
 * as detailed in Assignment 3 handout.
 * 
 * As a matter of good programming habit, you should break up your imple-
 * mentation into functions. All these functions should contained in this
 * file as you are only allowed to submit this file.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
// Include necessary header files

/**
 * The main function should be able to accept a command-line argument
 * argv[0]: program name
 * argv[1]: port number
 * 
 * Read the assignment handout for more details about the server program
 * design specifications.
 */ 

#define MAX_MSG_LENGTH 1024
#define MAX_FILE_LENGTH 2000
//port number is 5057
int main(int argc, char *argv[])
{
  int port;
  struct sockaddr_in server_address, client_address;
  socklen_t client_address_len;
  char clientMSG[MAX_MSG_LENGTH];
  char buffer[MAX_MSG_LENGTH];
  int server_socket, client_socket, status;
  int exitflag = 0;
  
  // Check if port number argument is specified
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    return -1;
  }
  
  // Parse port number argument
  port = atoi(argv[1]);
  if (port < 1024) {
    fprintf(stderr, "Invalid port number: %d\n", port);
    return -1;
  }
  
  // Create server socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Error creating server socket");
    return -1;
  }
  
  // Bind server socket to port
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(port);
  status = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
  if (status < 0) {
    perror("Error binding server socket");
    close(server_socket);
    return -1;
  }
  
  // Listen for incoming connections
  status = listen(server_socket, 10);
  if (status < 0) {
    perror("Error listening for connections");
    close(server_socket);
    return -1;
  }
  
  printf("Server listening on port %d\n", port);
  
  // Accept incoming connections and handle them
  if (!exitflag){
    client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0) {
      perror("Error accepting connection");
      close(server_socket);
      return -1;
    }
  }
  while (!exitflag) {
    
    //reset buffer
    memset(clientMSG,0, MAX_MSG_LENGTH);
    
    // Send HELLO message to client
    status = send(client_socket, "HELLO\n", strlen("HELLO\n"), 0);
    if (status < 0) {
      perror("Error sending message to client");
      close(client_socket);
      close(server_socket);
      return -1;
    }
    printf("\nClient connected\n");
    
      // Wait for message from client
      status = recv(client_socket, clientMSG, MAX_MSG_LENGTH, 0);
      if (status < 0) {
        perror("Error receiving message from client");
        close(client_socket);
        close(server_socket);
        return -1;
      }
      sscanf(clientMSG,"%[^\n]", buffer);
      printf("Received message: %s\n", buffer);
      
      if (strcmp(buffer, "bye") == 0 || strcmp(buffer, "BYE") == 0) {
        printf("Received BYE message\n");
        // Close the connection
        exitflag = 1;
        close(client_socket);
        close(server_socket);
      } 
      
      else if (strncmp(buffer, "GET ", 4) == 0 || strncmp(buffer, "get ", 4) == 0) {
        char *filename = buffer + 4; // Skip over "GET " or "get "
        FILE *fp = fopen(filename, "r");
        if (fp) {
          // File opened successfully, send contents to client
          printf("file opened");
          char filebuf[MAX_FILE_LENGTH];
          int filelen = fread(filebuf, sizeof(char), MAX_FILE_LENGTH, fp);
          fclose(fp);
          char response[MAX_MSG_LENGTH];
          sprintf(response, "SERVER 200 OK\n\n%s\n\n\n", filebuf);
          status = send(client_socket, response, strlen(response), 0);
          if (status < 0) {
            perror("Error sending message to client");
            close(client_socket);
            close(server_socket);
            return -1;
          }
        } else if (errno == ENOENT) {
          // File not found
          char response[] = "SERVER 404 Not Found\n";
          status = send(client_socket, response, strlen(response), 0);
          // Close the connection 
          exitflag = 1;
          close(client_socket);
          close(server_socket);
          if (status < 0) {
            perror("Error sending message to client");
            close(client_socket);
            close(server_socket);
            return -1;
          }
        } else {
          // Other error
          char response[] = "SERVER 500 Get Error\n";
          status = send(client_socket, response, strlen(response), 0);
          // Close the connection
          exitflag = 1;
          close(client_socket);
          close(server_socket);
          if (status < 0) {
            perror("Error sending message to client");
            close(client_socket);
            close(server_socket);
            return -1;
          }
        }
        
      } else if (strncasecmp(buffer, "put ", 4) == 0) {
        printf("Received PUT message\n");
        // Get the filename from the message
        char *filename = buffer + 4;
        printf("filename gotten\n");
        // Open the file for writing
        FILE *file = fopen(filename, "w");
        printf("file opened: %s\n", filename);
        if (file == NULL) {
          // Send error response to client
          printf("file is null");
          status = send(client_socket, "SERVER 501 Put Error\n", strlen("SERVER 501 Put Error\n"), 0);
          if (status < 0) {
            perror("Error sending response to client");
            close(client_socket);
            close(server_socket);
            return -1;
          }
          close(client_socket);
          continue;
        }
        
        // Write messages to file until two consecutive empty lines are received
        int empty_line_count = 0;
        while (empty_line_count < 2) {
          status = recv(client_socket, buffer, MAX_MSG_LENGTH, 0);
          printf("empty lines not yet 2.");
          if (status < 0) {
            perror("Error receiving message from client");
            fclose(file);
            close(client_socket);
            close(server_socket);
            return -1;
          }
          
          if (status == 0) {
            // Connection closed by client
            break;
          }
          
          //buffer[status] = '\0';
          
          if (strcmp(buffer, "\n") == 0) {
            empty_line_count++;
            printf("empty line+1!");
          } else {
            empty_line_count = 0;
            fputs(buffer, file);
          }
        }
        
        // Write the last two consecutive empty lines to file
        fputs("\n\n", file);
        
        // Close the file
        fclose(file);
        
        // Send success response to client
        status = send(client_socket, "SERVER 201 Created\n", strlen("SERVER 201 Created\n"), 0);
        if (status < 0) {
          perror("Error sending response to client");
          close(client_socket);
          close(server_socket);
          return -1;
        }
      } else {
        // Invalid command received, send error response to client
        status = send(client_socket, "SERVER 502 Command Error\n", strlen("SERVER 502 Command Error\n"), 0);
        if (status < 0) {
          perror("Error sending response to client");
          close(client_socket);
          close(server_socket);
          return -1;
        }
      
      }
    }
    close(client_socket);
  close(server_socket);
  return 0;
}

