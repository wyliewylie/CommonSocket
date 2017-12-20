#include "SocketMgr.h"

#define UNIX_DOMAIN "/tmp/unix.domain"//我们创建一个本地域套接字

class CLocalListener
{
public:
    CLocalListener(void);
    ~CLocalListener(void);

    //我们只演示如何用公共类开启一个本地监听服务
    int start_local_service();

private:
    int ser_sock_fd;
    pthread_t recv_thread;
    int _socket;//本地socket
    CSocketMgr _sock_mgr;
};
