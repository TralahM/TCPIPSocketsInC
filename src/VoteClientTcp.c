#include "Practical.h"
#include "VoteProtocol.h"
#include "VoteEncoding.h"
#include "Framer.h"


int main(int argc, char *argv[])
{
    if (argc < 4 || argc >5 )
        DieWithUserMessage("Parameter(s)", "<Server> <Port/Service> <Candidate> [I]");
    char *server=argv[1];
    char *service=argv[2];
    int candi=atoi(argv[3]);
    if (candi < 0 || candi > MAX_CANDIDATE)
        DieWithUserMessage("Candindate # not valid", argv[3]);

    bool inq=argc>4 && strcmp(argv[4], "I")==0;

    // Create a connected TCP socket
    int sock=SetupTCPClientSocket(server,service);
    if (sock<0)
        DieWithUserMessage("SetupTCPClientSocket() failed","unable to connect");
    FILE *str= fdopen(sock,"r+"); // Wrap for stream I/O
    if (str==NULL)
        DieWithSystemMessage("fdopen() failed");
    // Set up info for a request
    VoteInfo vi;
    memset(&vi,0,sizeof(vi));

    vi.isInquiry=inq;
    vi.candidate=candi;

    // Encode For Transmission
    uint8_t outbuf[MAX_WIRE_SIZE];
    size_t reqSize=Encode(&vi,outbuf, MAX_WIRE_SIZE);

    // Print info
    printf("Sending %d-byte %s for candidate %d...\n", reqSize,(inq ? "inquiry" : "vote"),candi);
    //Frame and send
    if (PutMsg(outbuf,reqSize,str)<0)
        DieWithSystemMessage("PutMsg() failed");

    // Receive and print response
    //
    uint8_t inbuf[MAX_WIRE_SIZE];
    size_t respSize=GetNextMsg(str,inbuf,MAX_WIRE_SIZE); // Get the msg
    if (Decode(inbuf,respSize,&vi)){
        printf("Received:\n");
        if (vi.isResponse)
            printf(" Response to ");
        if (vi.isInquiry)
            printf("inquiry ");
        else
            printf("vote ");
        printf("for candidate %d\n", vi.candidate);
        if (vi.isResponse)
            printf(" count = %llu\n",vi.count);
    }
    //Close up
    fclose(str);
    exit(0);

    //
}
