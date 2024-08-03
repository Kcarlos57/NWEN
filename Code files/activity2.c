#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<unistd.h>


#define DEFAULT_STRLEN 100
#define SERVER_PORT  23456

int create_server_sock(struct sockaddr_in *address);
void handle_client(int clientfd);
void reverse_input(char *word, int begin, int end);

int main(void)
{
    int sockfd;

    struct sockaddr_in address;


    /* Create and bind socket using the function create_server_sock. Print an error message if unsucessful*/
    // Create server socket
    sockfd = create_server_sock(&address);
    if (sockfd == -1)
    {
        printf("Error: Failed to create socket\n");
        return 0;
    }

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Error: Failed to bind socket\n");
        return 0;
    }


    /* Listen for incoming connections. Print an error message if unsucessful */
    if(listen(sockfd, SOMAXCONN) < 0) {
        printf("Error: Failed to listen for connections\n");
        return 0;
    }

        /* Accept */
        while(1){
        int addrlen = sizeof(address);
        int clientfd = accept(sockfd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen);

        /* Handle client */
        handle_client(clientfd);
        }
    return 0;
}

int create_server_sock(struct sockaddr_in *address)
{
    int sockfd;

   // Create socket file descriptor. Return -1 if error
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) return -1;


    // Define Server address
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(SERVER_PORT);



    // Bind socket to server address. Return -2 if error
    if (bind(sockfd, (struct sockaddr *)address, sizeof(*address)) < 0)
    {
        perror("bind failed");
        return -2;
    }


    //return socket file descriptor
    return sockfd;
}


void handle_client(int clientfd)
{
    char outmsg[DEFAULT_STRLEN];
    char inmsg[DEFAULT_STRLEN];


    if(clientfd < 0) {
        printf("Error: Failed to accept client connection\n");
        close(clientfd);
        return;
    }
     /* Read the msg from the client*/
    if (read(clientfd, inmsg, DEFAULT_STRLEN) < 0) {
        printf("Error: Failed to read from client\n");
        close(clientfd);
        return;
    }

     /* Reverse the msg using function reverse_input*/

    reverse_input(inmsg, 0, strlen(inmsg) - 1);


     /* Send the reversed string back to client*/
    if (write(clientfd, inmsg, strlen(inmsg)) <= 0)
    {
        printf("Error: Failed to send reversed string to client\n");
        close(clientfd);
        return;
    }

    close(clientfd);

}

void reverse_input(char *word, int begin, int end){
    char temp;
    while (begin < end)
    {
        temp = word[begin];
        word[begin] = word[end];
        word[end] = temp;
        begin++;
        end--;
    }
}
