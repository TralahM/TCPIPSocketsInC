#include "Practical.h"


int SetupTCPClientSocket(const char *host, const char *service){
    // Tell System what kind(s) of address info we want
    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria));
    addrCriteria.ai_family=AF_UNSPEC;
    addrCriteria.ai_socktype=SOCK_STREAM;
    addrCriteria.ai_protocol=IPPROTO_TCP;
    // Get address(es)
    struct addrinfo *servAddr;
    int rtnVal=getaddrinfo(host,service,&addrCriteria,&servAddr);
    if (rtnVal !=0)
        DieWithUserMessage("getaddrinfo() failed",gai_strerror(rtnVal));
    int sock=-1;
    for (struct addrinfo *addr = servAddr; addr !=NULL; addr=addr->ai_next){
        // Create a reliable, stream socket using TCP
        sock=socket(addr->ai_family, addr->ai_socktype,addr->ai_protocol);
        if (sock < 0)
            continue; // Socket Creation Failed;try next address
        // Establish connection to the echo server
        if (connect(sock, addr->ai_addr,addr->ai_addrlen)==0)
            break; // connection succeeded ; break and return socket
        close(sock); // Socket connection failed; try next address
        sock = -1;
    }
    freeaddrinfo(servAddr);
    return sock;
}
