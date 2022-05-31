#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <iostream>
#include "vector"

using namespace std;
#define MAX_MSG 100
#define SERVER "0.0.0.0" // Local host ip
#define PORT 1500        // the port on which to listen for incoming data
class client
{
private:
    int n, serverLen;
    char msg[MAX_MSG];
    int sd, rc;
    struct sockaddr_in remoteServAddr; // server addresses
    int port;
    string hostServer;
    vector<int> readings;

public:
    client()
    {
        this->port = PORT;
        this->hostServer = SERVER;
    }

    void initClient()
    {
        memset((char *)&this->remoteServAddr, 0, sizeof(this->remoteServAddr));
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

    void AvgOverTime()
    {
        int sum = 0;
        for (auto &it : readings)
        {
            sum = sum + it;
        }
        sum = sum / 5;
        cout << "the average over time (5) is: " << sum << endl;
    }

    void GetRequest()
    {
        serverLen = sizeof(remoteServAddr);
        n = recvfrom(sd, msg, MAX_MSG, 0,
                     (struct sockaddr *)&remoteServAddr,
                     (socklen_t *)&serverLen);

        readings.push_back(stoi(msg));
        
        if (readings.size() == 5)
        {
            AvgOverTime();
            // AccOverTime();
            readings.clear();
        }

        if (n < 0)
        {
            cout << "Cannot Recieve data" << endl;
        }
        /* print received message */
        cout << "Echo From Server: " << msg << endl;
    }
    void SendAdd()
    {
        /* send an empty msg to the server*/
        string empty;
        rc = sendto(sd, empty.c_str(), 0, 0,
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
