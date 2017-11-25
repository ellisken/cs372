/********************************************************************
 * ** Program Filename: ftserver.c
 * ** Author: Kendra Ellis
 * ** Date: 11/26/2017
 * ** Description: Implementation of a TCP connected text file
 *      transfer server.
 * ** Input: None, but meant to interact with ftclient.py.
 * ** Output: When prompted, sends either a current listing of
 *      ftserver.c's directory or transfers the .txt file specified
 *      by ftclient.py.
 * ** Sources Cited: This program relies heavily on...
 * *********************************************************************/

#include <stdio.h> //input/output
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> //defn's of data types used in system calls
#include <sys/socket.h> //defn's of structures needed for sockets
#include <netinet/in.h> //constants and structs needed for internet domain addrs
#include <netdb.h>
const int BUFFER_SIZE = 500;



/*********************************************************************
 * ** Function:error
 * ** Description:Uses the c function perror() to print a descriptive
 *      error message to stderr.
 * ** Source: This function comes from:
 *      http://www.linuxhowtos.org/C_C++/socket.htm
 * ** Parameters:Takes a constant char message
 * ** Pre-Conditions:None
 * ** Post-Conditions:Will exit the program.
 * *********************************************************************/
void error(const char *msg){
    perror(msg);
    exit(1);
}



/*********************************************************************
 * ** Function: createSocket()
 * ** Description: Creates a new socket and checks that the socket
 *      creation was successful.
 * ** Parameters: Takes the address of a socket file descriptor.
 * ** Pre-Conditions: There's a file descriptor created to receive
 *      the socket.
 * ** Post-Conditions: The socket is open.
 * *********************************************************************/
void createSocket(int *socketFD){
    //Open socket
    *socketFD = socket(AF_INET, SOCK_STREAM,0);

    //Check socket creation success
    if(*socketFD < 0) error("ERROR opening socket");
}



/*********************************************************************
 * ** Function:startUp()
 * ** Description:Starts the server up listening on the port specified
 *      on the command line.
 * ** Parameters: The serverPort number defined on command line, the
 *      file descriptor for the server socket, a pointer to the server's
 *      sockaddr_in struct, and the file descriptor for the socket.
 * ** Pre-Conditions: The server port number must be valid and
 *      defined, the server's socket must be instantiated.
 * ** Post-Conditions: A message will display in the console that the
 *      server is open and listening on the provided port number
 * *********************************************************************/
void startUp(int portNum, struct sockaddr_in *servAddr, int sockFD){
    //Initialize servAddr to zeros
    bzero((char*) servAddr, sizeof(*servAddr));

    //Set address family, port number(network byte order), and
    //host IP address
    servAddr->sin_family = AF_INET;
    servAddr->sin_addr.s_addr = INADDR_ANY;
    servAddr->sin_port = htons(portNum);

    //Bind the socket to the host address and port number
    if(bind(sockFD,(struct sockaddr *) servAddr, sizeof(*servAddr)) < 0)
        error("ERROR on binding");

    //Start listening for connections
    listen(sockFD,5);
    printf("Server open and listening on port %i.\n", portNum);
}



/*********************************************************************
 * ** Function: sendMsg()
 * ** Description: Sends messages to the client over the specified
 *      socket. Terminates if there's an error writing to the socket.
 * ** Parameters: A socket file descriptor (for the socket
 *      over which the message will be sent), a pointer to a
 *      const char message to be sent.
 * ** Pre-Conditions: The connection file descriptor must be associated
 *      with an open socket. The char array must be malloc'd.
 * ** Post-Conditions: The message will be sent to a connected client.
 * *********************************************************************/
void sendMsg(int socketFD, const char *msg){
    //Write to the socket
    int success = write(socketFD, msg, BUFFER_SIZE);
    if(success < 0)
        error("ERROR writing message to socket.");
}



/*********************************************************************
 * ** Function: recMsg()
 * ** Description: Reads messages from the socket for handling.
 *      Function does its own error checking and terminates the
 *      program if there's an error.
 * ** Parameters: A pointer to a char array, a file descriptor
 *      for the communication socket.
 * ** Pre-Conditions: There is an active socket connection and a
 *      char array has been malloc'd in main.
 * ** Post-Conditions: The message will be available for use by other
 *      functions like handleRequest. If an error occurs, the program
 *      will terminate.
 * *********************************************************************/
void recMsg(char *buffer, int socketFD){
    //Zero out the buffer
    bzero(buffer, BUFFER_SIZE);

    //Read from the socket
    int n = read(socketFD, buffer, BUFFER_SIZE);

    //Check for success
    if(n < 0) error ("ERROR reading from socket");
}




/*********************************************************************
 * ** Function: acceptClient()
 * ** Description: Starts accepting clients on the socket created.
 * ** Parameters: A pointer to a sockaddr_in struct for the client's
 *      information, the address of a file descriptor for the control
 *      connection, the server socket's file descriptor.
 * ** Pre-Conditions: The server must be open and listening.
 * ** Post-Conditions: The server will accept client connections.
 **********************************************************************/
void acceptClient(struct sockaddr_in *cliAddr, int *controlFD, int servFD){
    socklen_t clilen = sizeof(*cliAddr);

    //Start accepting client connections
    *controlFD = accept(servFD,(struct sockaddr*) cliAddr, &clilen);

    //Check for success
    if(*controlFD < 0) error("ERROR on accept");
}



/*********************************************************************
 * ** Function: sendDir()
 * ** Description:
 * ** Parameters:
 * ** Pre-Conditions:
 * ** Post-Conditions:
 * *********************************************************************/



/*********************************************************************
 * ** Function: sendFile()
 * ** Description:
 * ** Parameters:
 * ** Pre-Conditions:
 * ** Post-Conditions:
 * *********************************************************************/




/*********************************************************************
 * ** Function: handleRequest()
 * ** Description:
 * ** Parameters:
 * ** Pre-Conditions:
 * ** Post-Conditions:
 * *********************************************************************/
void handleRequest(){
    //Receive command from the client
    //If command is -l
        //Send current directory listing across
    //If command is -g
        //Validate file name
            //If valid, send file across
            //Else send error message
    //Else send error message

}


/*MAIN*/
int main(int argc, char *argv[]){
    //Variable, file descriptors, and Struct definitions
    int listenSockFD, connectSockFD, dataSockFD;
    int portNum;
    //socklen_t clilen; //Stores size of address of the client
    struct sockaddr_in *servAddr = malloc(sizeof(struct sockaddr_in));
    struct sockaddr_in *cliAddr=malloc(sizeof(struct sockaddr_in)); //From <netinet/in.h>
    int n; //For storing number of chars read or written
    char *buffer = malloc(BUFFER_SIZE); //For storing characters exchanged in socket connection

    //command-line parameter validation
    if(argc < 2){
        fprintf(stderr, "ERROR, no port provided.\n");
        printf("usage: ./executableName portNum.\n\tportNum: must be in range 4,000-65,000.\n");
        exit(1);
    }
    //Check valid port number
    if(atoi(argv[1]) < 4000 || atoi(argv[1]) > 65000){
        fprintf(stderr, "ERROR, invalid port number.\n");
        printf("usage: ./executableName portNum.\n\tportNum: must be in range 4,000-65,000.\n");
        exit(1);
    }

    //Convert specified portNum to int
    portNum = atoi(argv[1]);

    //Create a new socket
    createSocket(&listenSockFD);

    //Start up server to listen
    startUp(portNum, servAddr, listenSockFD);

    //Accept client connection
    acceptClient(cliAddr, &connectSockFD, listenSockFD);

    //Get command and other info from the client
    recMsg(buffer, connectSockFD);
    printf("Here is the command the client sent: %s\n", buffer);

    //Establish data connection
    createSocket(&dataSockFD);
    printf("Data socket created.\n");

    cliAddr->sin_port = htons(3762);
    if(connect(dataSockFD, (struct sockaddr*) cliAddr, sizeof(*cliAddr)) < 0)
        error("ERROR establishing data connection.\n");
    printf("Data connecting poopies\n");
    bzero(buffer, BUFFER_SIZE);
    printf("Please enter your message: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    sendMsg(dataSockFD, buffer);
    //n = write(dataSockFD, "I got your command", 18);
    //if(n < 0) error("ERROR writing to socket.");

    //Close socket
    close(dataSockFD);
    //close(connectSockFD);
    free(servAddr);
    free(buffer);
    return 0;
}
