#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include <string>

using namespace std;

void *respond(void *arg);
int thread_count = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
vector<string> tokens;

int main(int argc, char const *argv[])
{
    int serverSocket, new_socket, port, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    pthread_t tid[100];

    // Creating socket file descriptor
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully serverSocket socket to the port 8080
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    cout << "Please enter listening port:";
    cin.getline(buffer, 1024, '\n');
    port = atoi(buffer);
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
        exit(EXIT_FAILURE);

    if (listen(serverSocket, 3) < 0)
        exit(EXIT_FAILURE);

    cout << "Listen to new tcp request..." << endl;

    while ((new_socket = accept(serverSocket, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)))
    {
        if (pthread_create(&tid[thread_count], NULL, respond, &new_socket) != 0)
        {
            cout << "Failed to create thread\n";
            return -1;
        }
        else
        {
            pthread_detach(tid[thread_count]);
            pthread_mutex_lock(&mutex1);
            thread_count++;
            pthread_mutex_unlock(&mutex1);
            cout << "Number of connections:" << thread_count << endl;
        }

        while (thread_count > 99) // If there are 100 threads
        {
            sleep(1); // wait for a thread to end;
        }
    }
    return 0;
}

void *respond(void *arg)
{
    int new_socket;
    char buffer[1024] = {0};
    char return_message[1024] = {0};
    int nBytes;
    char *temp, *t;

    new_socket = *((int *)arg);
    do
    {
        nBytes = read(new_socket, buffer, 1024);
        buffer[nBytes] = 0;
        cout << "Recevied from user:" << buffer << endl;
        temp = strdup(buffer);
        t = strtok(temp, " ");

        while (t != NULL)
        {
            tokens.push_back(t);
            t = strtok(NULL, " ");
        }
        cout << "Tokens are:" << endl;
        for (auto i : tokens)
            cout << i << endl;
        // Clearn tokens vector
        tokens.clear();
        send(new_socket, buffer, strlen(buffer), 0); // Sending back to the user
    } while (strcmp(buffer, "Quit") != 0);
    cout << "Client disconnected!" << endl;
    pthread_mutex_lock(&mutex1);
    thread_count--;
    cout << "Number of connections:" << thread_count << endl;
    pthread_mutex_unlock(&mutex1);
    close(new_socket);
    pthread_exit(NULL);
}