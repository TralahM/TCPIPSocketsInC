#include "Practical.h"

static const int MAXPENDING=5;

void HandleUDPClient(int clntSocket){
    char buffer[UDPBUFSIZE]; // Buffer for echo string
    struct sockaddr_storage clntAddr;
    socklen_t clntAddrLen = sizeof(clntAddr);
    // receive msg from client
    ssize_t numBytesRcvd = recvfrom(clntSocket,buffer, UDPBUFSIZE, 0,(struct sockaddr *)&clntAddr, &clntAddrLen);
    PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
    if (numBytesRcvd < 0)
        DieWithSystemMessage("recvfrom() failed");
    // sendto received string and receive again until end of stream
    // Echo msg back to client
    ssize_t numBytesSent=sendto(clntSocket,buffer,numBytesRcvd,0,(struct sockaddr *)&clntAddr, sizeof(clntAddr));
    if (numBytesSent < 0)
        DieWithSystemMessage("sendto() failed");
    else if (numBytesSent !=numBytesRcvd)
        DieWithUserMessage("sendto()","sent unexpected number of bytes");
    /* close(clntSocket); // Close client Socket */
}

int SetupUDPServerSocket(const char *service){
    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
    addrCriteria.ai_family = AF_UNSPEC; // Any address family
    addrCriteria.ai_flags = AI_PASSIVE; // Accept on any address/port
    addrCriteria.ai_socktype = SOCK_DGRAM; // Only stream sockets
    addrCriteria.ai_protocol = IPPROTO_UDP; // Only TCP protocol

    struct addrinfo *servAddr; // List of server addresses
    int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
    if (rtnVal != 0)
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

    int servSock = -1;
    for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
        // Create a UDP socket
        servSock = socket(servAddr->ai_family, servAddr->ai_socktype,
                servAddr->ai_protocol);
        if (servSock < 0)
            continue; // Socket creation failed; try next address

        // Bind to the local address and set socket to list
        if ((bind(servSock, servAddr->ai_addr, servAddr->ai_addrlen) == 0)) {
            // Print local address of socket
            struct sockaddr_storage localAddr;
            socklen_t addrSize = sizeof(localAddr);
            if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
                DieWithSystemMessage("getsockname() failed");
            fputs("Binding to ", stdout);
            PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
            fputc('\n', stdout);
            break; // Bind and list successful
        }

        close(servSock); // Close and try again
        servSock = -1;
    }
    // Free address list allocated by getaddrinfo()
    freeaddrinfo(servAddr);
    return servSock;
}

