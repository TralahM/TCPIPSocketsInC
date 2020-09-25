#include "Practical.h"


int main(int argc, char *argv[]) {

    if (argc != 2) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Port/Service>");

    char *service = argv[1]; // First arg: local port

    // Create socket for incoming connections
    int servSock = SetupUDPServerSocket(service);
    if (servSock < 0)
        DieWithUserMessage("SetupUDPServerSocket() failed", service);

    for (;;) { // Run forever
        // New connection creates a connected client socket

        HandleUDPClient(servSock); // Process client
        /* close(servSock); */
    }
    // NOT REACHED
}
