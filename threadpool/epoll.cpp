#include "epoll.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <unordered_map>
#include "threadpool.h"
#include <fcntl.h>

#define MAX_EVENTS 100
#define PORT 9999

void handle_new_connection(int epoll_fd, int listen_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd;

    while ((client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) > 0)
    {
        std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

        // 设置非阻塞模式
        int flags = fcntl(client_fd, F_GETFL, 0);
        fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

        // 将客户端套接字添加到 epoll
        struct epoll_event event;
        event.data.fd = client_fd;
        event.events = EPOLLIN | EPOLLET; // 边缘触发
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0)
        {
            perror("epoll_ctl");
            close(client_fd);
        }
    }
}

void handle_client_data(int epoll_fd, int client_fd)
{
    char buffer[1024];
    auto bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read > 0)
    {
        std::cout << "Received data from client: " << buffer << std::endl;
        // 回显数据
        write(client_fd, buffer, bytes_read);
    }

    if (bytes_read == 0)
    {
        std::cout << "Client disconnected" << std::endl;
        close(client_fd);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
    }
}

int run()
{
    int listen_fd, epoll_fd;
    struct sockaddr_in server_addr;
    struct epoll_event event, events[MAX_EVENTS];
    int nfds, client_fd;

    // 创建监听套接字
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket");
        return 1;
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(listen_fd);
        return 1;
    }

    // 开始监听
    if (listen(listen_fd, 10) < 0)
    {
        perror("listen");
        close(listen_fd);
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // 创建 epoll 文件描述符
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        perror("epoll_create1");
        close(listen_fd);
        return 1;
    }

    // 将监听套接字添加到 epoll
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET; // 边缘触发
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) < 0)
    {
        perror("epoll_ctl");
        close(listen_fd);
        close(epoll_fd);
        return 1;
    }

    ThreadPool pool(4, 16);
    // 主循环
    while (true)
    {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0)
        {
            perror("epoll_wait");
            break;
        }

        for (int n = 0; n < nfds; ++n)
        {
            int client_fd = events[n].data.fd;
            if (client_fd == listen_fd)
            {
                pool.AddTask(std::make_shared<Task<int, int>>(handle_new_connection, epoll_fd, listen_fd));
            }
            else
            {
                pool.AddTask(std::make_shared<Task<int, int>>(handle_client_data, epoll_fd, client_fd));
            }
        }
    }

    // 清理
    close(listen_fd);
    close(epoll_fd);

    return 0;
}
