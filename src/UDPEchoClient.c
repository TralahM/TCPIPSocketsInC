#include "Practical.h"


int main(int argc, char *argv[])
{
    if (argc < 3 || argc >4) //Test for correct number of arguments
        DieWithUserMessage("Parameter(s)","<Server Address> <Echo Word> [<Server Port>]");

    char *server = argv[1];
    char *echoString=argv[2];
    // Third arg (optional): server port (numeric). 7 is well-known echo port
    char *servPort = (argc==4) ? argv[3] : "echo";
    // Get address(es)
    struct addrinfo addrCriteria; // Criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
    addrCriteria.ai_family = AF_UNSPEC;
    addrCriteria.ai_socktype = SOCK_DGRAM; // Only datagram sockets
    addrCriteria.ai_protocol = IPPROTO_UDP;
    struct addrinfo *servAddr; // List of server addresses
    int rtnVal = getaddrinfo(server, servPort, &addrCriteria, &servAddr);
    if (rtnVal != 0)
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

    // Create a connected UDP Socket
    //// Create a datagram/UDP socket
    int sock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol); // Socket descriptor for client
    //
    if (sock < 0)
        DieWithUserMessage("SetupUDPClientSocket() failed","unable to connect");

    size_t echoStringLen = strlen(echoString); //Determine Input Length
    // Send the string too the server
    // Send the string to the server
    ssize_t numBytes = sendto(sock, echoString, echoStringLen, 0, servAddr->ai_addr, servAddr->ai_addrlen);
    if (numBytes < 0)
        DieWithSystemMessage("send() failed");
    else if (numBytes != echoStringLen)
        DieWithUserMessage("send()", "sent unexpected number of bytes");
    // Receive the Same String back from the server
    // Receive a response

    struct sockaddr_storage fromAddr; // Source address of server
    // Set length of from address structure (in-out parameter)
    socklen_t fromAddrLen = sizeof(fromAddr);
    char buffer[UDPBUFSIZE + 1]; // I/O buffer
    numBytes = recvfrom(sock, buffer, UDPBUFSIZE, 0, (struct sockaddr *) &fromAddr, &fromAddrLen);
    if (numBytes < 0)
        DieWithSystemMessage("recvfrom() failed");
    else if (numBytes != echoStringLen)
        DieWithUserMessage("recvfrom() error", "received unexpected number of bytes");

    // Verify reception from expected source
    /* if (!SockAddrsEqual(servAddr->ai_addr, (struct sockaddr *) &fromAddr)) */
    /*     DieWithUserMessage("recvfrom()", "received a packet from unknown source"); */

    freeaddrinfo(servAddr);

    buffer[echoStringLen] = '\0'; // Null-terminate received data
    printf("Received: %s\n", buffer); // Print the echoed string
    close(sock);
    exit(0);
}
