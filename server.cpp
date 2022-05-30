
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <iostream>
#include <bits/stdc++.h>

using namespace std;
#define MAX_MSG 100
#define PORT 1500

class UDP_Server
{
private:
    int sd, rc, n, cliLen, port;
    struct sockaddr_in cliAddr, servAddr;
    char msg[MAX_MSG];

public:
    UDP_Server()
    {
        this->port = PORT;
    }

    void initServer()
    {
        //  this->LOCAL_SERVER_PORT = port;
        this->sd = socket(AF_INET, SOCK_DGRAM, 0); // socket creation, sock_dgram for udp
        if (sd < 0)
        { // if failed then sd is smaller then 0
            cout << "failed to open socket" << endl;
            exit(1);
        }
        /* bind local server port */
        this->servAddr.sin_family = AF_INET;
        this->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->servAddr.sin_port = htons(port);
        this->rc = bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
        if (rc < 0)
        {
            cout << " cannot bind port number " << port << " " << endl;
            exit(1);
        }

        cout << "INIT GOT EXECUTED SUCCESFULLY" << endl;
        cout << "Waiting for data on port " << port << endl;
    }

    void DoOperation()
    {
        GetRequest();
        /* server infinite loop */
        while (1)
        {
            SendTemp();
        }
    }
    void GetRequest()
    {

        /* init buffer */
        memset(msg, 0x0, MAX_MSG);
        /* receive message */
        cliLen = sizeof(cliAddr);
        n = recvfrom(sd, msg, MAX_MSG, 0,
                     (struct sockaddr *)&cliAddr, (socklen_t *)&cliLen);
        cout << "Connected to client: " << &cliAddr << endl;
        
    }
    void SendTemp()
    {
        srand(time(0));
        int random = 10 + rand() % 30;
        // cout << to_string(random).c_str()<< endl;
        usleep(1000000);
        rc = sendto(sd, to_string(random).c_str(), strlen(to_string(random).c_str()) + 1, 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
        if (rc < 0) // if send faild
        {
            cout << "Cannot send data\n";
            close(sd);
            exit(1);
        }
    }
};

int main(int argc, char *argv[])
{
    UDP_Server x;
    x.initServer();
    x.DoOperation();
    return 0;
}
