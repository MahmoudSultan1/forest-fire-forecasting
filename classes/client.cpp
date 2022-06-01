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
#include <numeric>

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
    int counter = 0;
    string hostServer;
    vector<int> readings;
    float accOverTime = 0;
    int avgOverTime = 0;

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
            throw "failed to init";
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
        n = recvfrom(sd, msg, MAX_MSG, 0,
                     (struct sockaddr *)&remoteServAddr,
                     (socklen_t *)&serverLen);
        if (n < 0)
        {
            cout << "Cannot Recieve data" << endl;
            close(sd);
            exit(1);
        }
        readings.push_back(stoi(msg));

        if (readings.size() > 1) // checking if readings has atleast 2 values
        {
            int curr = readings[readings.size() - 1];
            int prev = readings[readings.size() - 2];

            AccOverTime(curr, prev);
        }
        AvgOverTime();

        if (readings.size() % 5 == 0) // printing the
        {
            counter++;
            cout << "Accumlation over time is " << accOverTime << endl;
            cout << "the average over time  "
                 << "for " << counter * 5
                 << " seconds is: " << avgOverTime << "°C" << endl;
        }

        /* print received message */
        // cout << "Echo From Server: " << msg << endl;
    }
    void SendAdd()
    {
        /* send an empty msg to the server for address acknowledgement */
        string empty;
        rc = sendto(sd, empty.c_str(), 0, 0,
                    (struct sockaddr *)&remoteServAddr,
                    sizeof(remoteServAddr));
        if (rc < 0)
        {
            throw "cannot send";
            close(sd);
            exit(1);
        }
    }

    void setReadings(int x)
    {
        this->readings.push_back(x);
    }

    float getAccOverTime()
    {
        return accOverTime;
    }
    int getAvgOverTime()
    {
        return avgOverTime;
    }

    void AvgOverTime()
    {
        int sum = 0;
        sum = accumulate(readings.begin(), readings.end(), 0);
        avgOverTime = sum / readings.size(); // getting the average
    }

    /* Assumption: The accumlation over time is calculated as a summation of the change in temprature*/
    void AccOverTime(int curr, int prev)
    {
        accOverTime += curr - prev;
    }

    ~client()
    {
    }
};
