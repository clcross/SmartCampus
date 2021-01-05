#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int sock = 0, nBytes;
    int port = 0;
    char ip_string[17];
    struct sockaddr_in serv_addr;
    char user_input[1024] = {0};
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << endl
             << "Socket creation error" << endl;
        return -1;
    }
    cout << "Please enter TCP server IP:";
    cin.getline(ip_string, 1024, '\n');
    cout << "Please enter TCP Server Port:";
    cin.getline(user_input, 1024, '\n');
    port = atoi(user_input);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    serv_addr.sin_addr.s_addr = inet_addr(ip_string);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << endl
             << "Connection Failed" << endl;
        return -1;
    }
    do
    {
        cout << "Message to server:";
        cin.getline(user_input, 1024, '\n');
        if (send(sock, user_input, strlen(user_input), 0) == -1)
            break;
        if ((nBytes = read(sock, buffer, 1024)) == -1)
            break;
        buffer[nBytes] = 0;
        printf("Server replied:%s\n", buffer);
    } while (strcmp(buffer, "Quit") != 0);
    close(sock);
    return 0;
}