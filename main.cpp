#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    std::cout << "start" << std::endl;

    int sockfd, len, s, port = 20000, pid;
    struct sockaddr_in listen_IPv4, sender_IPv4;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    listen_IPv4.sin_family = AF_INET;
    listen_IPv4.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_IPv4.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&listen_IPv4, sizeof(listen_IPv4)) == -1)
    {
        perror("bind");
        return EXIT_FAILURE;
    }

    listen(sockfd, 5);

    while (1)
    {
        s = accept(sockfd, (struct sockaddr *)&sender_IPv4, (socklen_t *)sizeof(sender_IPv4));

        pid = fork();

        if (pid == 0)
        {
            if (s < 0)
            {
                perror("accept");
                return EXIT_FAILURE;
            }

            std::cout << inet_ntoa(sender_IPv4.sin_addr) << " " << ntohs(sender_IPv4.sin_port) << std::endl;

            close(s);
        }
        else
        {
            close(s);
            continue;
        }
    }
    close(sockfd);
    
    std::cout<< "end" << std::endl;

    return 0;
}
