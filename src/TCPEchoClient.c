#include "Practical.h"


int main(int argc, char *argv[])
{
    if (argc < 3 || argc >4) //Test for correct number of arguments
        DieWithUserMessage("Parameter(s)","<Server Address> <Echo Word> [<Server Port>]");

    char *server = argv[1];
    char *echoString=argv[2];
    // Third arg (optional): server port (numeric). 7 is well-known echo port
    char *service = (argc==4) ? argv[3] : "echo";
    // Create a connected TCP Socket
    int sock= SetupTCPClientSocket(server,service);
    // Create a reliable, stream socket using TCP
    //
    if (sock < 0)
        DieWithUserMessage("SetupTCPClientSocket() failed","unable to connect");

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
