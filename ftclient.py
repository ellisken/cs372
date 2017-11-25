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
import argparse

def main():
    #Get server name, server port number, command,
    #file name, and data port number from console
    parser = argparse.ArgumentParser()
    parser.add_argument('server', nargs=1, default="none", type=str)
    parser.add_argument('servPort', nargs=1, default=0, type=int)
    #parser.add_argument('-l', dest='listDir', action='store_true')
    #parser.add_argument('-g', dest='fileName', default="none", type=str)
    parser.add_argument('dataPort', nargs=1, default=0, type=int)


    args = parser.parse_args()
    server = args.server[0]
    servPort = args.servPort[0]
    #listDir = args.listDir
    #fileName = args.fileName
    dataPort = args.dataPort[0]


    #Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM)
    #Get remote IP address
    servIP = gethostbyname(server)
    clientSocket.connect((servIP, servPort))
    print("Connection established with server on port: " + str(servPort))

    #Start listening on specified dataPort
    listenSocket = socket(AF_INET, SOCK_STREAM)
    listenSocket.bind(('', dataPort))
    listenSocket.listen(1)
    print("Listening for data connections on port: " + str(dataPort))

    testSentence = "Hello, server!"
    clientSocket.sendall(testSentence)
    response = clientSocket.recv(18)
    print response

    dataSocket, addr = listenSocket.accept()
    poopie = dataSocket.recv(18).decode()
    print('received response: ' + poopie)


    #clientSocket.close()


""" Function: initiateContact
    Description:
    Parameters:
    Pre-Conditions:
    Post-Conditions:
"""


""" Function: makeRequest
    Description:
    Parameters:
    Pre-Conditions:
    Post-Conditions:
"""


""" Function: receiveFile
    Description:
    Parameters:
    Pre-Conditions:
    Post-Conditions:
"""


if __name__ == '__main__':
    main()
