"""
    Description: Implementation of a TCP connected simple file transfer
            program.
    Input:
    Output: Receives directory listing or .txt file transfer from
            ftserver.c
    Sources Cited: This program relies heavily on the TCP client example
            provided in "Computer Networking: A Top-Down Approach, 7th ed."
            by J. Kurose and K. Ross
"""
from socket import *
import argparse #For argument parsing
import os #For interacting with current directory

def main():
    #Get server name, server port number, command,
    #file name, and data port number from console
    parser = argparse.ArgumentParser()
    parser.add_argument('server', nargs=1, default="none", type=str, help='Server\'s name or IP.')
    parser.add_argument('servPort', nargs=1, default=0, type=int, help='Server\'s port number.')
    parser.add_argument('-l', dest='listDir', action='store_true', default=False, help='Request dir listing.')
    parser.add_argument('-g', dest='fileName', default="%none", type=str, help='Request file transfer. Takes file name arg.')
    parser.add_argument('dataPort', nargs=1, default=0, type=str, help='Data connection port num. Must be valid.')


    args = parser.parse_args()
    server = args.server[0]
    servPort = args.servPort[0]
    listDir = args.listDir
    fileName = args.fileName
    dataPort = args.dataPort[0]


    #Create socket
    clientSocket = initiateContact(servPort, server)
    print("Connection established with server on port: " + str(servPort))

    #Send command or file name on control connection
    makeRequest(listDir, fileName, clientSocket)

    #Start listening on specified dataPort
    dataSocket = startListening(int(dataPort))

    #Send data port
    clientSocket.sendall(dataPort)

    #Receive file or response from server
    transferSocket, addr = dataSocket.accept()
    response = getServResponse(transferSocket)
    handleResponse(response, transferSocket, fileName, dataPort)

    #Close control connection sockets
    clientSocket.close()




""" Function: initiateContact()
    Description: Creates a socket and initiates contact with
        the server on the port specified on the command-line.
    Parameters: The port number, the server name.
    Pre-Conditions: The server name and port number must be
        defined.
    Post-Conditions: Returns a file descriptor to the socket opened.
"""
def initiateContact(portNum, serverName):
    #Open socket
    clientSocket = socket(AF_INET, SOCK_STREAM)

    #Get remote IP address
    servIP = gethostbyname(serverName)

    #Establish connection
    clientSocket.connect((servIP, portNum))
    return clientSocket



""" Function: startListening()
    Description: Creates a socket to which the server
        can connect and send data on the data port number
        specified on the command-line.
    Parameters: The data port number specified on the command-line.
    Pre-Conditions: The data port number must be defined.
    Post-Conditions: Returns a file descriptor to the opened
        socket.
"""
def startListening(dataPort):
    #Open socket
    dataSocket = socket(AF_INET, SOCK_STREAM)
    dataSocket.bind(('', dataPort))
    dataSocket.listen(1)
    return dataSocket



""" Function: makeRequest()
    Description: Depending on which arguments were received on
        the command-line, this function either sends a request
        to get the server's directory listing or a request
        to get the file specified.
    Parameters: The variable storing the result of filename, the variable
        storing the result of -l, the connection sockets file
        descriptor
    Pre-Conditions: Either a filename must be specified or the
        listDir variable must be True.
    Post-Conditions: Sends request to the server.
"""
def makeRequest(listDir, fileName, socketFD):
    #If listDir == True, send '-l' to server
    if listDir == True:
        command = "-l"
        socketFD.send(command.encode())
    #Else send the filename over
    else:
        socketFD.sendall(fileName.encode())



""" Function: getServResponse()
    Description: Receives the server's response to
        client's request and stores the response in a
        variable to determine flow control of directory
        listing -OR- file receipt.
    Parameters: socket file descriptor
    Pre-Conditions: The socket must be open and connected
        with the server.
    Post-Conditions: Returns the server's response as a string
"""
def getServResponse(socketFD):
    response = socketFD.recv(500).decode()
    response = response.rstrip('\0')
    response = response.rstrip()
    return response



""" Function: handleResponse()
    Description: Handles the server's response by either
        accepting and listing the server directory's contents
        -OR- accepting the file transfer.
    Parameters: The server's response, the connection
        file descriptor
    Pre-Conditions: There's an active connection between
        the client and server. The client has already
        made its request to the server.
    Post-Conditions: Will list the server's directory contents,
        accept a file transfer, or display an error message.
"""
def handleResponse(response, socketFD, transferFile, portNum):
    #If response is 'dir', accept directory contents
    if response == "dir":
        print ("Receiving directory structure from server: " + portNum)
        fileName = getServResponse(socketFD)
        while fileName != "~done":
            print(fileName)
            fileName = getServResponse(socketFD)
        return
    #If response is 'fil', accept file transfer
    elif response == "fil":
        receiveFile(transferFile, socketFD, portNum)
        print("File transfer complete.")
        return
    elif response == "nof":
        print ("Server says: FILE NOT FOUND")
        return
    #Else if response is 'unk' print error message
    else:
        print("command unknown")
        return




""" Function: receiveFile()
    Description: Opens a new file for writing the transferred
        file contents to. If file name already exists, prompts
        the user to rename the new file.
    Parameters: The file name specified on the command-line, the
        file descriptor for the connection.
    Pre-Conditions: There must be an open connection between
        the client and the server. The file name must be specified.
    Post-Conditions: The file transferred will be written to the
        client's directory.
"""
def receiveFile(fileName, socketFD, portNum):
    print("Receiving \"" + fileName + "\" from server on " + portNum)
    #Check file does not already exist
    #If file name exists, prompt user for action
    while os.path.isfile('./'+ fileName):
        #Handle renaming or replacement
        print("File name already in use.")
        fileName = raw_input("Please enter new name for file: ")

    #Open file for writing
    file = open(fileName, "w")
    #Receive file contents
    data = socketFD.recv(500)
    while data:
        file.write(data)
        data = socketFD.recv(500)
    #Close file
    file.close()

    return


if __name__ == '__main__':
    main()
