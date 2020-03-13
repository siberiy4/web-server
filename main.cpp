#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    std::cout << "start" << std::endl;

    int sockfd, len, s, port = 8080, mtu = 1500, pid;
    sockaddr_in listen_IPv4, sender_IPv4;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    listen_IPv4.sin_family = AF_INET;
    listen_IPv4.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_IPv4.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&listen_IPv4, sizeof(listen_IPv4)) == -1)
    {
        std::perror("bind");
        return EXIT_FAILURE;
    }

    listen(sockfd, 5);
    len = sizeof(sender_IPv4);
    
    while (1)
    {
        s = accept(sockfd, (struct sockaddr *)&sender_IPv4, (socklen_t *)&len);
        //プロセス分ける
        pid = fork();
        std::cout << s << " " << std::endl;

        //子プロセスの内容
        if (pid == 0)
        {
            std::string packet;
            //accept()で異常がないか確認する
            if (s < 0)
            {
                std::perror("accept");
                return EXIT_FAILURE;
            }

            //接続元IP、PORTを表示する
            std::cout << inet_ntoa(sender_IPv4.sin_addr) << " " << ntohs(sender_IPv4.sin_port) << std::endl;

            //ブラウザから送られてきたGETメソッドを受け取る
            recv(s, &packet, mtu, 0);
            close(s);
        }
        else
        {
            close(s);
            continue;
        }
    }
    close(sockfd);

    std::cout << "end" << std::endl;

    return EXIT_SUCCESS;
}
