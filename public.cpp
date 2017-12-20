#ifndef __SOCKET_MGR_H__
#define __SOCKET_MGR_H__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>



#define MAXSOCKFD 1024
#define MAX_MSG_BUF_HEAD 8
#define MAX_MSG_BUF 4000       //1024
#define BUFFLEN 1024
#define MAXCLIENT 50
#define RETRY_SEND 5

#define QUERY_SLAVE_TIME_INTERVAL 2*60*1000*1000  /*2 minutes*/

typedef struct fsocket_info fsock_info;

class CSocketMgr
{
public:
    CSocketMgr();
    ~CSocketMgr();


    /*停止本地服务*/
    int stop_sock_mgr();
    int sock_create(int, int, int);
    int sock_connect(int, struct sockaddr *, int);
    int sock_bind(int, struct sockaddr *, int);
    int sock_listen(int, int);
    int sock_send(int, const char *, unsigned int ,int);
    int sock_recv(int, char *, unsigned int *, int);
    void sock_close(int *fd);

    /*循环接收命令接收线程*/
    static void* s_accept_routine(void* context);
    static void *s_recv_routine(void *context);
    int send_request(char *, int);
    int accept_routine(int, fsock_info *, int);
    int recv_routine(fsock_info *);
    void *set_fd_to_arry(int, struct sockaddr_in, fsock_info *, int);

    int recv_msg(fsock_info *);

    int sync_send_and_recv(char const *ip, int port, const char *, char *, unsigned int send_len, unsigned int *recv_len);
    int sync_send(char const *ip, int port, const char * buf, int len);
private:
    int ser_sock_fd;
    pthread_t accept_thread;
    pthread_t recv_thread;
    bool _running;
    int _socket;//本地socket
    pthread_t client_thread[MAXCLIENT];
    CXmlBuilder _node_info;
    MutexLock _mutex;
    char *_pBuf;
    /*可以组合公共网络类*/
};


typedef struct fsocket_info
{
    bool sock_fd_use_flag;
    time_t accept_time;
    int sock_fd;
    struct sockaddr_in client_addr;
} fsock_info;

typedef struct ObjectFd
{
    int listen_type;
    fsock_info *watch_sock;
    class CSocketMgr _sock_mgr;
} ObjectFd;

typedef struct Client_Info
{
    char ip[16];
    int port;
    class CSocketMgr *client_this;
} Client_Info;

#endif
//定义结束
