#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>   /* memset() */
#include <string>     /* memset() */
#include <sys/time.h> /* select() */
#include <iostream>

using namespace std;
//#define REMOTE_SERVER_PORT 1600
#define MAX_MSG 100
#define SERVER "0.0.0.0"  // Local host ip
#define PORT 1500  // the port on which to listen for incoming data
class client
{
private:
    int n, serverLen;
    char msg[MAX_MSG];
    int sd, rc;
    struct sockaddr_in remoteServAddr; //server addresses
    int port;
    string hostServer;

public:
    client()
    {
        this->port = PORT;
        this->hostServer = SERVER;
    }

    void initClient()
    {
        memset((char*)&this->remoteServAddr, 0, sizeof(this->remoteServAddr));
        this->remoteServAddr.sin_family = AF_INET;
        this->remoteServAddr.sin_port = htons(this->port);

        /* socket creation */
        this->sd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sd < 0)
        {
            cout << "Cannot open socket !" << endl;
            exit(1);
        }

    }
    void DoOperation()
    {
        SendAdd();
        while (1)
        {
            GetRequest();
        }
    }
    void GetRequest()
    {
        serverLen = sizeof(remoteServAddr);
        n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&remoteServAddr, (socklen_t *)&serverLen);
        if (n < 0)
        {
            cout << "Cannot Recieve data" << endl;
        }
        /* print received message */
        cout << "Echo From Server: " << msg << endl;
    }
    void SendAdd()
    {
        /* send data */
        string empty;
        rc = sendto(sd, empty.c_str(), strlen(empty.c_str()) + 1, 0,
                    (struct sockaddr *)&remoteServAddr,
                    sizeof(remoteServAddr));
        if (rc < 0)
        {
            cout << "Cannot send data !" << endl;
            close(sd);
            exit(1);
        }
    }


    ~client()
    {
    }
};

int main(int argc, char *argv[])
{
 

    client x;

    x.initClient();
    x.DoOperation();
    return 1;
}
