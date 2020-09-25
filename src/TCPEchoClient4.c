#include "Practical.h"


int main(int argc, char *argv[])
{
    if (argc < 3 || argc >4) //Test for correct number of arguments
        DieWithUserMessage("Parameter(s)","<Server Address> <Echo Word> [<Server Port>]");

    char *servIP = argv[1];
    char *echoString=argv[2];
    // Third arg (optional): server port (numeric). 7 is well-known echo port
    in_port_t servPort = (argc==4) ? atoi(argv[3]) : 7;
    // Create a reliable, stream socket using TCP
    int sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (sock < 0)
        DieWithSystemMessage("socket() failed");

    // Construct the server address structure
    struct sockaddr_in servAddr; // Server address
    memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
    servAddr.sin_family=AF_INET; //IPv4 address family
    // Convert address
    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
        DieWithUserMessage("inet_pton() failed","invalid address string");
    else if (rtnVal < 0)
        DieWithSystemMessage("inet_pton() failed");
    servAddr.sin_port=htons(servPort); //Server port

    // Establish the connection to the echo server.
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("connect() failed.");
    size_t echoStringLen = strlen(echoString); //Determine Input Length
    // Send the string too the server
    ssize_t numBytes = send(sock, echoString, echoStringLen,0);
    if (numBytes < 0)
        DieWithSystemMessage("send() failed");
    else if (numBytes != echoStringLen)
        DieWithUserMessage("send()", "sent unexpected number of bytes");
    // Receive the Same String back from the server
    unsigned int totalBytesRcvd=0; // Count oof total bytes received
    fputs("Received: ", stdout); // Setup to print the echoed string
    while (totalBytesRcvd < echoStringLen){
        char buffer[BUFSIZE]; // I/O buffer
        //receive upto the buffer size (minus 1 to leave space
        //for a null terminator) bytes from the sender
        numBytes=recv(sock, buffer, BUFSIZE-1, 0);
        if (numBytes < 0)
            DieWithSystemMessage("recv() failed");
        else if (numBytes == 0)
            DieWithUserMessage("recv()","connection closed prematurely");
        totalBytesRcvd += numBytes; //keep tally of total bytes
        buffer[numBytes]='\0'; // Terminate the string!
        fputs(buffer,stdout); // Print the echo buffer
    }
    fputc('\n',stdout); // Print a final linefeed
    close(sock);
    exit(0);
}
