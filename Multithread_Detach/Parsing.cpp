#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <cstdlib>
#include <pthread.h>
#include <vector>

using namespace std;
void *respond(void *arg);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, port, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int thread_count = 0;
    pthread_t iret[100];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    cout << "Please enter listening port:";
    cin.getline(buffer, 1024, '\n');
    port = atoi(buffer);
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        exit(EXIT_FAILURE);

    if (listen(server_fd, 3) < 0)
        exit(EXIT_FAILURE);

    cout << "Listen to new tcp request..." << endl;

    while ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)))
    {
        if (pthread_create(&iret[thread_count], NULL, respond, &new_socket) != 0)
        {
            cout << "Failed to create thread\n";
            return -1;
        }
        thread_count++;
        cout << "Number of connections:" << thread_count << endl;

        if (thread_count >= 100)
        {
            thread_count = 0;
            while (thread_count < 100)
            {
                pthread_join(iret[thread_count++], NULL);
            }
            thread_count = 0;
        }
    }

    return 0;
}

void *respond(void *arg)
{
    int new_socket;
    char buffer[1024] = {0};
    char return_message[1024] = {0};
    int nBytes, pos;
    char *Tokens;

    new_socket = *((int *)arg);
    nBytes = read(new_socket, buffer, 1024);
    buffer[nBytes] = 0;
    cout << "Recevied from user:" << buffer << endl;
    Tokens = strtok(buffer, " ");
    while (Tokens != NULL)
    {
        cout << Tokens << endl;
        Tokens = strtok(NULL, " ");
    }
    send(new_socket, "Quit", strlen("Quit"), 0);
    //send(new_socket, buffer, strlen(buffer), 0);
    cout << "Disconnected!" << endl;
    close(new_socket);
}