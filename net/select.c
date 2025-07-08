#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <ctype.h>
#include <pthread.h>

pthread_mutex_t mutex;
typedef struct fdinfo
{
    int fd;
    int *maxfd;
    fd_set *rdset;
} FDInfo;

void *acceptConn(void *arg)
{
    printf("线和ID: %ld\n", pthread_self());
    FDInfo *info = (FDInfo *)arg;
    struct sockaddr_in cliaddr;
    int cliLen = sizeof(cliaddr);

    int cfd = accept(info->fd, (struct sockaddr *)&cliaddr, (socklen_t *)&cliLen);
    pthread_mutex_lock(&mutex);
    FD_SET(cfd, info->rdset);
    *info->maxfd = cfd > *info->maxfd ? cfd : *info->maxfd;
    pthread_mutex_unlock(&mutex);

    free(info);
    return NULL;
}

void *communication(void *arg)
{
    printf("线和ID: %ld\n", pthread_self());
    FDInfo *info = (FDInfo *)arg;
    char buf[1024] = {0};
    int len = recv(info->fd, buf, sizeof(buf), 0);

    if (len == 0)
    {
        printf("客户端关闭了连接...\n");
        pthread_mutex_lock(&mutex);
        FD_CLR(info->fd, info->rdset);
        pthread_mutex_unlock(&mutex);
        close(info->fd);
    }
    else if (len > 0)
    {
        printf("收到客户端%d的数据%s...\n", info->fd, buf);
        for (int j = 0; j < len; j++)
        {
            buf[j] = toupper(buf[j]);
        }
        send(info->fd, buf, strlen(buf) + 1, 0);
    }
    else
    {
        perror("read");
    }
    free(info);
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    // 1. 创建监听的fd
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd, (struct sockaddr *)&addr, sizeof(addr));

    // 3. 设置监听
    listen(lfd, 128);

    int maxfd = lfd;
    fd_set rdset;
    fd_set rdtemp;
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    while (1)
    {
        pthread_mutex_lock(&mutex);
        rdtemp = rdset;
        pthread_mutex_unlock(&mutex);
        printf("0000...\n");
        int num = select(maxfd + 1, &rdtemp, NULL, NULL, NULL);
        if (FD_ISSET(lfd, &rdtemp))
        {
            printf("11111...\n");
            pthread_t tid;
            FDInfo *info = (FDInfo *)malloc(sizeof(FDInfo));
            info->fd = lfd;
            info->maxfd = &maxfd;
            info->rdset = &rdset;
            pthread_create(&tid, NULL, acceptConn, info);
            pthread_detach(tid);
        }

        for (int i = 0; i < maxfd + 1; ++i)
        {
            if (i != lfd && FD_ISSET(i, &rdtemp))
            {
                pthread_t tid;
                FDInfo *info = (FDInfo *)malloc(sizeof(FDInfo));
                info->fd = i;
                info->rdset = &rdset;
                pthread_create(&tid, NULL, communication, info);
                pthread_detach(tid);
            }
        }
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
